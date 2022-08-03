##########################################################################################################################
##　　　　　　　　　　　　　　　　　　　　Dockerfile for redis
##

##  参考 https://github.com/docker-library/redis/blob/7900c5d31e0b3a4c463c57a8d69cc497d58fbe70/4.0/alpine/Dockerfile
FROM redis:5.0-alpine

MAINTAINER baykier 1035666345@qq.com

ENV REDIS_VERSION 4.0.6

COPY redis.conf /etc/redis.conf

RUN set -x \
    && mkdir -p /data \
    && chown redis:redis /data 

ENTRYPOINT ["/usr/local/bin/docker-entrypoint.sh"]

EXPOSE 6379

CMD ["redis-server","/etc/redis.conf"]