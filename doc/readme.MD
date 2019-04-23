devbox使用文档
==============

简介
----

devbox是一个基于[virtualbox](https://www.virtualbox.org/)和[vagrant](https://www.vagrantup.com/)的开发环境
主要用于软件开发，并在vagrant中借助docker来完成开发环境的切换，部署

安装
----

1. 安装virtualbox(具体参考官方文档)

2. 安装vagrant(具体参考官方文档)

3. vagrant box 这里使用[https://vagrantcloud.com/geerlingguy/boxes/ubuntu1604/versions/1.1.4](https://vagrantcloud.com/geerlingguy/boxes/ubuntu1604/versions/1.1.4)

4. 安装docker(如果需要，参考官方文档)


使用
----

vagrant 使用 Vagrantfile 来作为虚拟机的配置文件，在[devbox](https://github.com/baykier/devbox)根目录放置了一个
Vagrantfile,使用的时候可以根据需要随时调整

我推荐的使用[git bash](https://git-scm.com/download/win),里面集成了git，和一些简单的linux环境，安装好vagrant后，
可以直接像linux环境一样使用

1. git clone 本项目

```
	# 切换到合适的目录
	cd /path to your project/
	# clone devbox
	git clone https://github.com/baykier/devbox.git
	# 进入devbox目录
	cd devbox
	
```

2. vagrant box 添加，vagrant启动，配置重载 关闭

由于GFW的原因，vagrant对于国内的用户来说直接使用`vagrant init geerlingguy/debian8`来说非常慢，
所以推荐先下载到本地然后在添加

```
	vagrant box add <box name> <box path>
```

3. vagrant 使用

```
	# 进入 devbox目录 vagrant up 是开启虚拟机 vagrant ssh是ssh登录虚拟机
	vagrant up && vagrant ssh
	# vagrant reload 是当更新Vagrantfile后，重新载入当前配置
	vagrant reload
	# vagrant halt 关闭虚拟机，当关闭电脑时，务必要先关闭vagrant 的虚拟机，防止出现不可预知的错误
	vagrant halt
```

4. 进入ubuntu 1604后操作

由于是进入ubuntu1604操作，所以基本上就是在内置的linux中操作了下面简单的

4.1安装php7.0

```
 sudo apt update
 sudo apt install php7.0-cli php7.0-dev php7.0-common php-xdebug
 
```

4.2安装并配置nginx

nginx默认配置为`/etc/nginx/site-aviable/default`

```
 sudo apt install nginx
 
 #配置虚拟机
 sudo cp /etc/nginx/site-aviable/default /etc/nginx/site-aviable/<vhost name>
 sudo nano /etc/nginx/site-aviable/<vhost name>
 sudo ln -s /etc/nginx/site-aviable/<vhost name> //etc/nginx/site-enbaled/<vhost name>
 # 重载nginx配置
 sudo systemctl reload nginx.service
```
4.3 mariadb安装配置



