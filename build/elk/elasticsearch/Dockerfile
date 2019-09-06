ARG ELK_VERSION

# https://github.com/elastic/elasticsearch-docker
FROM elasticsearch:${ELK_VERSION}

# Add your elasticsearch plugins setup here
# Example: RUN elasticsearch-plugin install analysis-icu

COPY --chown=elasticsearch:elasticsearch ./plugins/* /tmp/

RUN set -ex && \
    chmod 755 /tmp/elasticsearch-analysis-ik-6.7.2.zip && \
    elasticsearch-plugin install -b file:///tmp/elasticsearch-analysis-ik-6.7.2.zip