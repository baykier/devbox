# kcptun build for shadowsocks


## 基本使用


>  配置kcp 参数 和 sshaodwocks 参数

 ```
    docker-compose  up -d kcp-server

 ```

## 扩展

> 如果kcptun_server 有额外的参数，直接编辑 **Dockerfile** ,构建即可

## 客户端使用


### windows pc

1. 下载编译好的windows kcptun 可执行文件，复制到shadowscks目录，重新命名为kcptun.exe  
2. 配置windows gui 在服务器配置的插件中  填写kcptun,插件参数为服务器上面配置的参数
如下

```
    -l %SS_LOCAL_HOST%:%SS_LOCAL_PORT% -r %SS_REMOTE_HOST%:%SS_REMOTE_PORT% --key key --crypt aes-128 --mode fast2
```

### android 

1. 下载apk 文件 [shadowsocks android](https://github.com/shadowsocks/kcptun-android)

2. 配置插件参数 

## 参考

- https://github.com/shadowsocks/shadowsocks-windows/issues/2211
- https://github.com/shadowsocks/kcptun-android
- https://github.com/shadowsocks/shadowsocks-windows/wiki/%E4%BD%BF%E7%94%A8%E9%9D%9ESIP003%E8%A7%84%E8%8C%83%E7%9A%84%E6%8F%92%E4%BB%B6
