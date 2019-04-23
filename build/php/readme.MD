php build说明
-------------

为了最小化镜像.PHP的image全部基于[alpine基础镜像](https://github.com/gliderlabs/docker-alpine)，和以alpine为底包的
[php官方镜像包](https://github.com/docker-library/php/)，在以上基础包上安装扩展，配合权限修改或者安装gosu制作的开发
环境

- 如何安装扩展

    php扩展分三种种，一种核心扩展，安装包包含在源码包安装时候直接安装就行，第二类是pecl上面的官方扩展，
    没有随源码一起发布，这种需要先从pecl上面下载扩展包，在进行安装，第三类就是其扩展具体
    可参考php docker 扩展安装(https://hub.docker.com/_/php/)

- 在安装pecl扩展时候，经常会发现错误如何解决

```
No releases available for package "pecl.php.net/gnupg"
install failed
```

为了避免这种错误，我们需要先下载下来安装包，再进行安装,以redis为例

```
    ##安装redis
    wget -O /tmp/redis-2.2.8.tgz http://pecl.php.net/get/redis-2.2.8.tgz \
    && pecl install /tmp/redis-2.2.8.tgz \
    && docker-php-ext-enable redis \
    && rm -fr /tmp/redis-2.2.8.tgz \
    ...

```

### php5.6

基于docker php 官方 5.6.32-fpm-alpine3.4,同时安装以下扩展

- memcached-2.2.0
- gd
- redis
- mongo


###默认安装扩展:

> cgi-fcgi Core ctype curl 

###镜像说明，考虑到镜像到可移植性，所以，对于php以及php-fpm的配置，保存在镜像内部。对于日志可以输出到文件

### php7.2

基于 docker php 官方镜像 php:7.2.0-fpm-alpine3.7 同时安装下面扩展

- pdo_mysql
- gd
- bcmath
- zip
- exif
- intl
- imagick
- redis
- mongodb
- memcached

### php7.2-cli

参考官方镜像，由于swoole需要libaio-dev 和linux/aio_abi.h 支持 底包采用alpine:edge ，同时修复的COPY docker-php-source 执行权限的问题
，编译php时加入 --enable-socktes 参数,下面是详细修改

- 底包采用 baykier/alpine:edge
- php源码下载url改为国内镜像地址，加快下载速度
- 修复 COPY docker-php-source shell脚本权限不足的问题(在阿里云服务器机器build时候)
- php编译参数加入--enable-socktes

### php-7.2-fpm

参考官方镜像，由于swoole需要libaio-dev 和linux/aio_abi.h 支持 底包采用alpine:edge ，同时修复的COPY docker-php-source 执行权限的问题
，编译php时加入 --enable-socktes 参数,下面是详细修改

- 底包采用 baykier/alpine:edge
- php源码下载url改为国内镜像地址，加快下载速度
- 修复 COPY docker-php-source shell脚本权限不足的问题(在阿里云服务器机器build时候)
- php编译参数加入--enable-socktes

### swoole-1.9.23

基于baykier/php:7.2-cli底包，并加入一些扩展

- inotify
- igbinary
- redis
- zip
- opcache
- bcmath
- pdo_mysql
- swoole-1.9.23 同时加入编译参数 --enable-http2    --enable-openssl  --enable-swoole-debug   --enable-async-redis   --enable-sockets
- composer

### swoole-2.0.10

基于baykier/php-7.2-cli底包，同时加入一些扩展

- inotify
- zip
- igbinary
- redis
- opcache
- bcmath
- pdo_mysql
- swoole-2.0.10 同时加入编译参数 --enable-http2    --enable-openssl  --enable-swoole-debug   --enable-async-redis   --enable-sockets --enable-coroutine
= composer