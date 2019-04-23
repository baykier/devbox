composer build for php
======================

使用方法
--------

- 将下面的代码保存到可执行文件composer添加可执行权限，就可以全局使用了

```
#!/usr/bin/env bash
docker run --rm -ti -v $PWD:/var/composer baykier/composer:1.5.6 "$@"
```
- 或者下载 **./composer** 保存到本地环境目录，添加可执行权限

依赖
----

- docker
