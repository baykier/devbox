Mongo build
===========

底包采用baykier/alpine:3.7 采用 apk 安装

如何保存数据文件
----------------

alpine 版的mongo 默认数据保存在 **`/data/db`**,配置文件位于 **`/data/conf`**,挂载数据卷时要注意
不能用virtualbox 同步的目录 ，这样会导致容器启动出错   具体请参考
https://docs.mongodb.com/manual/administration/production-notes/#fsync-on-directories

如何新增新用户
--------------

新增用户采用mongo 命令 下面为例子

```
docker-compose exec mongo mongo
use admin
db.createUser({user:'admin',pwd:'123456',roles:[{role:"userAdminAnyDatabase",db:"admin"}]})

```
具体参考https://docs.mongodb.com/manual/reference/method/db.createUser/#db.createUser

日志文件保存在哪里
------------------

日志保存路径默认保存在/var/log/mongo/mongod.log,可以挂载目录到此处将日志保存到本地也可以修改配置文件