#!/bin/sh
##项目启动脚本
set -x

#######################################################################
##    项目目录改为www-data
##

chmod 777 -R $WORKER_DIR && chown www-data:www-data -R $WORKER_DIR

DOT_FILE=".INIT"

##如果不存在，进行初始化
if [ ! -f $DOT_FILE ]; then
#######################################################################
##   创建项目目录 主站目录 日志目录 配置目录 数据目录
##

	cp -a $BACKUP_DIR/* $WORKER_DIR/
	##创建测试脚本
	echo -e "<?php\n phpinfo();\n?>" > $HOST_DIR/index.php
######################################################################
##
## 创建DOT_FILE
	php -r "echo '创建时间' . date('Y-m-d H:i:s');" > $DOT_FILE
fi


########################################################################
##                      启动 php-fpm
##
php-fpm -D -y $CONFIG_DIR/php/php-fpm.conf

#######################################################################
##                      启动memcached

##设置默认端口
if [ -z $MEMCACHED_PORT ]; then
	MEMCACHED_PORT=11211
fi

##默认以root运行
memcached -u root -d -p $MEMCACHED_PORT

#######################################################################
##                       启动idallocator


idallocator $CONFIG_DIR/idallocator/idallocator.conf

#######################################################################
##                       启动sshd
##

/usr/sbin/sshd

#######################################################################
##   					启动 lighttpd
##

lighttpd   -f $CONFIG_DIR/lighttpd/lighttpd.conf

#######################################################################
##   					启动 lighttpd
##

if [ -z $SAMBA_USER ];then
    SAMBA_USER=devbox
fi

if [ -z $SAMBA_PASSWORD ];then
    SAMBA_PASSWORD=devbox
fi

## 添加用户
addgroup -g 1000 devbox
adduser -D -H -G devbox -s /bin/false -u 1000 $SAMBA_USER
echo -e "$SAMBA_PASSWORD\n$SAMBA_PASSWORD" | smbpasswd -a -s -c $CONFIG_DIR/samba/smb.conf  $SAMBA_USER

smbd  -s $CONFIG_DIR/samba/smb.conf

nmbd  -s $CONFIG_DIR/samba/smb.conf


######################################################################
##
##           监控lighttpd access.log文件

sleep 5s;

tail -f $LOG_DIR/lighttpd/access.log







