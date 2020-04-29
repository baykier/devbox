########################################################################################################################
##                                      Dockerfile for baykier/php:7.4-fpm
##

FROM php:7.4-fpm-alpine3.11

MAINTAINER baykier 1035666345@qq.com

## 安装 gosu 解决权限问题仅用于开发环境
## homepage https://github.com/tianon/gosu

ENV GOSU_VERSION 1.11
RUN set -eux; \
	\
	apk add --no-cache --update --virtual .gosu-deps \
		dpkg \
		gnupg \
	; \
	\
    apk update; \
	dpkgArch="$(dpkg --print-architecture | awk -F- '{ print $NF }')"; \
	wget -O /usr/local/bin/gosu "https://github.com/tianon/gosu/releases/download/$GOSU_VERSION/gosu-$dpkgArch"; \
	wget -O /usr/local/bin/gosu.asc "https://github.com/tianon/gosu/releases/download/$GOSU_VERSION/gosu-$dpkgArch.asc"; \
	\
# verify the signature
	export GNUPGHOME="$(mktemp -d)"; \
# for flaky keyservers, consider https://github.com/tianon/pgp-happy-eyeballs, ala https://github.com/docker-library/php/pull/666
	gpg --batch --keyserver ha.pool.sks-keyservers.net --recv-keys B42F6819007F00F88E364FD4036A9C25BF357DD4; \
	gpg --batch --verify /usr/local/bin/gosu.asc /usr/local/bin/gosu; \
	command -v gpgconf && gpgconf --kill all || :; \
	rm -rf "$GNUPGHOME" /usr/local/bin/gosu.asc; \
	\
# clean up fetch dependencies
	apk del --no-network .gosu-deps; \
	\
	chmod +x /usr/local/bin/gosu; \
# verify that the binary works
	gosu --version; \
	gosu nobody true


## git
RUN set -ex \
    && apk update \
    ## 使用阿里源
    && sed -i 's/dl-cdn.alpinelinux.org/mirrors.aliyun.com/g' /etc/apk/repositories \
    && apk add ca-certificates git

## 安装php扩展
## php:7.4 docker默认安装的扩展
## { Core ctype curl date dom fileinfo filter ftp hash iconv json libxml mbstring mysqlnd openssl pcre PDO
## pdo_sqlite Phar posix readline Reflection session SimpleXML SPL sqlite3 standard tokenizer xml xmlreader
## xmlwriter zlib }
##

## 安装mysql bcmatch exif 扩展
RUN set -x \
    && docker-php-source extract \
    && docker-php-ext-install   pdo_mysql \
                                bcmath \
                                exif  \
                                pcntl \
    && docker-php-source delete

## 安装zip扩展
RUN set -x \
    && apk add --no-cache libzip-dev \
    && docker-php-ext-configure zip \
    && docker-php-ext-install zip \
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
    ## https://github.com/docker-library/php/issues/912
    && docker-php-ext-configure     gd \
               			           --with-freetype \
               			           --with-jpeg \
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


## imap 扩展
RUN set -x \
	&& apk add --no-cache  imap-dev \
							openssl-dev \
	&& apk add --no-cache --virtual .phpize-deps \
                                    ${PHPIZE_DEPS} \

    ## 修复openssl not found 错误  https://github.com/docker-library/php/issues/891#issuecomment-531023107
	&& PHP_OPENSSL=yes docker-php-ext-configure imap --with-imap --with-imap-ssl \
	&& docker-php-ext-install imap \
	&& apk del .phpize-deps \
    && docker-php-source delete

## mongodb 扩展
ENV MONGODB_VERSION 1.6.1
RUN set -x \
    && apk add --no-cache --virtual .phpize-deps \
                                    ${PHPIZE_DEPS} \
    && wget -O /tmp/mongodb-${MONGODB_VERSION}.tgz  https://pecl.php.net/get/mongodb-${MONGODB_VERSION}.tgz \
    && pecl install /tmp/mongodb-${MONGODB_VERSION}.tgz \
    && docker-php-ext-enable mongodb \
    && apk del .phpize-deps \
    && rm -fr /tmp/mongodb-${MONGODB_VERSION}.tgz

## tideways 扩展
## refer https://github.com/tideways/php-xhprof-extension
ENV TIDEWAYS_VERSION "5.0.2"

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
    && docker-php-ext-enable tideways_xhprof \
    && rm -fr /tmp/* \
    && apk del .phpize-deps

## composer

ENV COMPOSER_VERSION 1.9.2

RUN set -x \
    && wget -O /usr/bin/composer-${COMPOSER_VERSION}.phar  https://getcomposer.org/download/${COMPOSER_VERSION}/composer.phar \
    && chmod +x /usr/bin/composer-${COMPOSER_VERSION}.phar \
    && ln -s  /usr/bin/composer-${COMPOSER_VERSION}.phar /usr/bin/composer

## 设置工作目录
ENV WORKDIR_SPACE /project

RUN set -ex \
    && mkdir -p ${WORKDIR_SPACE} /var/log/php

#配置文件
COPY ./php.ini-dev ${PHP_INI_DIR}/php.ini
COPY ./php-fpm.d/* ${PHP_INI_DIR}/php-fpm.d/
COPY ./php-fpm.conf /usr/local/etc/php-fpm.conf
# COPY conf.d/*.ini ${PHP_INI_DIR}/conf.d/



WORKDIR ${WORKDIR_SPACE}

VOLUME ${WORKDIR_SPACE}

EXPOSE 9000

ENTRYPOINT gosu root php-fpm


