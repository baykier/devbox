################################################################################
##
## dockerfile for tesseract

FROM tesseractshadow/tesseract4re

COPY --chown=root:root ./sources.list /etc/apt/sources.list
COPY --chown=root:root ./Miniconda3-latest-Linux-x86_64.sh /usr/local/bin/miniconda.sh


# RUN apt-get update && \
#     apt-get install -y  wget

# anconda install



# ENV ANCONDA_INSTALLER_URL = "https://mirrors.tuna.tsinghua.edu.cn/anaconda/archive/Anaconda3-2020.07-Linux-x86_64.sh"

#  ENV MINICONDA_URL = "https://mirrors.tuna.tsinghua.edu.cn/anaconda/miniconda/Miniconda3-latest-Linux-ppc64le.sh"

#  RUN wget ${MINICONDA_URL} -O ~/miniconda.sh

#  RUN which bash && whoami && ls -alF ~
RUN  bash /usr/local/bin/miniconda.sh -b -p /usr/local/bin/miniconda


# RUN pip3 install -i https://mirrors.aliyun.com/pypi/simple/ cmake \
#                                                             scikit-build \
#                                                             pipenv \
#                                                             pytesseract \
#                                                             pillow \
#                                                             opencv-python \
#                                                             click \
#                                                             matplotlib \
#                                                             toml


WORKDIR /project

RUN which top

# ENTRYPOINT [ "top" ]