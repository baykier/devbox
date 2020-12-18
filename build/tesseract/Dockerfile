################################################################################
##
## dockerfile for tesseract

FROM tesseractshadow/tesseract4re

COPY --chown=root:root ./sources.list /etc/apt/sources.list
COPY --chown=root:root ./docker-python-entrypoint /usr/bin/


RUN apt-get update && \
    apt-get install -y  python3-pip && \
    pip3 install -i  https://mirrors.aliyun.com/pypi/simple/    scikit-build \
                                                                cmake && \
    pip3 install -i  https://mirrors.aliyun.com/pypi/simple/  opencv-python \
                                                                pytesseract \
                                                                click \
                                                                matplotlib \
                                                                toml

WORKDIR /project

RUN chmod +x /usr/bin/docker-python-entrypoint

ENTRYPOINT [ "docker-python-entrypoint" ]