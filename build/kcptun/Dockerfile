#
# Dockerfile for shadowsocks-libev with kcptun
# refer https://github.com/shadowsocks/kcptun
#

FROM alpine:3.10

LABEL MAINTAINER="baykier 1035666345@qq.com"

ARG KCPTUN_VERSION="20200409"
ENV KCPTUN_URL https://github.com/xtaci/kcptun/releases/download/v${KCPTUN_VERSION}/kcptun-linux-amd64-${KCPTUN_VERSION}.tar.gz
ENV KCPTUN_HOME "/opt/kcptun"

# install kcptun
RUN set -ex \
    && apk add --no-cache  --virtual TMP curl tar \
    && mkdir -p ${KCPTUN_HOME} \
    && cd ${KCPTUN_HOME} \
    && curl -sSL $KCPTUN_URL | tar xz \
    && apk del TMP

ENV KCPTUN_TARGET ""
ENV KCPTUN_KEY "key"
ENV KCPTUN_CRYPY "aes-128"
ENV KCPTUN_MODE "fast2"

EXPOSE 2990


 CMD /opt/kcptun/server_linux_amd64 --listen "0.0.0.0:2990" \
                                    --target $KCPTUN_TARGET \
                                    --key    $KCPTUN_KEY \
                                    --crypt  $KCPTUN_CRYPY \
                                    --mode   $KCPTUN_MODE
