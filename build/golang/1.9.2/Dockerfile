########################################################################################################################
##                                            Dockerfile for golang

FROM baykier/alpine:edge

MAINTAINER baykier 1035666345@qq.com

## golang worker place
ENV WORKPLACE /project
## golang version
ENV GO_VERSION 	1.9.2-r1

COPY ./docker-go-entrypoint /usr/bin/

RUN set -ex \
    && apk add --no-cache go-tools=${GO_VERSION} \
                                            git \
    && chmod +x /usr/bin/docker-go-entrypoint \
    && mkdir -p ${WORKPLACE}

WORKDIR ${WORKPLACE}

ENTRYPOINT ["docker-go-entrypoint"]

CMD ["go","help"]



