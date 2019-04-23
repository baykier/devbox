####################################################################################################################
##                              Dockerfile for alpine 基础镜像

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



