################################################################################################################################################
##                                      docker for lumen-fpm-7.1

FROM urbit/lumen-php-fpm

MAINTAINER baykier <1035666345@qq.com>

# PHP
ADD ./logging.ini /usr/local/etc/php/conf.d
ADD ./lumen.ini /usr/local/etc/php/conf.d

# PHP-FPM
ADD ./lumen.pool.conf /usr/local/etc/php-fpm.d
RUN rm -fr /usr/local/etc/php-fpm.d/www.conf*


ENV WORKDIR /project

RUN mkdir $WORKDIR

WORKDIR $WORKDIR

EXPOSE 9000
