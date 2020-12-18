#
# Dockerfile for shadowsocks-libev with kcptun
# refer https://github.com/shadowsocks/kcptun
#

FROM alpine:3.10

LABEL MAINTAINER="baykier 1035666345@qq.com"

ARG KCP_VER="20200409"
ENV KCP_URL https://github.com/xtaci/kcptun/releases/download/v${KCP_VER}/kcptun-linux-amd64-${KCP_VER}.tar.gz
ENV KCP_HOME "/opt/kcptun"

# install kcptun
RUN set -ex \
    && apk add --no-cache  --virtual TMP curl tar \
    && mkdir -p ${KCPTUN_HOME} \
    && cd ${KCPTUN_HOME} \
    && curl -sSL $KCPTUN_URL | tar xz \
    && apk del TMP

ENV KCP_TARGET ""
ENV KCP_KEY "key"
ENV KCP_CRYPT "aes-128"
ENV KCP_MODE "fast2"

EXPOSE 2990


 CMD /opt/kcptun/server_linux_amd64 --listen "0.0.0.0:2990" \
                                    --target $KCP_TARGET \
                                    --key    $KCP_KEY \
                                    --crypt  $KCP_CRYPT \
                                    --mode   $KCP_MODE
