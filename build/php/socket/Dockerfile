########################################################################################################################
##                                   Dockerfile for amphp/reactphp/workerman
##

FROM baykier/php:7.2-cli

MAINTAINER baykier 1035666345@qq.com

## 安装 libev
## homepage https://bitbucket.org/osmanov/pecl-ev/src
## pecl https://pecl.php.net/package/ev

ENV PHPEV_VERSION 1.0.4
RUN set -ex \
    && apk add --no-cache --virtual .phpize-deps   \
                                    ${PHPIZE_DEPS} \
    && wget -O /tmp/ev-${PHPEV_VERSION}.tgz https://pecl.php.net/get/ev-${PHPEV_VERSION}.tgz  \
    && pecl install /tmp/ev-${PHPEV_VERSION}.tgz \
    && docker-php-ext-enable ev \
    && rm -fr /tmp/* \
    && apk del .phpize-deps

## 安装event
## homepage https://bitbucket.org/osmanov/pecl-event
## pecl https://pecl.php.net/package/event

ENV PHPEVENT_VERSION 2.3.0
RUN set -ex \
    && apk add --no-cache --virtual .phpize-deps   \
                                    ${PHPIZE_DEPS} \
    && apk add --no-cache libevent-dev \
                          openssl-dev \
    && wget -O /tmp/event-${PHPEVENT_VERSION}.tgz https://pecl.php.net/get/event-${PHPEVENT_VERSION}.tgz  \
    && pecl install /tmp/event-${PHPEVENT_VERSION}.tgz \
    && docker-php-ext-enable event \
    && rm -fr /tmp/* \
    && apk del .phpize-deps

## 安装 php-uv
## homepage https://github.com/bwoebi/php-uv

ENV PHPUV_VERSION 0.2.2
RUN set -ex \
    && apk add --no-cache --virtual .phpize-deps   \
                                    ${PHPIZE_DEPS} \
    && apk add --no-cache libuv-dev \
    && wget -O /tmp/php-uv-v${PHPUV_VERSION}.zip  https://github.com/bwoebi/php-uv/archive/v${PHPUV_VERSION}.zip \
    && unzip -d /tmp/ /tmp/php-uv-v${PHPUV_VERSION}.zip \
    && ( \
        cd /tmp/php-uv-${PHPUV_VERSION} \
        && phpize  \
        && ./configure \
        && make \
        && make install \
    ) \
    && docker-php-ext-enable uv \
    && rm -fr /tmp/* \
    && apk del .phpize-deps

## 安装pcntl mysql

RUN set -ex \
    && docker-php-ext-install pcntl     \
                              pdo_mysql \
    && docker-php-source delete


## 安装 xdebug
## homepage https://xdebug.org/

## 添加xdebug.ini
COPY ./config/xdebug.ini ${PHP_INI_DIR}/conf.d

ENV XDEBUG_VERSION 2.6.0alpha1
RUN set -ex \
    && apk add --no-cache --virtual .phpize-deps \
                                    ${PHPIZE_DEPS} \
    && wget -O /tmp/${XDEBUG_VERSION}.tgz http://pecl.php.net/get/xdebug-${XDEBUG_VERSION}.tgz \
    && pecl install /tmp/${XDEBUG_VERSION}.tgz \
    && docker-php-ext-enable xdebug \
    && rm -fr /tmp/* \
    && apk del .phpize-deps

## 安装composer
## homepage https://getcomposer.org/

ENV COMPOSER_VERSION 1.5.6
RUN set -ex \
    && wget -O /usr/bin/composer-${COMPOSER_VERSION}.phar https://getcomposer.org/download/${COMPOSER_VERSION}/composer.phar \
    && chmod +x /usr/bin/composer-${COMPOSER_VERSION}.phar \
    && ln -s /usr/bin/composer-${COMPOSER_VERSION}.phar /usr/bin/composer \
    && sync \
    && composer --version \
    ## 设置composer 国内镜像
    && composer config -g repo.packagist composer https://packagist.phpcomposer.com


## 安装调试工具
## refer https://wiki.swoole.com/wiki/page/487.html
## refer http://wiki.alpinelinux.org/wiki/Configure_Networking
ENV SWOOLE_DEBUG_TOOLS gdb \
                       strace \
                       tcpdump \
                       ## ss
                       iproute2 \
                       ## vmstat
                       procps \
                       ## sar
                       sysstat \
                       ## ping
                       iputils

RUN set -ex \
    && apk add --no-cache --update $SWOOLE_DEBUG_TOOLS

## 添加配置文件
COPY ./config/php.ini-dev ${PHP_INI_DIR}/php.ini

ENTRYPOINT ["docker-php-entrypoint"]

CMD ["php", "-a"]