#######################################################################################################################
##                                       Dockerfile for composer
##

FROM baykier/php:7.2-cli

MAINTAINER baykier 1035666345@qq.com

## 安装phpunit
## homepage https://phar.phpunit.de/

ENV PHPUNIT_VERSION "7.0.0"

COPY ./docker-phpunit-entrypoint /usr/bin/

RUN set -ex \
    && wget -O /usr/bin/phpunit https://phar.phpunit.de/phpunit-${PHPUNIT_VERSION}.phar \
    && chmod +x /usr/bin/phpunit \
    && chmod +x /usr/bin/docker-phpunit-entrypoint

## 启动脚本

ENTRYPOINT ["docker-phpunit-entrypoint"]

CMD ["phpunit"]