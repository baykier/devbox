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

COPY server_config.json $KCPTUN_HOME


 CMD ["/opt/kcptun/server_linux_amd64","-c","/opt/kcptun/server_config.json"]
