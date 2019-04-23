##################################################################################################################################################
##                                   Dockerfile for tushare

FROM python:3.7-alpine3.8

MAINTAINER baykier <1035666345@qq.com>

COPY ./docker-python-entrypoint /usr/bin/

RUN set -ex \
    && apk update \
    && apk add  subversion \
                py3-setuptools \
                lapack-dev \
                gcc \
                freetype-dev \              
                musl-dev \
                g++ \
                libxslt-dev \
                python3-dev \
    && ln -s /usr/include/locale.h /usr/include/xlocale.h \ 
    && pip3 install tushare \
                    bs4 \
    && chmod +x /usr/bin/docker-python-entrypoint

ENTRYPOINT ["docker-python-entrypoint"]

CMD ["python3"]



