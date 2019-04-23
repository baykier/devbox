lighttpd-php-fpm说明
=========================

为什么要放在一起?
----------------

由于**lighttpd**目前不支持域名解析fastcgi的设置，这对于**一个容器一个微服务**的模式而言，无疑
是无法应用的，所有才有把lighttpd和php-fpm放在一起的需求

构建说明
--------

###安装的软件包

* php-5.6.29

* lighttpd-1.4.39

* memcached-1.4.34

* openssh

* idallocator 订单ID生成器


###php安装的扩展

镜像构建时参考php的官方镜像[docker-library/php/5.6/fpm/alpine](https://github.com/docker-library/php/blob/75854bb7e97d3e01c9b3597674450be43521e296/5.6/fpm/alpine/Dockerfile)
仅仅修改配置目录，同时安装了下面的扩展:

* memcached-2.2.0

* memcache-2.2.7

* gd

* xdebug

* mysqli

* pdo_mysql

* redis

* mcrypt

* calendar

* zip

* **sign**扩展，自定义扩展

如果有新的扩展需求，请参考[How to install more PHP extensions](https://hub.docker.com/_/php/)

lighttpd 版本为1.4.39，采用 默认的apk安装包安装

如何使用？
---------

###目录结构

项目最终的目录结构如下:

* [project](#)
	* [conf](#)
	* [htdocs](#)
	* [log](#)
	* [data](#)
	* [.INIT](#)

上面的所有目录及文件在容器启动时自动创建，推荐使用空目录指定数据卷,这样便于保持良好的目录结构
其中:
* **conf**为所有的配置目录，包括lighttpd,php. 

* **htdocs** 为主站目录 

* **log** 为所有的日志目录 

* **data**为数据目录比如mysql的lib等等，

* **.INIT**文件为第一次初始化时自动创建

###使用方法


* docker cli:

```
	docker run --name <some name> -d -v <path>:/var/project -p 80:80 -p 443:443 <image>
```

* docker-compose(推荐这种方式):

```
	##docker-compose.yml 文件
	<some name>:
        image: <image>
        volumes:
            - <path>:/var/project
        extra_hosts:
                - "ad.zhiwo.com:127.0.0.1"
        ports:
            - "80:80"
            - "443:443"
	##启动
	docker-compose up -d <some name>
```

如果有修改php配置，或者lighttpd配置，重启容器即可

相关资源
--------

* [docker-xunsearch](https://hub.docker.com/r/hightman/xunsearch/)

* [lighttpd](https://www.lighttpd.net/)


