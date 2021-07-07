######################################################################################################################
##
##                                   基于docker-library/nginx官方镜像
##                                https://hub.docker.com/r/library/nginx
##                               https://github.com/nginxinc/docker-nginx

FROM nginx:1.21.1-alpine

LABEL author="baykier <1035666345@qq.com>"

## 安装 shadow (usermod)

RUN set -ex \
    && rm -fr /etc/nginx/conf.d \
    && rm -f /etc/nginx/nginx.conf

COPY --chown=nginx:nginx ./nginx.conf /etc/nginx/nginx.conf
COPY --chown=nginx:nginx ./conf.d /etc/nginx/conf.d
COPY --chown=root:root ./docker-entrypoint.sh /usr/bin/docker-entrypoint.sh

RUN set -x \
    && apk --no-cache add shadow \
    && chmod 644 /etc/nginx/nginx.conf \
    && chmod 644 /etc/nginx/conf.d/* \
    && chmod +x /usr/bin/docker-entrypoint.sh

## 指定端口

EXPOSE 80 443

## 默认命令

STOPSIGNAL SIGTERM



ENTRYPOINT [ "/usr/bin/docker-entrypoint.sh" ]




