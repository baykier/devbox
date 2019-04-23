########################################################################################################################
##                                            Dockerfile for hugo

FROM baykier/alpine:edge

MAINTAINER baykier 1035666345@qq.com

## hugo worker place
ENV WORKPLACE /hugo
## hugo version
ENV HUGO_VERSION 0.30.2-r0

COPY ./docker-hugo-entrypoint /usr/bin/

RUN set -ex \
    && apk add --no-cache hugo=${HUGO_VERSION} \
    && chmod +x /usr/bin/docker-hugo-entrypoint \
    && mkdir -p ${WORKPLACE}

WORKDIR ${WORKPLACE}

ENTRYPOINT ["docker-hugo-entrypoint"]

CMD ["hugo","help"]


