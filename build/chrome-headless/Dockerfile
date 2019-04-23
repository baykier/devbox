### chrome-headless 中文字体支持
### https://stackoverflow.com/questions/49067625/how-can-i-use-chinese-in-alpine-headless-chrome

FROM zenika/alpine-chrome

MAINTAINER baykier <1035666345@qq.com>

USER root

RUN set -ex \
    && sed -i 's/dl-cdn.alpinelinux.org/mirrors.aliyun.com/g' /etc/apk/repositories \
    && apk add wqy-zenhei --no-cache --repository https://mirrors.aliyun.com/alpine/edge/testing --allow-untrusted 
    

EXPOSE 9222

USER chrome

ENTRYPOINT ["chromium-browser", "--headless", "--disable-gpu", "--disable-software-rasterizer", "--disable-dev-shm-usage", "--no-sandbox", "--remote-debugging-address=0.0.0.0", "--remote-debugging-port=9222"]
