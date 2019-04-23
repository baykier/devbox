########################################################################################################################
##                                      Dockerfile for baykier/php:7.2
##

FROM php:7.2.0-fpm-alpine3.7

MAINTAINER baykier 1035666345@qq.com

## 更新apk源为阿里源 安装 shadow (usermod)
## http://mirrors.aliyun.com/
ENV ALPINE_VERSION v3.7
RUN set -x \
    && echo https://mirrors.aliyun.com/alpine/${ALPINE_VERSION}/main > /etc/apk/repositories \
    && echo https://mirrors.aliyun.com/alpine/${ALPINE_VERSION}/community >> /etc/apk/repositories \
    && apk --no-cache add shadow

## 安装php扩展
## php:7.2 docker默认安装的扩展
## { Core ctype curl date dom fileinfo filter ftp hash iconv json libxml mbstring mysqlnd openssl pcre PDO
## pdo_sqlite Phar posix readline Reflection session SimpleXML SPL sqlite3 standard tokenizer xml xmlreader
## xmlwriter zlib }
##

## 安装mysql 扩展
RUN set -x \
    && docker-php-source extract \
    && docker-php-ext-install pdo_mysql \
    && docker-php-source delete

## 安装bcmath 扩展
RUN set -x \
    && docker-php-source extract \
    && docker-php-ext-install bcmath \
    && docker-php-source delete

## 安装zip扩展
RUN set -x \
    && apk add --no-cache zlib-dev \
    && docker-php-ext-configure zip \
    && docker-php-ext-install zip \
    && docker-php-source delete

## 安装exif扩展
RUN set -x \
    && docker-php-source extract \
    && docker-php-ext-install exif \
    && docker-php-source delete

## 安装intl扩展
RUN set -x \
    && apk add --no-cache icu-dev \
    && docker-php-ext-configure intl \
    && docker-php-ext-install intl \
    && docker-php-source delete

## 安装gd扩展
RUN set -x \
    && apk add 	--no-cache  freetype \
               			    libpng \
               			    libjpeg-turbo \
               			    freetype-dev \
               			    libpng-dev \
               			    libjpeg-turbo-dev \
    && docker-php-ext-configure     gd \
               			            --with-gd \
               			            --with-freetype-dir=/usr/include \
               			            --with-png-dir=/usr/include \
               			            --with-jpeg-dir=/usr/include \
    && docker-php-ext-install gd \
    && docker-php-source delete

## 安装imagick
ENV IMAGICK_VERSION 3.4.3
RUN set -x \
    && apk add --no-cache --virtual .phpize-deps \
                                    ${PHPIZE_DEPS} \
    && apk add --no-cache imagemagick-dev \
    && wget -O /tmp/imagick-${IMAGICK_VERSION}.tgz http://pecl.php.net/get/imagick-${IMAGICK_VERSION}.tgz \
    && pecl install /tmp/imagick-${IMAGICK_VERSION}.tgz \
    && docker-php-ext-enable imagick \
    && apk del .phpize-deps \
    && rm -fr /tmp/imagick-${IMAGICK_VERSION}.tgz

## 安装redis扩展
ENV REDIS_VERSION 3.1.5
RUN set -x \
    && apk add --no-cache --virtual .phpize-deps \
                                    ${PHPIZE_DEPS} \
    && wget -O /tmp/redis-${REDIS_VERSION}.tgz http://pecl.php.net/get/redis-${REDIS_VERSION}.tgz \
    && pecl install /tmp/redis-${REDIS_VERSION}.tgz \
    && docker-php-ext-enable redis \
    && apk del .phpize-deps \
    && rm -fr /tmp/redis-${REDIS_VERSION}.tgz

## 安装mongodb扩展
ENV MONGODB_VERSION 1.3.4
RUN set -x \
    && apk add --no-cache --virtual .phpize-deps \
                                    ${PHPIZE_DEPS} \
    && apk add --no-cache openssl-dev \
    && wget -O /tmp/mongodb-${MONGODB_VERSION}.tgz http://pecl.php.net/get/mongodb-${MONGODB_VERSION}.tgz \
    && pecl install /tmp/mongodb-${MONGODB_VERSION}.tgz \
    && docker-php-ext-enable mongodb \
    && apk del .phpize-deps \
    && rm -fr /tmp/mongodb-${MONGODB_VERSION}.tgz

## 安装memcached扩展
ENV MEMCACHED_VERSION 3.0.4
RUN set -x \
    && apk add --no-cache --virtual .phpize-deps \
                                    ${PHPIZE_DEPS} \
    && apk add --no-cache  	libmemcached-dev \
                            cyrus-sasl-dev \
    && wget -O /tmp/memcached-${MEMCACHED_VERSION}.tgz http://pecl.php.net/get/memcached-${MEMCACHED_VERSION}.tgz \
    && pecl install /tmp/memcached-${MEMCACHED_VERSION}.tgz \
    && docker-php-ext-enable memcached \
    && apk del .phpize-deps \
    && rm -fr /tmp/memcached-${MEMCACHED_VERSION}.tgz

## 将www-data uid gid 更新为开发环境用户ID
ARG DEVBOX_UID=1000
ARG DEVBOX_GID=1000

RUN set -x \
    && mkdir -p /project \
    && chown -R www-data:www-data /project \
    && usermod -u ${DEVBOX_UID} www-data \
    && groupmod -g ${DEVBOX_GID} www-data

EXPOSE 9000

ENTRYPOINT ["docker-php-entrypoint"]

CMD ["php-fpm"]