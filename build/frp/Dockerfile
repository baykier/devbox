########################################################################################################################
##                                            Dockerfile for frp

FROM alpine:3.11

MAINTAINER baykier 1035666345@qq.com

RUN mkdir -p /frp && mkdir -p /etc/frp

ADD ./frp_0.32.0_linux_amd64.tar.gz /frp/

RUN set -ex && \
    cp /frp/frp_0.32.0_linux_amd64/*.ini /etc/frp -R && \
    ls -alF /frp/frp_0.32.0_linux_amd64/ && \
    ln -s /frp/frp_0.32.0_linux_amd64/frps /usr/local/bin/frps &&  \
    ln -s /frp/frp_0.32.0_linux_amd64/frpc /usr/local/bin/frpc

CMD ["frps","-h"]

