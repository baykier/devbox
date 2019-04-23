#!/usr/bin/env bash
## devbox.sh for docker
##需要注意的是 bash 默认的指令分割符为&& 或者;所以输入的命令中不能包含，否则命令会被截断
##set -x
BIN="docker-compose"
CMD="exec"

## 验证服务是否存在
SERVICES=$(cd `pwd` && docker-compose config --services)
for _S in $SERVICES
do
    _SS=$(echo $* | grep -o $_S)
    if [[ $_S == $_SS ]]; then
        SERVICE=$_SS
    fi
done

if [[ -z $SERVICE ]]; then
    echo -e "devbox.sh v0.01  "
    echo -e "usage  ./devbox.sh service command  "
    echo -e "请输入有效的service它们是下面之一..."
    for _S in $SERVICES
    do
        echo -e "$_S"
    done
    exit 1
fi


## 所有参数左移
shift
if [[ $# == 0 ]]; then
    echo -e "devbox.sh v0.01  "
    echo -e "usage  ./devbox.sh service command  "
    echo -e "请输入指定服务$SERVICE内部的命令....\n"
    exit 2
fi
## 目录路径替换
## 将 project .project 替换成容器内部的路径 /project
ARGS=$*
for DES in "project" "./project"
    do
        SEARCH=$(echo "$*" | grep -o "$DES" )
        if [[ "$DES" == "$SEARCH" ]]; then
            ARGS=$(echo "$*" | sed "s@$DES@/project@" )
        fi
    done
##执行docker-compose exec service sh -c "$cmd"
$BIN $CMD $SERVICE sh -c "$ARGS"
