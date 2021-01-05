FROM python:3.8-alpine3.12


COPY . /app

RUN set -ex \
    && sed -i 's/dl-cdn.alpinelinux.org/mirrors.aliyun.com/g' /etc/apk/repositories \
    && apk add --no-cache tzdata \
    && cp /usr/share/zoneinfo/Asia/Shanghai /etc/localtime \
    && apk add --no-cache libxml2-dev \
            libxslt-dev \
            gcc \
            g++ \
    && pip install -i https://pypi.tuna.tsinghua.edu.cn/simple -r /app/requirements.txt

WORKDIR /app

CMD ["python","main.py"]