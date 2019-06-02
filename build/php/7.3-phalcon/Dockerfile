########################################################################################################################
##                                      Dockerfile for baykier/php:7.2-phalcon dev
##

FROM mileschou/phalcon:7.3-fpm-alpine

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
    && apk add ca-certificates git 

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

## 设置工作目录
ENV WORKDIR_SPACE /project

RUN set -ex \
    && mkdir -p ${WORKDIR_SPACE} /var/log/php  

## 安装phalcon
ARG PHALCON_VERSION=3.4.2
ARG PHALCON_EXT_PATH=php7/64bits

RUN set -xe \
    # Compile Phalcon
	&& apk add --no-cache --virtual .phpize-deps \
                                    ${PHPIZE_DEPS} \
  	&& curl -LO https://github.com/phalcon/cphalcon/archive/v${PHALCON_VERSION}.tar.gz \
    &&  tar xzf v${PHALCON_VERSION}.tar.gz \
	&& cd cphalcon-${PHALCON_VERSION}/build \
	&& sh install \
    &&  echo "extension=phalcon.so" > /usr/local/etc/php/conf.d/phalcon.ini  \
    &&  cd ../.. && rm -rf v${PHALCON_VERSION}.tar.gz cphalcon-${PHALCON_VERSION} \
	&& apk del .phpize-deps \
	# devtools
	&& curl -LO https://github.com/phalcon/phalcon-devtools/archive/v${INSTALL_VERSION}.tar.gz \
	&&	tar xzf v${INSTALL_VERSION}.tar.gz \
	&&	rm -rf v${INSTALL_VERSION}.tar.gz \
	&&	mv phalcon-devtools-${INSTALL_VERSION} /usr/src/phalcon-devtools \
	&&	ln -sf /usr/src/phalcon-devtools/phalcon.php /usr/local/bin/phalcon 


## 配置文件
COPY ./php.ini-dev ${PHP_INI_DIR}/php.ini
COPY ./php-ext.conf.d/* ${PHP_INI_DIR}/conf.d/
COPY ./php-fpm.d/* ${PHP_INI_DIR}/php-fpm.d/
COPY ./php-fpm.conf /usr/local/etc/php-fpm.conf


WORKDIR ${WORKDIR_SPACE}

VOLUME ${WORKDIR_SPACE}

EXPOSE 9000

ENTRYPOINT gosu root php-fpm


