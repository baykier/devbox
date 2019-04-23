#########################################################################################################
##                                       Dockerfile for php-fpm 5.6 wl
##   

FROM baykier/php:5.6-fpm

MAINTAINER baykier 1035666345@qq.com


## 安装gosu 解决权限问题
ENV GOSU_VERSION 1.10
RUN set -x \
	&& apk add --no-cache --virtual .gosu-deps \
		                                dpkg \
		                                gnupg \
		                                openssl \
	&& dpkgArch="$(dpkg --print-architecture | awk -F- '{ print $NF }')"  \
	&& wget -O /usr/local/bin/gosu "https://github.com/tianon/gosu/releases/download/$GOSU_VERSION/gosu-$dpkgArch"  \
	&& wget -O /usr/local/bin/gosu.asc "https://github.com/tianon/gosu/releases/download/$GOSU_VERSION/gosu-$dpkgArch.asc" \
	&& chmod +x /usr/local/bin/gosu \
    # verify that the binary works
	&& gosu nobody true \
	&& apk del .gosu-deps

## PHP 扩展
## 安装 memcached
ENV MEMCACHED_VERSION 2.2.0
RUN set -x \
    && apk add --no-cache --virtual .phpize-deps  \
                                    ${PHPIZE_DEPS}\
    && apk add --no-cache libmemcached-dev  \
                          zlib-dev \
                          cyrus-sasl-dev \
    && wget -O /tmp/memcached-${MEMCACHED_VERSION}.tgz http://pecl.php.net/get/memcached-${MEMCACHED_VERSION}.tgz \
    && pecl install /tmp/memcached-${MEMCACHED_VERSION}.tgz \
    && docker-php-ext-enable memcached \
    && docker-php-source delete \
    && rm -fr /tmp/memcached-${MEMCACHED_VERSION}.tgz \
    && apk del .phpize-deps

## 安装 GD
RUN set -x \
    && apk add --no-cache --virtual .phpize-deps  \
                                    ${PHPIZE_DEPS}\
    && apk add 	--no-cache \
           			freetype \
           			libpng \
           			libjpeg-turbo \
           			freetype-dev \
           			libpng-dev \
           			libjpeg-turbo-dev\
    && docker-php-ext-configure gd \
           			--with-gd \
           			--with-freetype-dir=/usr/include/ \
           			--with-png-dir=/usr/include/ \
           			--with-jpeg-dir=/usr/include/ \
    && docker-php-ext-install gd \
    && docker-php-source delete \
    && apk del 	.phpize-deps

## 安装pdo_mysql bcmath
RUN set -x \
    && docker-php-ext-install   pdo_mysql \
                                bcmath \
    && docker-php-source delete

## 安装redis
ENV REDIS_VERSION 2.2.8
RUN set -x \
    && apk add --no-cache --virtual .phpize-deps  \
                                    ${PHPIZE_DEPS}\
    && wget -O /tmp/redis-${REDIS_VERSION}.tgz http://pecl.php.net/get/redis-${REDIS_VERSION}.tgz \
    && pecl install /tmp/redis-${REDIS_VERSION}.tgz \
    && docker-php-ext-enable redis \
    && docker-php-source delete \
    && rm -fr /tmp/redis-${REDIS_VERSION}.tgz \
    && apk del 	.phpize-deps

## 安装mongo扩展
ENV MONGO_VERSION 1.6.16
RUN set -x \
    && apk add --no-cache --virtual .phpize-deps  \
                                    ${PHPIZE_DEPS}\
    && apk add --no-cache libressl-dev \
    && wget -O /tmp/mongo-${MONGO_VERSION}.tgz    http://pecl.php.net/get/mongo-${MONGO_VERSION}.tgz \
    && pecl install /tmp/mongo-${MONGO_VERSION}.tgz \
    && docker-php-ext-enable mongo \
    && docker-php-source delete \
    && rm -fr /tmp/mongo-${MONGO_VERSION}.tgz \
    && apk del 	.phpize-deps

## 安装zip扩展
ENV ZIP_VERSION 1.15.2
RUN set -x \
    && apk add --no-cache --virtual .phpize-deps  \
                                    ${PHPIZE_DEPS}\
    && apk add --no-cache              libzip-dev \
    && wget -O /tmp/zip-${ZIP_VERSION}.tgz    http://pecl.php.net/get/zip-${ZIP_VERSION}.tgz \
    && pecl install /tmp/zip-${ZIP_VERSION}.tgz \
    && docker-php-ext-enable zip \
    && docker-php-source delete \
    && rm -fr /tmp/zip-${ZIP_VERSION}.tgz \
    && apk del 	.phpize-deps

## tideways 扩展
## refer https://github.com/tideways/php-xhprof-extension
ENV TIDEWAYS_VERSION "4.1.6"

RUN set -ex \
    ## 安装tideways-php-ext
    && apk add --no-cache --virtual .phpize-deps    \
                                    ${PHPIZE_DEPS}  \
    && wget -O /tmp/tideways-php-${TIDEWAYS_VERSION}.tar.gz https://github.com/tideways/php-xhprof-extension/archive/v${TIDEWAYS_VERSION}.tar.gz \
    && mkdir -p /tmp/tideways-php-${TIDEWAYS_VERSION}-src \
    && tar -xf /tmp/tideways-php-${TIDEWAYS_VERSION}.tar.gz -C /tmp/tideways-php-${TIDEWAYS_VERSION}-src --strip-components=1 \
    && ( \
        cd /tmp/tideways-php-${TIDEWAYS_VERSION}-src \
        && phpize \
        &&  ./configure \
        && make install \
    ) \
    && docker-php-ext-enable tideways \
    && rm -fr /tmp/* \
    && apk del .phpize-deps

## mcrypt

RUN set -ex \
    && apk add --no-cache libmcrypt-dev \
    && docker-php-ext-install mcrypt \
    && docker-php-source delete

##工作目录
ENV WORKSPACE "/project"

COPY ./php.ini $PHP_INI_DIR/
COPY ./php-fpm.conf /usr/local/etc/php-fpm.conf
COPY ./php-fpm.d/* /usr/local/etc/php-fpm.d/
COPY ./tideways.ini ${PHP_INI_DIR}/conf.d/

RUN set -x \
	&& mkdir -p ${WORKSPACE} /var/log/php \
	&& chmod 644 -R /usr/local/etc


VOLUME ["${WORKSPACE}"]

##指定工作目录

WORKDIR ${WORKSPACE}

EXPOSE 9000

ENTRYPOINT gosu root php-fpm



