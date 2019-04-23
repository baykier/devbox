#######################################################################################################################
##
##                                              Dockerfile for baykier/nodejs

FROM node:9.4.0-alpine

MAINTAINER baykier 1035666345@qq.com

## alpine version env
ENV ALPINE_VERSION v3.6

## 安装淘宝镜像
RUN set -ex \
    ## add bower gulp cnpm
    && npm install -g cnpm --registry=https://registry.npm.taobao.org \
    && npm install -g bower --registry=https://registry.npm.taobao.org \
    && npm install -g gulp --registry=https://registry.npm.taobao.org  \
    ## fix bugs for https://stackoverflow.com/questions/29368205/cant-find-python-executable-npm-install-hangs
    && echo https://mirrors.aliyun.com/alpine/${ALPINE_VERSION}/main > /etc/apk/repositories \
    && echo https://mirrors.aliyun.com/alpine/${ALPINE_VERSION}/community >> /etc/apk/repositories \
    && apk add --no-cache python2  \
                              make \
                              gcc  \
                              g++ \
                              git  

CMD npm