########################################################################################################################
##                                      Dockerfile for baykier/php:7.2-dev
##

FROM baykier/php:7.2-fpm

MAINTAINER baykier 1035666345@qq.com

## 安装 gosu 解决权限问题仅用于开发环境
## homepage https://github.com/tianon/gosu

ENV GOSU_VERSION 1.10
RUN set -x \
	&& apk add --no-cache --virtual .gosu-deps \
		                                dpkg \
		                                gnupg \
		                               # openssl1.0 \
	&& dpkgArch="$(dpkg --print-architecture | awk -F- '{ print $NF }')"  \
	&& wget -O /usr/local/bin/gosu "https://github.com/tianon/gosu/releases/download/$GOSU_VERSION/gosu-$dpkgArch"  \
	&& wget -O /usr/local/bin/gosu.asc "https://github.com/tianon/gosu/releases/download/$GOSU_VERSION/gosu-$dpkgArch.asc" \
	&& chmod +x /usr/local/bin/gosu \
    # verify that the binary works
	&& gosu nobody true \
	&& apk del .gosu-deps

## git
RUN set -ex \
    && apk update \
    && apk add ca-certificates git 

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
ENV MONGODB_VERSION 1.5.2
RUN set -x \
    && apk add --no-cache --virtual .phpize-deps \
                                    ${PHPIZE_DEPS} \
    && apk add --no-cache openssl1.0-dev \
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

## 安装tideways
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

# mcrypt

RUN set -ex \
    


## 配置文件
COPY ./php.ini-dev ${PHP_INI_DIR}/php.ini
COPY ./php-ext.conf.d/* ${PHP_INI_DIR}/conf.d/
COPY ./php-fpm.d/* ${PHP_INI_DIR}/php-fpm.d/
COPY ./php-fpm.conf /usr/local/etc/php-fpm.conf

## 设置工作目录
ENV WORKDIR_SPACE /project

RUN set -ex \
    && mkdir -p ${WORKDIR_SPACE} /var/log/php  


WORKDIR ${WORKDIR_SPACE}

VOLUME ${WORKDIR_SPACE}

EXPOSE 9000

ENTRYPOINT gosu root php-fpm
