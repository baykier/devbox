# coding: utf-8
from sshtunnel import SSHTunnelForwarder
from sshtunnel import BaseSSHTunnelForwarderError
import os

# sshtunnel

try:
    # 获取绑定参数
    ssh_server_host = str(os.environ.get('SSH_SERVER_HOST'))
    ssh_server_port = int(os.environ.get('SSH_SERVER_PORT'))
    ssh_username = str(os.environ.get('SSH_USERNAME'))
    ssh_password = str(os.environ.get('SSH_PASSPORD'))
    bind_remote_host = str(os.environ.get('BIND_REMOTE_HOST'))
    bind_remote_port = int(os.environ.get('BIND_REMOTE_PORT'))
    bind_local_host = str(os.environ.get('BIND_LOCAL_HOST','0.0.0.0'))
    bind_local_port = int(os.environ.get('BIND_LOCAL_PORT', 3306))
    # 开启隧道
    with SSHTunnelForwarder(
        (ssh_server_host, ssh_server_port),
        ssh_username=ssh_username,
        ssh_password=ssh_password,
        remote_bind_address=(bind_remote_host,bind_remote_port),
        local_bind_address=(bind_local_host, bind_local_port)
    ) as server:
        print("tunnel start at {}:{}" . format(server.local_bind_host,server.local_bind_port))
        while True:
            pass
except Exception  as e:
    print("系统错误:", e)
