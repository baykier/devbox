##############################################################################################################
##                                          MongoDB 镜像包
##                           参考 https://github.com/Leafney/alpine-mongo

FROM alpine:3.9

MAINTAINER baykier 1035666345@qq.com

ENV MONGODB_VERSION  4.0.5-r0

ARG DEVBOX_UID=900
ARG DEVBOX_GID=900

RUN set -x  \  
    && apk add --no-cache   mongodb=${MONGODB_VERSION} \
                            mongodb-tools=${MONGODB_VERSION}  \
    && mkdir -p /data/db  \
    && mkdir -p /data/conf  \
    && mkdir -p /var/log/mongo  \
    && rm -rf /var/cache/apk/*   \
    && apk --no-cache add shadow \
    && usermod -u ${DEVBOX_UID} mongodb  \
    && groupmod -g ${DEVBOX_GID} mongodb


## 负责配置文件
COPY ./mongod.conf /data/conf/mongod.conf

RUN set -x  \
    && chown -R mongodb:mongodb /data  \
    && chmod 644 /data/conf/mongod.conf

VOLUME ["/data/db","/var/log/mongo"]

EXPOSE 27017

CMD ["/usr/bin/mongod","-f","/data/conf/mongod.conf"]






