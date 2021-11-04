######################################################################################################################################
##
##                                                   Dockerfile  for openface
##

FROM garyfeng/docker-openface:latest

LABEL mainter "baykier 1035666345@qq.com"

RUN set -ex \
    && apt-get update \
    && apt-get install -y software-properties-common wget \
    && add-apt-repository ppa:deadsnakes/ppa \
    && apt-get update \
    && apt-get purge -y python3 \
    && apt-get autoremove -y \
    && apt-get install -y python3.9 \
    && ln -s /usr/bin/python3.9 /usr/bin/python \
    && apt-get install -y  python3.9-distutils \
    && wget https://bootstrap.pypa.io/get-pip.py \
    && python3.9 get-pip.py \
    # clean
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*

WORKDIR /project


ENTRYPOINT ["tail","-f","/etc/hosts"]