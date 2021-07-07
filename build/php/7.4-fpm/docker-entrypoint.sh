#!/bin/sh

set -ex

# 设置uid
if [[ -n "$DEVBOX_UID" && -n "$DEVBOX_GID" ]];then
    echo "reset www-data uid && gid \n"
    usermod -u $DEVBOX_UID www-data
    groupmod -g $DEVBOX_GID www-data
fi

## bootstrap fpm

php-fpm