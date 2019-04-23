########################################################################################################################
##                                   Dockerfile for swoole-distributed
##

FROM baykier/php:7.2-cli

MAINTAINER baykier 1035666345@qq.com

## 安装inotify 扩展
ENV INOTIFY_VERSION 2.0.0
RUN set -x \
    && apk add --no-cache --virtual .phpize-deps \
                                    ${PHPIZE_DEPS} \
    && wget -O /tmp/inotify-${INOTIFY_VERSION}.tgz  http://pecl.php.net/get/inotify-${INOTIFY_VERSION}.tgz \
    && pecl install /tmp/inotify-${INOTIFY_VERSION}.tgz \
    && docker-php-ext-enable inotify \
    && rm -fr /tmp/inotify-${INOTIFY_VERSION}.tgz \
    && apk del .phpize-deps

## 安装zip扩展
RUN set -x \
    && apk add --no-cache zlib-dev \
    && docker-php-ext-configure zip \
    && docker-php-ext-install zip \
    && docker-php-source delete

## 安装 opcache
RUN set -x \
    && docker-php-ext-install opcache \
    && docker-php-source delete

## 安装bcmatch
RUN set -x \
    && docker-php-ext-install bcmath \
    && docker-php-source delete

## 安装 pdo_mysql
RUN set -x \
    && docker-php-ext-install pdo_mysql \
    && docker-php-source delete

## 安装swoole
ENV SWOOLE_VERSION 1.9.23
RUN set -x \
    && apk add --no-cache --virtual .phpize-deps \
                                    ${PHPIZE_DEPS} \
    && apk add --no-cache   hiredis-dev \
                            openssl-dev \
                            nghttp2-dev \
                            libaio-dev \
                            linux-headers \
    && wget -O /tmp/swoole-${SWOOLE_VERSION}.tgz  http://pecl.php.net/get/swoole-${SWOOLE_VERSION}.tgz \
    && mkdir -p /tmp/swoole-${SWOOLE_VERSION}-src \
    && tar -xf  /tmp/swoole-${SWOOLE_VERSION}.tgz -C /tmp/swoole-${SWOOLE_VERSION}-src --strip-components=1 \
    && ( \
        cd /tmp/swoole-${SWOOLE_VERSION}-src \
        && phpize \
        && ./configure  --enable-http2 \
                        --enable-openssl \
                        --enable-swoole-debug  \
                        --enable-async-redis \
                        --enable-sockets \
        && make -j$(nproc) \
        && make install \
    ) \
    && docker-php-ext-enable swoole \
    && rm -fr /tmp/* \
    && apk del .phpize-deps

## 安装 igbinary
ENV IGBINARY_VERSION 2.0.5
RUN set -x \
    && apk add --no-cache --virtual .phpize-deps \
                                    ${PHPIZE_DEPS} \
    && wget -O /tmp/igbinary-${IGBINARY_VERSION}.tgz http://pecl.php.net/get/igbinary-${IGBINARY_VERSION}.tgz \
    && pecl install /tmp/igbinary-${IGBINARY_VERSION}.tgz \
    && docker-php-ext-enable igbinary \
    && rm -fr /tmp/igbinary-${IGBINARY_VERSION}.tgz \
    && apk del .phpize-deps

## 安装php redis
ENV PHPREDIS_VERSION 3.1.5
RUN set -x \
    && apk add --no-cache --virtual .phpize-deps \
                                    ${PHPIZE_DEPS} \
    && wget -O /tmp/redis-${PHPREDIS_VERSION}.tgz  http://pecl.php.net/get/redis-${PHPREDIS_VERSION}.tgz \
    && mkdir -p /tmp/redis-${PHPREDIS_VERSION}-src \
    && tar -xf  /tmp/redis-${PHPREDIS_VERSION}.tgz -C /tmp/redis-${PHPREDIS_VERSION}-src --strip-components=1 \
    && ( \
            cd /tmp/redis-${PHPREDIS_VERSION}-src \
            && phpize \
            && ./configure --enable-redis-igbinary \
            && make -j$(nproc) \
            && make install \
    ) \
    && docker-php-ext-enable redis \
    && rm -fr /tmp/* \
    && apk del .phpize-deps

## 安装 xdebug
ENV XDEBUG_VERSION 2.6.0alpha1
RUN set -ex \
    && apk add --no-cache --virtual .phpize-deps \
                                    ${PHPIZE_DEPS} \
    && wget -O /tmp/${XDEBUG_VERSION}.tgz http://pecl.php.net/get/xdebug-${XDEBUG_VERSION}.tgz \
    && pecl install /tmp/${XDEBUG_VERSION}.tgz \
    && docker-php-ext-enable xdebug \
    && rm -fr /tmp/* \
    && apk del .phpize-deps

## 安装intl扩展
RUN set -x \
    && apk add --no-cache icu-dev \
    && docker-php-ext-configure intl \
    && docker-php-ext-install intl \
    && docker-php-source delete


## 添加php配置
COPY ./php.ini-development ${PHP_INI_DIR}/php.ini
## 添加xdebug.ini
COPY ./xdebug.ini ${PHP_INI_DIR}/conf.d

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

## 安装composer
ENV COMPOSER_VERSION 1.5.6
RUN set -ex \
    && wget -O /usr/bin/composer-${COMPOSER_VERSION}.phar https://getcomposer.org/download/${COMPOSER_VERSION}/composer.phar \
    && chmod +x /usr/bin/composer-${COMPOSER_VERSION}.phar \
    && ln -s /usr/bin/composer-${COMPOSER_VERSION}.phar /usr/bin/composer \
    && sync \
    && composer --version \
    ## 设置composer 国内镜像
    && composer config -g repo.packagist composer https://packagist.phpcomposer.com

ENTRYPOINT tail -f /etc/hosts
