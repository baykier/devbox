##############################################################################################################################
##
##                       dockerfile for baykier/mysql:8.0

FROM percona:ps-8.0.23-14

LABEL ahthor=" baykier <1035666345@qq.com>"


COPY --chown=mysql:mysql ./docker-entrypoint.sh /usr/bin/docker-entrypoint.sh

# 自定义配置文件
# COPY --chown=mysql:mysql ./my.cnf /etc/mysql/

RUN set -ex \
    #&& chmod 644 /etc/mysql/* \
    && chmod +x /usr/bin/docker-entrypoint.sh

VOLUME ["/etc/mysql","/var/lib/mysql"]

EXPOSE 3306

USER root

ENTRYPOINT ["/usr/bin/docker-entrypoint.sh"]

CMD [ "mysqld","--user=root" ]

