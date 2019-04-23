########################################################################################################################
##                                            Dockerfile for python2.7

FROM baykier/alpine:edge

MAINTAINER baykier 1035666345@qq.com

## python worker place
ENV WORKPLACE /project
## python version
ENV PYTHON_VERSION 2.7.14-r2

COPY ./docker-python-entrypoint /usr/bin/

RUN set -ex \
    && apk add --no-cache python2-dev=${PYTHON_VERSION} \
    && chmod +x /usr/bin/docker-python-entrypoint \
    && mkdir -p ${WORKPLACE}

WORKDIR ${WORKPLACE}

ENTRYPOINT ["docker-python-entrypoint"]

CMD ["python","--help"]


