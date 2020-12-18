########################################################################################################################
##                                      Dockerfile for baykier/php:8.0-fpm
##

FROM php:8.0.0-fpm-alpine3.12

MAINTAINER baykier 1035666345@qq.com


##  git
RUN set -ex \

    # 使用阿里源
    && sed -i 's/dl-cdn.alpinelinux.org/mirrors.aliyun.com/g' /etc/apk/repositories \
    && apk update \
    && apk add ca-certificates git

## 安装 gosu 解决权限问题仅用于开发环境
## homepage https://github.com/tianon/gosu

ENV GOSU_VERSION 1.12
RUN set -eux; \
	\
	apk add --no-cache --virtual .gosu-deps \
		ca-certificates \
		dpkg \
		gnupg \
	; \
	\
	dpkgArch="$(dpkg --print-architecture | awk -F- '{ print $NF }')"; \
	wget -O /usr/local/bin/gosu "https://github.com/tianon/gosu/releases/download/$GOSU_VERSION/gosu-$dpkgArch"; \
	wget -O /usr/local/bin/gosu.asc "https://github.com/tianon/gosu/releases/download/$GOSU_VERSION/gosu-$dpkgArch.asc"; \
	\
# verify the signature
	export GNUPGHOME="$(mktemp -d)"; \
	gpg --batch --keyserver hkps://keys.openpgp.org --recv-keys B42F6819007F00F88E364FD4036A9C25BF357DD4; \
	#gpg --batch --verify /usr/local/bin/gosu.asc /usr/local/bin/gosu; \
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

## 安装imagick 8.0 暂未支持 https://github.com/Imagick/imagick/issues/271
# ENV IMAGICK_VERSION 3.4.4
# RUN set -x \
#     && apk add --no-cache --virtual .phpize-deps \
#                                     ${PHPIZE_DEPS} \
#     && apk add --no-cache imagemagick-dev \
#     && wget -O /tmp/imagick-${IMAGICK_VERSION}.tgz http://pecl.php.net/get/imagick-${IMAGICK_VERSION}.tgz \
#     && pecl install /tmp/imagick-${IMAGICK_VERSION}.tgz \
#     && docker-php-ext-enable imagick \
#     && apk del .phpize-deps \
#     && rm -fr /tmp/imagick-${IMAGICK_VERSION}.tgz

## 安装redis扩展
ENV REDIS_VERSION 5.3.2
RUN set -x \
    && apk add --no-cache --virtual .phpize-deps \
                                    ${PHPIZE_DEPS} \
    && wget -O /tmp/redis-${REDIS_VERSION}.tgz http://pecl.php.net/get/redis-${REDIS_VERSION}.tgz \
    && pecl install /tmp/redis-${REDIS_VERSION}.tgz \
    && docker-php-ext-enable redis \
    && apk del .phpize-deps \
    && rm -fr /tmp/redis-${REDIS_VERSION}.tgz


# imap 扩展
RUN set -ex \
	&& apk add --no-cache  imap-dev openssl-dev \
	&& apk add --no-cache --virtual .phpize-deps ${PHPIZE_DEPS} \
    ## 修复openssl not found 错误  https://github.com/docker-library/php/issues/891#issuecomment-531023107
	&& PHP_OPENSSL=yes docker-php-ext-configure imap --with-imap --with-imap-ssl \
	&& docker-php-ext-install imap \
	&& apk del .phpize-deps \
    && docker-php-source delete

## mongodb 扩展
ENV MONGODB_VERSION 1.9.0
RUN set -x \
    && apk add --no-cache --virtual .phpize-deps \
                                    ${PHPIZE_DEPS} \
    && wget -O /tmp/mongodb-${MONGODB_VERSION}.tgz  https://pecl.php.net/get/mongodb-${MONGODB_VERSION}.tgz \
    && pecl install /tmp/mongodb-${MONGODB_VERSION}.tgz \
    && docker-php-ext-enable mongodb \
    && apk del .phpize-deps \
    && rm -fr /tmp/mongodb-${MONGODB_VERSION}.tgz


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


