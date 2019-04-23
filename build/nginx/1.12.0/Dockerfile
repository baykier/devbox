######################################################################################################################
##
##                                   基于docker-library/nginx官方镜像
##                                https://hub.docker.com/r/library/nginx
##                               https://github.com/nginxinc/docker-nginx

FROM nginx:1.12-alpine

ENV ALPINE_VERSION v3.5

MAINTAINER baykier <1035666345@qq.com>

## 更新apk源为阿里源 安装 shadow (usermod)
## http://mirrors.aliyun.com/

RUN set -x \
    && echo https://mirrors.aliyun.com/alpine/${ALPINE_VERSION}/main > /etc/apk/repositories \
    && echo https://mirrors.aliyun.com/alpine/${ALPINE_VERSION}/community >> /etc/apk/repositories \
    && apk --no-cache add shadow

## 指定构建参数

ARG DEVBOX_UID=1000
ARG DEVBOX_GID=1000

## 将nginx uid gid 更新为开发环境用户ID
RUN set -x \
    && usermod -u ${DEVBOX_UID} nginx \
    && groupmod -g ${DEVBOX_GID} nginx

## 自定义配置

RUN set -x \
    && rm -fr /etc/nginx/conf.d \
    && rm -f /etc/nginx/nginx.conf

COPY ./nginx.conf /etc/nginx/nginx.conf
COPY ./conf.d /etc/nginx/conf.d

RUN set -x \
    && chmod 644 /etc/nginx/nginx.conf \
    && chmod 644 /etc/nginx/conf.d/*

## 指定端口

EXPOSE 80 443

## 默认命令

STOPSIGNAL SIGTERM

CMD ["nginx", "-g", "daemon off;"]




