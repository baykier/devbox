/*
 * Tech support by zhaoshunyao
 * Email: yatan35@163.com
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <signal.h>
#include <errno.h>
#include <pthread.h>

/* Log levels */
#define DEBUG_LOG 0
#define TRACE_LOG 1
#define NOTICE_LOG 2
#define WARNING_LOG 3

#define MAX_TABLE_NUM 256
#define MAX_LOGMSG_LEN 4096
#define MAX_CONFIGLINE 1024
#define MAX_EPOLL_SIZE 2048
#define BACKLOG 128
#define MAXBUF 1024

/* Id table configuration structure */
struct id_table_conf {
	int id_min;
	int id_max;
	int id_increment;
	char *table_name;
};

/* Id allocator structure */
struct id_allocator {
	int id_min;
	int id_max;
	int id_increment;
	int id_current;
	int table_status; 	/* 0=>close, 1=>only read, 2=>only write */
	char *table_name;
	char *table_file;
	FILE *table_fp;
	struct id_allocator *next;
};

/* Global server state structure */
struct global_server {
    int port;
    char *bindaddr;
	char *datadir;
    char *pidfile;
    char *logfile;
	int daemonize;

	/* Operation variable */
	int listen_fd;
	int epoll_fd;
	int client_fd;
	int client_sockfd;
	int sockopt;
	socklen_t sockaddr_len;
	struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
	struct epoll_event ev;
	struct epoll_event events[MAX_EPOLL_SIZE];
	char recv_buf[MAXBUF];
	char send_buf[MAXBUF];
	ssize_t buf_len;
	struct id_table_conf table_conf[MAX_TABLE_NUM];
	int tablenum;
};

struct global_server server;			/* server global state */
struct id_allocator *id_head = NULL;	/* id storage tables addr */
pthread_t write_disk_pid;

void * thread_write_disk(void * arg);

void write_log(int level, const char *fmt, ...) {
    const char *c = ".-*#";
    time_t now = time(NULL);
    va_list ap;
    FILE *fp;
    char buf[64];
    char msg[MAX_LOGMSG_LEN];

    if (level < 0 || level > 3) return;

    fp = (server.logfile == NULL) ? stdout : fopen(server.logfile, "a");
    if (!fp) return;

    va_start(ap, fmt);
    vsnprintf(msg, sizeof(msg), fmt, ap);
    va_end(ap);

    strftime(buf, sizeof(buf), "%b %d %H:%M:%S", localtime(&now));
    fprintf(fp,"[%d] %s %c %s\n", (int)getpid(), buf, c[level], msg);
    fflush(fp);

    if (server.logfile) fclose(fp);
}

void setnonblocking(int sockfd) {
	if (fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFD, 0) | O_NONBLOCK) == -1) {
        write_log(WARNING_LOG, "setnonblocking fail");
		exit(1);
    }
}

char * rtrim(char *str, char c) {
	char *h = str;
    for (; *str != '\0'; str++);
    for (str--; *str == c || *str == 0x20 || *str == '\t' || *str == '\r' || *str == '\n'; str--);
    *++str = '\0';
	return h;
}

char * trim(char *str) {
    char *str_last, *str_cur;
    if(str == NULL) return NULL;
    for(; *str == 0x20 || *str == '\r' || *str == '\n' || *str == '\t'; ++str);
    for(str_last = str_cur = str; *str_cur != '\0'; ++str_cur) {
        if(*str_cur != 0x20 && *str_cur != '\r' && *str_cur != '\n' && *str_cur != '\t') {
            str_last = str_cur;
		}
	}
    *++str_last = 0;
    return str;
}

void split(char **arr, char *str, const char *d) {
	char *s = NULL; 
	s = strtok(str, d);
	while (s != NULL) {
		*arr++ = trim(s);
		s = strtok(NULL, d);
	}
}

void init_config() {
    server.port = 5001;
    server.bindaddr = "127.0.0.1";
	server.daemonize = 1;
    server.pidfile = strdup("/home/idallocator.pid");
	server.logfile = strdup("/home/idallocator.log"); /* NULL = log on standard output */
	server.sockaddr_len = sizeof(struct sockaddr_in);
	server.sockopt = 1;
	server.tablenum = 0;
}

void load_config(char *filename) {
    FILE *fp;
    char buf[MAX_CONFIGLINE+1];
	char *conf = NULL;
	char *err = NULL;
	char *line = NULL;
	char *argv[2];
    int linenum = 0;
	int tablefag = 0;
	int i;

	conf = trim(filename);
	if ((fp = fopen(conf, "r")) == NULL) {
		fprintf(stderr, "can't open config file '%s'", conf);
		exit(1);
	}

    while (fgets(buf, MAX_CONFIGLINE + 1, fp) != NULL) {
        linenum++;
		for(i=0; i < strlen(buf); i++) {
			if(buf[i] == '#') {
				buf[i] = '\0';
			}
		}
        line = trim(buf);

        /* Skip comments and blank lines*/
        if (line[0] == '#' || line[0] == '\0') {
            line[0] = '\0';
            continue;
        }

        /* Split into arguments */
		memset(argv, 0, sizeof(argv));
		split(argv, line, "=");

        /* Execute config directives */
        if (!strcmp(argv[0], "listen_ip") && argv[1][0] != '\0') {
            server.bindaddr = strdup(argv[1]);
        } else if (!strcmp(argv[0], "listen_port") && argv[1][0] != '\0') {
            server.port = atoi(argv[1]);
            if (server.port < 0 || server.port > 65535) {
                err = "Invalid port"; goto loaderr;
            }
        } else if (!strcmp(argv[0], "datadir") && argv[1][0] != '\0') {
			 server.datadir = strdup(rtrim(argv[1], '/'));
        } else if (!strcmp(argv[0], "logfile") && argv[1][0] != '\0') {
            FILE *logfp;
            server.logfile = strdup(argv[1]);
			if ((logfp = fopen(server.logfile, "a")) == NULL) {
				snprintf(err, sizeof(err), "Can't open the log file: %s", strerror(errno));
				goto loaderr;
			}
			fclose(logfp);
        } else if (!strcmp(argv[0], "pidfile") && argv[1][0] != '\0') {
            server.pidfile = strdup(argv[1]);
        } else if (!strcmp(argv[0], "[table]")) {
			tablefag = 1;
            continue;
        }  else if (!strcmp(argv[0], "[/table]")) {
			tablefag = 0;
			server.tablenum++;
            continue;
        }  else if (tablefag) {
			if (server.tablenum >= MAX_TABLE_NUM) {
				fprintf(stderr, "the number of tables more than %d, please reduce the number of tables\n", server.tablenum);
				exit(1);
			}
			if (!strcmp(argv[0], "table_name") && argv[1][0] != '\0') {
				server.table_conf[server.tablenum].table_name = strdup(argv[1]);
			} else if (!strcmp(argv[0], "id_min") && argv[1][0] != '\0') {
				server.table_conf[server.tablenum].id_min = atoi(argv[1]);
			} else if (!strcmp(argv[0], "id_max") && argv[1][0] != '\0') {
				server.table_conf[server.tablenum].id_max = atoi(argv[1]);
			} else if (!strcmp(argv[0], "id_increment") && argv[1][0] != '\0') {
				server.table_conf[server.tablenum].id_increment = atoi(argv[1]);
			}
        } else {
            err = "Bad directive or wrong number of arguments"; goto loaderr;
        }
    }
    fclose(fp);
    return;

loaderr:
    fprintf(stderr, "\n*** FATAL CONFIG FILE ERROR ***\n");
    fprintf(stderr, "Reading the configuration file, at line %d\n", linenum);
    fprintf(stderr, ">>> '%s'\n", line);
    fprintf(stderr, "%s\n", err);
    exit(1);
}

int put_id_table(struct id_table_conf tc) {
	struct id_allocator *node, *t;
	char table_file[1024];
	char id_current[16] = {0};
	FILE *fp = NULL;

	snprintf(table_file, sizeof(table_file), "%s/%s.tb", server.datadir, tc.table_name);
	if ((fp = fopen(table_file, "r")) == NULL) {
		write_log(WARNING_LOG, "open table failed: %s", strerror(errno));
		return 1;
	} else {
		write_log(WARNING_LOG, "table file:[%s] open succ", &table_file);
		if (fgets(id_current, 16, fp) == NULL) {
			write_log(WARNING_LOG, "read table failed: %s", strerror(errno));
			fclose(fp);
			return 2;
		}
		fclose(fp);
		write_log(WARNING_LOG, "read table succ, data=[%d]", atoi(id_current));
	}
	
	node = (struct id_allocator *)malloc(sizeof(struct id_allocator));
	node->id_min = tc.id_min;
	node->id_max = tc.id_max;
	node->id_increment = (tc.id_increment <= 0) ? 1 : tc.id_increment;
	node->id_current = atoi(id_current);
	node->table_status = 0;
	node->table_name = strdup(tc.table_name);
	node->table_file = strdup(table_file);
	node->table_fp = NULL;
	node->next = NULL;

	if (id_head == NULL) {
		id_head = node;
	} else {
		t = id_head;
		while(t->next != NULL) {
			t = t->next;
		}
		t->next = node;
	}
	return 0;
}

struct id_allocator * get_id_table(struct id_allocator *head, char *table_name)
{
	struct id_allocator *p;
	p = head;
	while (p && strcmp(p->table_name, table_name) != 0) {
		p = p->next;
	}
	return p;
}

int load_id_allocator() {
	int i, ret, num = 0;
	
	if (server.tablenum <= 0) {
		write_log(WARNING_LOG, "load_id_allocator failed, server.tablenum = %d", server.tablenum);
		return -1;
	}
	
	for (i = 0; i < server.tablenum; i++) {
		ret = put_id_table(server.table_conf[i]);
		if (ret == 0) {
			num++;
		}
	}

	if (num == server.tablenum) {
		return 0;
	} else {
		return -2;
	}
}

int handle_id_allocator(char *table_name) {

	struct id_allocator *table;
	table = get_id_table(id_head, table_name);
	if (table) {
		table->id_current += table->id_increment;
		if (table->id_max > 0) {
			table->id_current = (table->id_current > table->id_max) ? table->id_min : table->id_current;
		}
		return table->id_current;
	} else {
		return -1;
	}
}

void print_id_allocator(struct id_allocator *head) {
	struct id_allocator *p;
	p = head;
	do {
		printf("%d %d %d %d %d %s %s\n",p->id_min, p->id_max, p->id_increment, p->id_current, p->table_status, p->table_name, p->table_file);
		p = p->next;
	} while(p != NULL);
}

void init_network() {

    signal(SIGHUP, SIG_IGN);
    signal(SIGPIPE, SIG_IGN);
	
	if ((server.listen_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		write_log(WARNING_LOG, "socket creat fail");
        exit(1);
	}

	setnonblocking(server.listen_fd);
	memset(&server.server_addr, 0, sizeof(server.server_addr));
	server.server_addr.sin_family = AF_INET;
	server.server_addr.sin_addr.s_addr = inet_addr(server.bindaddr); 
	server.server_addr.sin_port = htons(server.port);

	setsockopt(server.listen_fd, SOL_SOCKET, SO_REUSEADDR, &server.sockopt, sizeof(server.sockopt));
    if (bind(server.listen_fd, (struct sockaddr *) &server.server_addr, sizeof(server.server_addr)) == -1) {
		write_log(WARNING_LOG, "socket bind fail");
		close(server.listen_fd);
		exit(1);
    }

    if (listen(server.listen_fd, BACKLOG) == -1) {
		write_log(WARNING_LOG, "listen client conncect fail");
		close(server.listen_fd);
		exit(1);
	}
	write_log(TRACE_LOG, "network listen on %s:%d", server.bindaddr, server.port);
}

void epoll_loop() {
	int i, nfds, ret;

	server.epoll_fd = epoll_create(MAX_EPOLL_SIZE);
	if (server.epoll_fd < 0) {
		write_log(WARNING_LOG, "epoll_create() failed: %s", strerror(errno));
		close(server.epoll_fd);
		close(server.listen_fd);
		exit(1);
	}
	
	server.ev.data.fd = server.listen_fd;
    server.ev.events = EPOLLIN;
    if (epoll_ctl(server.epoll_fd, EPOLL_CTL_ADD, server.listen_fd, &server.ev) < 0) {
		write_log(WARNING_LOG, "epoll set insertion error: fd=%d", server.listen_fd);
		exit(1);
    }
	write_log(TRACE_LOG, "epoll_create success, listen fd=%d", server.listen_fd);

	while(1) {
		nfds = epoll_wait(server.epoll_fd, server.events, MAX_EPOLL_SIZE, -1);
		if (nfds == -1) {
			write_log(WARNING_LOG, "epoll_wait() failed: %s", strerror(errno));
			break;
        }
		else if(nfds == 0) {
			write_log(WARNING_LOG, "epoll_wait() timeout ...");
			continue;
		}
		
        for (i = 0; i < nfds; i++) {
			if(server.events[i].data.fd == server.listen_fd) {
                while ((server.client_fd = accept(server.listen_fd, (struct sockaddr *) &server.client_addr, &server.sockaddr_len)) > 0) {
					//write_log(TRACE_LOG, "accapt a connection #%d from %s:%d", server.client_fd, inet_ntoa(server.client_addr.sin_addr), ntohs(server.client_addr.sin_port));
					setnonblocking(server.client_fd);
					server.ev.data.fd = server.client_fd;
					server.ev.events = EPOLLIN|EPOLLET;
					if (epoll_ctl(server.epoll_fd, EPOLL_CTL_ADD, server.client_fd, &server.ev) < 0) {
						write_log(WARNING_LOG, "socket '%d' add epoll fail", server.client_fd);
						close(server.client_fd);
						exit(1);
					}
				}
            } else if (server.events[i].events & EPOLLIN) {
				if ((server.client_sockfd = server.events[i].data.fd) < 0) {
					continue;
				}

				memset(server.recv_buf, 0, sizeof(server.recv_buf));
				server.buf_len = read(server.client_sockfd, server.recv_buf, sizeof(server.recv_buf) - 1);
				if (server.buf_len < 0) {
					write_log(WARNING_LOG, "read() socket #%d failed: %s", server.client_sockfd, strerror(errno));
				} else if (server.buf_len == 0) {
					//write_log(TRACE_LOG, "on socket #%d read connection closed by peer.", server.client_sockfd);
					
					epoll_ctl(server.epoll_fd, EPOLL_CTL_DEL, server.client_sockfd, &server.ev);
					shutdown(server.client_sockfd, SHUT_RDWR);
					close(server.client_sockfd);
				} else {
					server.recv_buf[server.buf_len] = '\0';
					//write_log(TRACE_LOG, "read %d bytes from socket #%d : string = %s", server.buf_len, server.client_sockfd, server.recv_buf);
					
					ret = handle_id_allocator(server.recv_buf);
					if(ret <= 0) {
						write_log(WARNING_LOG, "handle table failed: %d", ret);
					}

					bzero(server.send_buf, sizeof(server.send_buf));
					sprintf(server.send_buf, "%d", ret);
					server.buf_len = write(server.client_sockfd, server.send_buf, strlen(server.send_buf));
					if(server.buf_len < 0) {
						write_log(WARNING_LOG, "write() socket #%d failed: %s", server.client_sockfd, strerror(errno));
					}
					//server.send_buf[server.buf_len] = '\0';
					//write_log(TRACE_LOG, "send %d bytes to socket #%d : string = %s", server.buf_len, server.client_sockfd, server.send_buf);
				}
			}
		}
	}
	
	close(server.epoll_fd);
	close(server.listen_fd);
}

void create_pidfile(void) {
    FILE *fp = fopen(server.pidfile, "w");
    if (fp) {
        fprintf(fp,"%d\n",(int)getpid());
        fclose(fp);
    }
}

void daemonize(void) {
    int fd;

    if (fork() != 0) exit(0);
    setsid();

    if ((fd = open("/dev/null", O_RDWR, 0)) != -1) {
        dup2(fd, STDIN_FILENO);
        dup2(fd, STDOUT_FILENO);
        dup2(fd, STDERR_FILENO);
        if (fd > STDERR_FILENO) close(fd);
    }
	//umask(0);
}

void usage() {
    fprintf(stderr, "Usage: ./idallocator [/path/to/idallocator.conf]\n");
    fprintf(stderr, "       ./idallocator --help\n");
    exit(1);
}

int main(int argc, char **argv) {

	int table_status = 0;

	init_config();

    if (argc == 2) {
        if (strcmp(argv[1], "--help") == 0) usage();
        load_config(argv[1]);
    } else {
        usage();
    }
	
	if (server.daemonize) daemonize();

	init_network();
	
	table_status = load_id_allocator();
	if (table_status != 0) {
		write_log(WARNING_LOG, "load_id_allocator failed");
		return 1;
	}
	
	if (pthread_create(&write_disk_pid, NULL, (void *)thread_write_disk, NULL) != 0) {
		write_log(WARNING_LOG, "pthread_create() thread failed: %s", strerror(errno));
		return 1;
	}
	if (server.daemonize) create_pidfile();
	epoll_loop();
	return 0;
}

void *thread_write_disk(void * arg) {

	struct id_allocator *p;
	char tmp[16];

	while(1) {
		p = id_head;
		do {
			if (p->table_fp == NULL) {
				if ((p->table_fp = fopen(p->table_file, "r+")) == NULL) {
					write_log(WARNING_LOG, "open table [%s] failed: %s", p->table_name, strerror(errno));
				} else {
					write_log(TRACE_LOG, "open table [%s] succeed", p->table_name);
				}
			}
			sprintf(tmp, "%d", p->id_current);
			fseek(p->table_fp, 0, SEEK_SET);
			fputs(tmp, p->table_fp);
			fflush(p->table_fp);
			
			p = p->next;
		} while(p != NULL);
		usleep(5000);
	}
	return NULL;
}
