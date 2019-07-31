################################################################################################################################################
##                                      docker for lumen-fpm-7.1

FROM urbit/lumen-php-fpm

MAINTAINER baykier <1035666345@qq.com>



## 安装扩张

## 安装exif扩展
RUN set -x \
    && docker-php-source extract \
    && docker-php-ext-install exif \
    && docker-php-source delete


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
    && rm -fr /tmp/tideways* \
    && apk del .phpize-deps

# PHP
ADD ./logging.ini /usr/local/etc/php/conf.d
ADD ./lumen.ini /usr/local/etc/php/conf.d
COPY ./php-ext.conf.d/* ${PHP_INI_DIR}/conf.d/

# PHP-FPM
ADD ./lumen.pool.conf /usr/local/etc/php-fpm.d
RUN rm -fr /usr/local/etc/php-fpm.d/www.conf*




ENV WORKDIR /project

RUN mkdir $WORKDIR

WORKDIR $WORKDIR

EXPOSE 9000
