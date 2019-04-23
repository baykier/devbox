#!/bin/sh

set -ex

gosu root mkdir -p $WORK_DIR

if [ ! -e /etc/mysql/.INIT ]; then
	gosu root touch /etc/mysql/.INIT
	gosu root cp -a $BACKUP/mysql/* /etc/mysql/

	gosu root mysql_install_db --user=root > /dev/null

	TMP=/etc/mysql/.tmp
    gosu root echo > $TMP
    gosu root echo "USE mysql;" >> $TMP
    gosu root echo "FLUSH PRIVILEGES;" >> $TMP
    gosu root echo "GRANT ALL PRIVILEGES ON *.* TO 'root'@'%' IDENTIFIED BY '$DB_ROOT_PASSWORD' WITH GRANT OPTION;" >> $TMP
    gosu root echo "GRANT ALL PRIVILEGES ON *.* TO 'root'@'localhost' WITH GRANT OPTION;" >> $TMP
    gosu root echo "UPDATE user SET password=PASSWORD('$DB_ROOT_PASSWORD') WHERE user='root' AND host='localhost';" >> $TMP
	gosu root echo "CREATE DATABASE IF NOT EXISTS \`$DB_DATABASE\` CHARACTER SET utf8 COLLATE utf8_general_ci;" >> $TMP
 	gosu root  echo "GRANT ALL ON \`$DB_DATABASE\`.* to '$DB_USER'@'%' IDENTIFIED BY '$DB_PASSWORD';" >> $TMP

    gosu root   /usr/bin/mysqld --user=root --bootstrap --verbose=0 < $TMP
    gosu root rm -f $TMP
fi

gosu root /usr/bin/mysqld --user=root --console