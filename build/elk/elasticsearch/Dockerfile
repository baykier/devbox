ARG ELK_VERSION

# https://github.com/elastic/elasticsearch-docker
FROM elasticsearch:${ELK_VERSION}

# Add your elasticsearch plugins setup here
# Example: RUN elasticsearch-plugin install analysis-icu



RUN set -ex && \
    elasticsearch-plugin install -b https://github.com/medcl/elasticsearch-analysis-ik/releases/download/v6.7.2/elasticsearch-analysis-ik-6.7.2.zip