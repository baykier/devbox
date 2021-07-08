#!/bin/sh

set -ex

# 设置uid
if [[ -n "$DEVBOX_UID" && -n "$DEVBOX_GID" ]];then
    echo "reset nginx uid && gid \n"
    usermod -u $DEVBOX_UID nginx
    groupmod -g $DEVBOX_GID nginx

fi
# boot nginx

nginx -g 'daemon off;'