###################################################################################
##
##          基于 docker-library/java官方镜像 java:8-jre-alpine
##

FROM java:8-jre-alpine

MAINTAINER baykier <aigoudebook@gmail.com>

ENV PLANTUML_VERSION 8055
ENV LANG en_US.UTF-8
RUN apk add --no-cache graphviz ttf-droid ttf-droid-nonlatin curl \
    && curl -L https://sourceforge.net/projects/plantuml/files/plantuml.${PLANTUML_VERSION}.jar/download -o /tmp/plantuml.jar \
    && mv /tmp/plantuml.jar /bin/plantuml.jar \
    && chmod +x /bin/plantuml.jar \
    && apk del curl

WORKDIR "/project"

VOLUME "/project"

ENTRYPOINT [ "java", "-jar", "/bin/plantuml.jar"]
CMD [ "-h" ]