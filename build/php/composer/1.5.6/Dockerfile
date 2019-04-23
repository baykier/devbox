#######################################################################################################################
##                                       Dockerfile for composer
##

FROM baykier/php:7.2-cli

MAINTAINER baykier 1035666345@qq.com

## 安装composer
## homepage https://getcomposer.org/
ENV COMPOSER_VERSION 1.5.6
RUN set -ex \
    ## 安装git
    && apk add --no-cache git \
    && wget -O /usr/bin/composer-${COMPOSER_VERSION}.phar https://getcomposer.org/download/${COMPOSER_VERSION}/composer.phar \
    && chmod +x /usr/bin/composer-${COMPOSER_VERSION}.phar \
    && ln -s /usr/bin/composer-${COMPOSER_VERSION}.phar /usr/bin/composer \
    && sync \
    && composer --version \
    ## 设置composer 国内镜像
    ## refer https://pkg.phpcomposer.com/
    && composer config -g repo.packagist composer https://packagist.phpcomposer.com


## 启动脚本
COPY ./docker-composer-entrypoint /usr/bin/

RUN set -ex \
    && chmod +x /usr/bin/docker-composer-entrypoint \
    && mkdir -p /var/composer

## 挂载目录

WORKDIR /var/composer

ENTRYPOINT ["docker-composer-entrypoint"]

CMD ["composer","list"]