开发环境安装
=============

vagrant安装，这里略过
---------------------

docker&&docker-compose安装和阿里云加速器配置
--------------------------------------------

这里推荐使用阿里云镜像安装[Ali-OSM](http://mirrors.aliyun.com/help/docker-engine?spm=0.0.0.0.VThZTI)

### 1. 安装docker

```
curl -sSL http://acs-public-mirror.oss-cn-hangzhou.aliyuncs.com/docker-engine/internet | sh -

```

### 2. 安装docker-compose

#### 1. 通过python-pip安装

```
sudo apt update

sudo apt install -y python-pip

sudo pip install -y docker-compose

```

#### 2. 拉取docker-compose镜像安装

```
curl -L https://github.com/docker/compose/releases/download/1.10.0/run.sh > ~/bin/docker-compose
chmod +x ~/bin/docker-compose

```

### 3. 配置阿里云加速器

```
sudo nano /etc/docker/daemon.json

## 加入下面内容保存
{
  "registry-mirrors": ["https://xxxxx.mirror.aliyuncs.com"]
}

sudo systemctl daemon-reload
sudo systemctl restart docker

```
