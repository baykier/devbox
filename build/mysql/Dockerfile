FROM alpine:3.4

MAINTAINER baykier <1035666345@qq.com>

############################################################
##
##                 安装gosu 解决权限问题

ENV GOSU_VERSION 1.10
RUN set -x \
    && apk add --no-cache --virtual .gosu-deps \
        dpkg \
        gnupg \
        openssl \
    && dpkgArch="$(dpkg --print-architecture | awk -F- '{ print $NF }')" \
    && wget -O /usr/local/bin/gosu "https://github.com/tianon/gosu/releases/download/$GOSU_VERSION/gosu-$dpkgArch" \
    && wget -O /usr/local/bin/gosu.asc "https://github.com/tianon/gosu/releases/download/$GOSU_VERSION/gosu-$dpkgArch.asc" \
    && export GNUPGHOME="$(mktemp -d)" \
    && gpg --keyserver ha.pool.sks-keyservers.net --recv-keys B42F6819007F00F88E364FD4036A9C25BF357DD4 \
    && gpg --batch --verify /usr/local/bin/gosu.asc /usr/local/bin/gosu \
    && rm -r "$GNUPGHOME" /usr/local/bin/gosu.asc \
    && chmod +x /usr/local/bin/gosu \
    && gosu nobody true \
    && apk del .gosu-deps

#############################################################
##
##                  安装 mysql-server

RUN apk add --no-cache mysql mysql-client

##############################################################
##
##                  配置备份与项目目录设置

ENV WORK_DIR "/project"
ENV BACKUP "/backup"

ENV DB_ROOT_PASSWORD "devbox"
ENV DB_USER "devbox"
ENV DB_PASSWORD "devbox"
ENV DB_DATABASE "devbox"

COPY ./init.sh /usr/bin/
COPY ./my.cnf /etc/mysql/

RUN set -ex \
    && chmod 777 /usr/bin/init.sh \
    && chown root:root /usr/bin/init.sh \
    && chmod 644 /etc/mysql/my.cnf \
    && chown root:root /etc/mysql/my.cnf \
    && mkdir -p $WORK_DIR $BACKUP/mysql \
    && cp -a /etc/mysql/* $BACKUP/mysql/ \
    ##创建/run/mysqld socket 目录
    && mkdir -p /run/mysqld 

VOLUME ["$WORK_DIR","/etc/mysql","/var/lib/mysql"]

EXPOSE 3306

ENTRYPOINT /usr/bin/init.sh

