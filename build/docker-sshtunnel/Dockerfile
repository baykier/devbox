#####################################################################################################
## 
##                                  Dockerfile for sshtunnel

FROM python:3.5-alpine3.8

MAINTAINER baykier 1035666345@qq.com

ENV ALPINE_VERSION v3.8

# 绑定配置
ENV BIND_LOCAL_HOST "0.0.0.0"
ENV BIND_LOCAL_PORT "3306" 
ENV BIND_REMOTE_HOST "0.0.0.0"
ENV BIND_REMOTE_PORT "3306"

# SSH 配置
ENV SSH_USERNAME "sshtunnel"
ENV SSH_PASSPORD "sshtunnel"
ENV SSH_SERVER_HOST "0.0.0.0"
ENV SSH_SERVER_PORT "22"


RUN set -ex \
    && echo https://mirrors.aliyun.com/alpine/${ALPINE_VERSION}/main > /etc/apk/repositories \
    && echo https://mirrors.aliyun.com/alpine/${ALPINE_VERSION}/community >> /etc/apk/repositories \
    && mkdir /app \
    && apk add --no-cache --virtual .build-deps   \
                                    dpkg-dev dpkg \
                                    gcc \
                                    libc-dev \
                                    openssl-dev \
                                    libffi-dev \
                                    python3-dev \
                                    make \
    && pip3 install -i https://pypi.tuna.tsinghua.edu.cn/simple/  \
                                                    sshtunnel \
    && apk del .build-deps 
     

COPY ./docker-sshtunnel.py /app/docker-sshtunnel.py

EXPOSE ${BIND_LOCAL_PORT}


ENTRYPOINT python3 /app/docker-sshtunnel.py





