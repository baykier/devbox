#######################################################################################################################
##                                       Dockerfile for composer
##

FROM baykier/php:7.2-cli

MAINTAINER baykier 1035666345@qq.com

## 安装psysh
## homepage https://github.com/bobthecow/psysh

## 手册语言
ENV MANUAL_LOCAL "zh"

COPY ./docker-psysh-entrypoint /usr/bin/

RUN set -ex \
    && mkdir -p /usr/local/share/psysh \
    && wget -O /usr/bin/psysh https://git.io/psysh \
    && ( \
        cd /usr/local/share/psysh \
        && wget http://psysh.org/manual/${MANUAL_LOCAL}/php_manual.sqlite \
     ) \
    && chmod +x /usr/bin/psysh \
    && chmod +x /usr/bin/docker-psysh-entrypoint



## 启动脚本

ENTRYPOINT ["docker-psysh-entrypoint"]

CMD ["psysh"]