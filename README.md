# 基于linux下muduo、protobuf和zookeeper开发的rpc框架

## 项目框架

![](https://s3.bmp.ovh/imgs/2024/03/09/15c043806fc3faf3.png)



![](https://s3.bmp.ovh/imgs/2024/03/09/258ab99ed5095b50.png)



![](https://s3.bmp.ovh/imgs/2024/03/09/c0a41088810d69d1.png)



## 项目工作流程

![](https://s3.bmp.ovh/imgs/2024/03/11/8f5cc196bfe62b94.png)

## 项目描述



- 基于protobuf和muduo封装rpc服务提供。实现远端rpc服务及方法的注册和基于回调的异步调用。
- 封装zookeeper原生API实现rpc服务节点在zookeeper服务上的注册与分发。基于Zookeeper 的 wathcer 机制,动态监视节点刷新服务列表，编写策略解决 Zookeeper watcher 的一次性触发问题。
- 自定义rpc协议解决网络数据粘包问题，基于Protobuf实现远程调用参数的序列化和反序列化，减小请求体积
- 自定义线程安全的队列实现日志模块。

## 技术栈

- muduo网络库开发
- protobuf消息序列化与反序列化
- zookeeper分布式节点配置

## 环境配置

- 系统：Ubuntu18.04
- muduo网络库
- protubuf：3.11.0
- zookeeper：3.4.10

## 编译运行

1.修改配置文件，原始配置如下，如在本地运行，可不修改。

```bash
# rpc节点ip
rpcServerIp=127.0.0.1
# rpc节点port
rpcServerPort=8000
# zookeeper节点ip
zkIp=127.0.0.1
# zookeeper节点port
zkPort=2181
```

2.执行一键编译脚本

```bash
./autobuild.sh
```

3.打开zookeeper服务

4.进入bin目录下，首先打开rpc服务器

```bash
./provider -i ../conf/rpcConf.conf
```

如下所示，若没有报错，则运行成功！运行成功后，rpc服务器会一直等待rpc客户端的调用

```bash
2024-03-08 16:52:59,206:3421(0x7f58cd99ca00):ZOO_INFO@log_env@726: Client environment:zookeeper.version=zookeeper C client 3.4.10
2024-03-08 16:52:59,206:3421(0x7f58cd99ca00):ZOO_INFO@log_env@730: Client environment:host.name=robot
2024-03-08 16:52:59,206:3421(0x7f58cd99ca00):ZOO_INFO@log_env@737: Client environment:os.name=Linux
2024-03-08 16:52:59,206:3421(0x7f58cd99ca00):ZOO_INFO@log_env@738: Client environment:os.arch=5.4.0-150-generic
2024-03-08 16:52:59,206:3421(0x7f58cd99ca00):ZOO_INFO@log_env@739: Client environment:os.version=#167~18.04.1-Ubuntu SMP Wed May 24 00:51:42 UTC 2023
2024-03-08 16:52:59,207:3421(0x7f58cd99ca00):ZOO_INFO@log_env@747: Client environment:user.name=pcl
2024-03-08 16:52:59,207:3421(0x7f58cd99ca00):ZOO_INFO@log_env@755: Client environment:user.home=/home/pcl
2024-03-08 16:52:59,207:3421(0x7f58cd99ca00):ZOO_INFO@log_env@767: Client environment:user.dir=/home/pcl/cProject/projects/myPro/mpRPC/mpRPCFrame
2024-03-08 16:52:59,207:3421(0x7f58cd99ca00):ZOO_INFO@zookeeper_init@800: Initiating client connection, host=127.0.0.1:2181 sessionTimeout=30000 watcher=0x559aa1224832 sessionId=0 sessionPasswd=<null> context=(nil) flags=0
2024-03-08 16:52:59,208:3421(0x7f58caf1d700):ZOO_INFO@check_events@1728: initiated connection to server [127.0.0.1:2181]
2024-03-08 16:52:59,222:3421(0x7f58caf1d700):ZOO_INFO@check_events@1775: session establishment complete on server [127.0.0.1:2181], sessionId=0x18e1d43f5860000, negotiated timeout=30000
zookeeper_init success!
znode create success!
znode create success!
20240308 08:53:42.531410Z  3421 INFO  TcpServer::newConnection [rpcProvider] - new connection [rpcProvider-127.0.0.1:8000#1] from 127.0.0.1:44446 - TcpServer.cc:80
============================
headerSize:25
headerStr:
UserServiceRPClogin
serviceName:UserServiceRPC
methodName:login
argsSize:18
args:                                                                                                             zhangsan123456
============================
doing login service
login success!  name:zhangsan passwd:123456
20240308 08:53:42.532026Z  3421 INFO  TcpServer::removeConnectionInLoop [rpcProvider] - connection rpcProvider-127.0.0.1:8000#1 - TcpServer.cc:109
xterm-256color20240308 08:53:42.564528Z  3421 INFO  TcpServer::newConnection [rpcProvider] - new connection [rpcProvider-127.0.0.1:8000#2] from 127.0.0.1:44454 - TcpServer.cc:80
============================
headerSize:23
headerStr:
UserServiceRPCreg
serviceName:UserServiceRPC
methodName:reg
argsSize:18
args李四666666
============================
doing register serivice!
login success!  name:李四 passwd:666666
20240308 08:53:42.564988Z  3421 INFO  TcpServer::removeConnectionInLoop [rpcProvider] - connection rpcProvider-127.0.0.1:8000#2 - TcpServer.cc:109

```

5.打开rpc客户端调用者

```bash
./consumer -i ../conf/rpcConf.conf
```

如没有报错，则运行成功，如下所示

```bash
============================
headerSize:25
serviceName:UserServiceRPC
methodName:login
argsSize:18
argsStr:                                                                                                          zhangsan123456
============================
2024-03-08 16:53:42,522:3461(0x7ff680d75d00):ZOO_INFO@log_env@726: Client environment:zookeeper.version=zookeeper C client 3.4.10
2024-03-08 16:53:42,522:3461(0x7ff680d75d00):ZOO_INFO@log_env@730: Client environment:host.name=robot
2024-03-08 16:53:42,522:3461(0x7ff680d75d00):ZOO_INFO@log_env@737: Client environment:os.name=Linux
2024-03-08 16:53:42,522:3461(0x7ff680d75d00):ZOO_INFO@log_env@738: Client environment:os.arch=5.4.0-150-generic
2024-03-08 16:53:42,522:3461(0x7ff680d75d00):ZOO_INFO@log_env@739: Client environment:os.version=#167~18.04.1-Ubuntu SMP Wed May 24 00:51:42 UTC 2023
2024-03-08 16:53:42,523:3461(0x7ff680d75d00):ZOO_INFO@log_env@747: Client environment:user.name=pcl
2024-03-08 16:53:42,523:3461(0x7ff680d75d00):ZOO_INFO@log_env@755: Client environment:user.home=/home/pcl
2024-03-08 16:53:42,523:3461(0x7ff680d75d00):ZOO_INFO@log_env@767: Client environment:user.dir=/home/pcl/cProject/projects/myPro/mpRPC/mpRPCFrame/bin
2024-03-08 16:53:42,523:3461(0x7ff680d75d00):ZOO_INFO@zookeeper_init@800: Initiating client connection, host=127.0.0.1:2181 sessionTimeout=30000 watcher=0x55b92e98cd2c sessionId=0 sessionPasswd=<null> context=(nil) flags=0
2024-03-08 16:53:42,526:3461(0x7ff67eaf4700):ZOO_INFO@check_events@1728: initiated connection to server [127.0.0.1:2181]
2024-03-08 16:53:42,528:3461(0x7ff67eaf4700):ZOO_INFO@check_events@1775: session establishment complete on server [127.0.0.1:2181], sessionId=0x18e1d43f5860001, negotiated timeout=30000
zookeeper_init success!
2024-03-08 16:53:42,532:3461(0x7ff680d75d00):ZOO_INFO@zookeeper_close@2527: Closing zookeeper sessionId=0x18e1d43f5860001 to [127.0.0.1:2181]

rpc login response success!1
============================
headerSize:23
serviceName:UserServiceRPC
methodName:reg
argsSize:18
argsStr李四666666
============================
2024-03-08 16:53:42,533:3461(0x7ff680d75d00):ZOO_INFO@log_env@726: Client environment:zookeeper.version=zookeeper C client 3.4.10
2024-03-08 16:53:42,533:3461(0x7ff680d75d00):ZOO_INFO@log_env@730: Client environment:host.name=robot
2024-03-08 16:53:42,533:3461(0x7ff680d75d00):ZOO_INFO@log_env@737: Client environment:os.name=Linux
2024-03-08 16:53:42,533:3461(0x7ff680d75d00):ZOO_INFO@log_env@738: Client environment:os.arch=5.4.0-150-generic
2024-03-08 16:53:42,533:3461(0x7ff680d75d00):ZOO_INFO@log_env@739: Client environment:os.version=#167~18.04.1-Ubuntu SMP Wed May 24 00:51:42 UTC 2023
2024-03-08 16:53:42,533:3461(0x7ff680d75d00):ZOO_INFO@log_env@747: Client environment:user.name=pcl
2024-03-08 16:53:42,533:3461(0x7ff680d75d00):ZOO_INFO@log_env@755: Client environment:user.home=/home/pcl
2024-03-08 16:53:42,534:3461(0x7ff680d75d00):ZOO_INFO@log_env@767: Client environment:user.dir=/home/pcl/cProject/projects/myPro/mpRPC/mpRPCFrame/bin
2024-03-08 16:53:42,534:3461(0x7ff680d75d00):ZOO_INFO@zookeeper_init@800: Initiating client connection, host=127.0.0.1:2181 sessionTimeout=30000 watcher=0x55b92e98cd2c sessionId=0 sessionPasswd=<null> context=(nil) flags=0
2024-03-08 16:53:42,535:3461(0x7ff67e2f3700):ZOO_INFO@check_events@1728: initiated connection to server [127.0.0.1:2181]
2024-03-08 16:53:42,562:3461(0x7ff67e2f3700):ZOO_INFO@check_events@1775: session establishment complete on server [127.0.0.1:2181], sessionId=0x18e1d43f5860002, negotiated timeout=30000
zookeeper_init success!
2024-03-08 16:53:42,565:3461(0x7ff680d75d00):ZOO_INFO@zookeeper_close@2527: Closing zookeeper sessionId=0x18e1d43f5860002 to [127.0.0.1:2181]

register success!1

```

