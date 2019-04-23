####################################################################################################################
##                              Dockerfile for alpine 开发基础镜像

## 基于alpine:3.5
## https://github.com/gliderlabs/docker-alpine

FROM alpine:edge

ENV ALPINE_VERSION edge

MAINTAINER baykier <1035666345@qq.com>

## 更新apk源为阿里源 安装 shadow (usermod)
## http://mirrors.aliyun.com/

RUN set -x \
    && echo https://mirrors.aliyun.com/alpine/${ALPINE_VERSION}/main > /etc/apk/repositories \
    && echo https://mirrors.aliyun.com/alpine/${ALPINE_VERSION}/community >> /etc/apk/repositories \
    && apk --no-cache add shadow

## 安装 gosu 解决权限问题仅用于开发环境
## homepage https://github.com/tianon/gosu

ENV GOSU_VERSION 1.10
RUN set -x \
	&& apk add --no-cache --virtual .gosu-deps \
		                                dpkg \
		                                gnupg \
		                                openssl \
	&& dpkgArch="$(dpkg --print-architecture | awk -F- '{ print $NF }')"  \
	&& wget -O /usr/local/bin/gosu "https://github.com/tianon/gosu/releases/download/$GOSU_VERSION/gosu-$dpkgArch"  \
	&& wget -O /usr/local/bin/gosu.asc "https://github.com/tianon/gosu/releases/download/$GOSU_VERSION/gosu-$dpkgArch.asc" \
	&& chmod +x /usr/local/bin/gosu \
    # verify that the binary works
	&& gosu nobody true \
	&& apk del .gosu-deps