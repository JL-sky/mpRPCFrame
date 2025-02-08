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

进入zookeeper安装目录，然后进入bin目录，执行

```bash
./zkServer.sh start
```

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

## 代码解读

### 服务端

#### calleeuserservice.cc

> 客户端想要调用服务端的方法，服务端首先得有对应的方法，因此服务端首先需要注册对应的rpc方法

如图，服务端需自定义UserService，并继承**UserServiceRPC**，然后**重写rpc方法的业务逻辑**

> 注意，在UserServiceRPC中，通用框架方法**CallMethod()**会根据**方法描述符调用对应的方法（login和reg函数）**，
>
> 因此框架的**本质是获取到对应的方法描述符后调用CallMethod()**，即可调用对应的方法（此时的方法已经被用户进行业务重写）

然后在main中注册自定义的UserService

```c++
    // RpcProvider即为rpc服务端
    RpcProvider provider;
    //注册rpc服务相关信息
    provider.notifyService(new UserService());
    //启动rpc服务节点，开始提供rpc远程调用服务
    provider.run();
```

![image-20250208191201126](https://raw.githubusercontent.com/jl-sky/imageDatesets/master/2025/02/upgit_20250208_1739013121.png)

#### rpcprovider.cc

![image-20250208193613133](https://raw.githubusercontent.com/jl-sky/imageDatesets/master/2025/02/upgit_20250208_1739014573.png)

如图所示，是rpcprovider的类图，其中

- **notifyService**：用于**注册rpc节点和方法**（也就是**UserServiceRPC中重写UserService的方法**），并将其保存到**_serviceMap**中，_serviceMap的结构为

```c++
/*
_serviceMap用于存储注册的rpc节点及其节点内的所有方法，其结构如下
{
serviceName:{
                service,
                {methodName:pmethodDesc}
            },
        ...
serviceName:{
                service,
                {methodName:pmethodDesc}
            }
}
*/
```

- **connectionCallback**：用于**监听网络的连接和断开**事件
- **messageCallback**：用于处理网络数据，其主要逻辑为

> 1.解析客户端发送的网络数据，即调用的**服务名、方法名和参数**
>
> 2.**根据客户端的服务名、方法名在_serviceMap中查询服务端注册的服务（rpc节点）和方法（方法描述符）**
>
> 3.**调用callMethod，callMethod会根据方法描述符调用对应的方法**（根据方法索引判断调用login还是reg），然后在调用结束之后回调sendRpcResponse

- **sendRpcResponse**：CallMethod方法调用结束后的回调函数，也就是当CallMethod处理完数据后进行调用的函数，在这里的业务逻辑是将处理结果封装后回传给客户端

### 客户端

**calleruserservice.cc**

客户端调用rpc方法的流程如下

```c++
    MpRpcApplication::Init(argc,argv);
    mpRpc::UserServiceRPC_Stub stub(new MyRpcChannel());
    //封装请求
    mpRpc::LoginRequest request;
    request.set_name("zhangsan");
    request.set_passwd("123456");
    // rpc响应，传出参数
    mpRpc::LoginResponse response;
    //记录远程调用的状态信息
    MyRpcController myController;
    // rpc远程调用，底层调用MyChannel中的callMethod方法（纯虚函数）
    stub.login(&myController,&request,&response,nullptr);
```

如下图所示，客户端在调用rpc方法之前需要首先使用**MyRpcChannel**初始化**UserServiceRPC_Stub**对象

然后使用**UserServiceRPC_Stub**对象调用rpc方法

> 需要说明的是，**UserServiceRPC_Stub**对象中的**所有rpc方法最终都调用的是MyRpcChannel中的CallMethod方法**

![image-20250208201927201](https://raw.githubusercontent.com/jl-sky/imageDatesets/master/2025/02/upgit_20250208_1739017167.png)

**mprpcchannel.cc**

![image-20250208202807592](https://raw.githubusercontent.com/jl-sky/imageDatesets/master/2025/02/upgit_20250208_1739017687.png)

如图所示，客户端所有rpc方法的调用都最终转到的rpcchannel中的**CallMethod**方法

```c++
void UserServiceRPC_Stub::login(::PROTOBUF_NAMESPACE_ID::RpcController* controller,
                              const ::mpRpc::LoginRequest* request,
                              ::mpRpc::LoginResponse* response,
                              ::google::protobuf::Closure* done) {
  channel_->CallMethod(descriptor()->method(0),
                       controller, request, response, done);
}
```

**CallMethod**：封装服务名、方法名和请求参数，然后发生给服务端，接下来服务端就根据服务名和方法名找到对应的方法，然后调用
