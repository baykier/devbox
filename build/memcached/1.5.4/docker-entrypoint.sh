#!/bin/sh

set -e

# first arg is `-f` or `--some-option`
if [ "${1#-}" != "memcached" ]; then
	set -- memcached "$@"
fi

exec "$@"