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

- protubuf：3.11.0 或 3.19.4

  - 下载

  ```bash
  https://github.com/protocolbuffers/protobuf/releases?expanded=true&page=9&q=
  ```

  - 安装

    - 解压安装包，进入解压文件夹

    - 安装基本工具

      ```bash
      sudo apt-get install autoconf automake libtool curl make g++ unzip 
      ```

    - 自动生成配置文件

      ```bash
      ./autogen.sh
      ```

    - 配置环境

      ```bash
      ./configure  
      ```

    - 源码编译

      ```bash
      sudo make
      ```

    - 安装

      ```bash
      sudo make install
      ```

    - 刷新动态库

      ```bash
      sudo ldconfig
      ```

      

- zookeeper：3.4.10 或 3.8.4

  - 下载

  ```bash
   https://dlcdn.apache.org/zookeeper/zookeeper-3.8.4/apache-zookeeper-3.8.4-bin.tar.gz 
  ```

  - 安装

  ```bash
  tar zxf apache-zookeeper-3.8.4-bin.tar.gz
  cd apache-zookeeper-3.8.4-bin/
  cd conf/
  cp zoo_sample.cfg zoo.cfg
  ```

  - 启动

  ```bash
  cd bin/
  sudo ./zkServer.sh start
  ```

  

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



## 框架使用解读

### 框架配置修改

**conf/rpcConf.conf**

以下是默认配置

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



### 服务端

#### 注册rpc节点

> 客户端想要调用服务端的方法，服务端首先得有对应的方法，因此**服务端首先需要注册对应的rpc方法**

**calleeuserservice.cc**

![image-20250208191201126](https://raw.githubusercontent.com/jl-sky/imageDatesets/master/2025/02/upgit_20250208_1739013121.png)

如图，服务端需自定义服务节点UserService，并**继承UserServiceRPC**，然后**重写rpc方法的业务逻辑**

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



#### 代码解读

##### RpcProvider类

RpcProvider  是⼀个框架专⻔为发布rpc服务的⽹络对象类。在服务端会⽤此类来注册服务，故 RpcProvider  类需要⽀持所有服务的发布。因此设计的NotifyService⽅法的参数必须要是这些服务的基类，也就是 google::protobuf::Service 。 因为protobuf只是提供了数据的序列化和反序列化还有RPC接⼝，并没有提供⽹络传输的相关代 码，所以此项⽬⽤了muduo库实现⽹络传输，同时还需要将服务注册到zookeeper上。 

![image-20250208193613133](https://raw.githubusercontent.com/jl-sky/imageDatesets/master/2025/02/upgit_20250208_1739014573.png)

```c++
//框架提供的专门提供给外部使用的，可以发布rpc方法的网络对象类
class RpcProvider
{
public:
    //在服务端注册（存储）rpc方法到_serviceMap
    void notifyService(google::protobuf::Service*service);
    //启动rpc服务,使用muduo库发布网络
    void run();
private:
    // 检测到连接或者断开事件时要做的事情
    void connectionCallback(const muduo::net::TcpConnectionPtr& conn);
    /*
    用户数据处理
    参数：
        conn：tcp连接对象
        buffer：缓冲区数据
        time：接收到数据的时间信息  
    */
    void messageCallback(const muduo::net::TcpConnectionPtr& conn,
                            muduo::net::Buffer* buffer,
                            muduo::Timestamp time
                        );
    // Closure回调，服务端处理完客户端的请求后的回调函数（将响应结果回传给客户端）
    void sendRpcResponse(const muduo::net::TcpConnectionPtr& conn,google::protobuf::Message*);

    muduo::net::EventLoop _loop;
    //服务类型信息
    typedef struct _serviceInfo
    {
        //保存服务对象
        google::protobuf::Service* _service;
        //保存服务方法
        unordered_map<std::string,const google::protobuf::MethodDescriptor*> _methodMap;
    }_serviceInfo;

    //存储注册成功的服务对象和其服务方法的信息
    unordered_map<std::string,_serviceInfo> _serviceMap;
};
```



-  **NotifyService**

> **notifyService**用于**注册rpc节点和方法**（也就是**UserServiceRPC中重写UserService的方法**），并将其保存到**_serviceMap**中
>
> - 从 *service  获取服务对象的描述信息，此接⼝由protobuf提供。
> - 从描述信息中获取到服务名字和服务对象service的⽅法和数量。
> - 遍历 service  获取服务对象指定虾苗的服务⽅法描述，并将其注册到 例如 FriendServiceRpc/GetFriendsList  .  
> - 最后将其加⼊服务对象集合 _serviceMap  中。 _serviceMap的结构为：
>
> ```c++
> /*
> _serviceMap用于存储注册的rpc节点及其节点内的所有方法，其结构如下
> {
> serviceName:{
>                 service,
>                 {methodName:pmethodDesc}
>             },
>         ...
> serviceName:{
>                 service,
>                 {methodName:pmethodDesc}
>             }
> }
> */
> ```
>



- **connectionCallback**：用于**监听网络的连接和断开**事件
- **messageCallback**：用于**监听并处理网络数据**，其主要逻辑为

> 1. **接收客户端请求**
>
>    - 客户端发送的请求数据通过网络到达服务端，服务端通过Socket接收数据。
>    - 由于**TCP是流式协议，可能会出现粘包问题**，因此需要**自定义协议**来解决。本项目采用的消息格式为：**消息头 + 消息体**，其中消息头包含消息的总长度。
>
> 2. **解析消息**
>
>    - 从接收到的数据中**先读取前四个字节**，这四个字节**表示消息体的长度**（`header_size`）。
>    - 根据`header_size`**读取消息头的原始字符串**，并反序列化消息头，**得到RPC请求的详细信息，包括服务名、方法名和参数。**
>
> 3. **调用RPC方法**
>
>    - 在服务注册表（`_serviceMap`）中，**根据客户端提供的服务名和方法名，查询服务端注册的服务对象和方法描述符。**
>
>    - **根据解析得到的服务名和方法名，生成RPC方法调用请求（`request`）和相应的响应（`response`）参数。**
>
>    - 调用`callMethod`函数，**callMethod会根据方法描述符调用Service中对应的方法**。
>
>      ```c++
>          pService->CallMethod(pMethod,nullptr,request,response,done);
>          /*
>      void UserServiceRPC::CallMethod(const ::PROTOBUF_NAMESPACE_ID::MethodDescriptor* method,
>                                   ::PROTOBUF_NAMESPACE_ID::RpcController* controller,
>                                   const ::PROTOBUF_NAMESPACE_ID::Message* request,
>                                   ::PROTOBUF_NAMESPACE_ID::Message* response,
>                                   ::google::protobuf::Closure* done) {
>        GOOGLE_DCHECK_EQ(method->service(), file_level_service_descriptors_user_2eproto[0]);
>        switch(method->index()) {
>          case 0:
>            login(controller,
>                   ::PROTOBUF_NAMESPACE_ID::internal::DownCast<const ::mpRpc::LoginRequest*>(
>                       request),
>                   ::PROTOBUF_NAMESPACE_ID::internal::DownCast<::mpRpc::LoginResponse*>(
>                       response),
>                   done);
>            break;
>          case 1:
>            reg(controller,
>                   ::PROTOBUF_NAMESPACE_ID::internal::DownCast<const ::mpRpc::RegisterRquest*>(
>                       request),
>                   ::PROTOBUF_NAMESPACE_ID::internal::DownCast<::mpRpc::RegisterResponse*>(
>                       response),
>                   done);
>            break;
>          default:
>            GOOGLE_LOG(FATAL) << "Bad method index; this should never happen.";
>            break;
>        }
>      }
>          */
>      ```
>
>      
>
>    - 在**方法调用结束后，CallMethod会回调`sendRpcResponse`函数将响应结果发送回客户端**。

- **sendRpcResponse**：CallMethod方法调用结束后的回调函数，也就是当CallMethod处理完数据后进行调用的函数，在这里的业务逻辑是将处理结果封装后回传给客户端

```c++
void RpcProvider::notifyService(google::protobuf::Service*service)
{
    // 获取服务对象的描述信息
    const google::protobuf::ServiceDescriptor* pserviceDesc=service->GetDescriptor();
    // 获取服务名称
    std::string serviceName=pserviceDesc->name();
    // service_name:UserServiceRPC
    LOG_INFO("service_name:%s",serviceName.c_str());

    _serviceInfo serviceInfo;
    serviceInfo._service=service;
    // 获取服务中方法的数量
    const int methodCnt=pserviceDesc->method_count();
    // 取出rpc服务节点下的所有方法
    for(int i=0;i<methodCnt;i++)
    {
        //获取指定服务方法的描述
        const google::protobuf::MethodDescriptor* pmethodDesc=pserviceDesc->method(i);
        std::string methodName=pmethodDesc->name();
        serviceInfo._methodMap.insert({methodName,pmethodDesc});
        LOG_INFO("method_name:%s",methodName.c_str());
    }
    _serviceMap.insert({serviceName,serviceInfo});
}
```



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

如图所示，客户端在调用rpc方法之前需要首先使用**MyRpcChannel**初始化**UserServiceRPC_Stub**对象

然后使用**UserServiceRPC_Stub**对象调用服务端的rpc方法（login等）

> 需要说明的是，**UserServiceRPC_Stub**对象中的**所有rpc方法最终都调用的是MyRpcChannel中的CallMethod方法**

![image-20250208201927201](https://raw.githubusercontent.com/jl-sky/imageDatesets/master/2025/02/upgit_20250208_1739017167.png)

#### MprpcChannel  模块

MprpcChannel  模块继承于 google::protobuf::RpcChannel  他是⼀个RPC通道的抽象 接⼝，表示⼀个到服务的通信线路，这个线路⽤于客户端远程调⽤服务端的⽅法。我们需要继承这 个类并重写他的 CallMethod  ⽅法。

```c++
 class PROTOBUF_EXPORT RpcChannel {
 public:
 inline RpcChannel() {}
 virtual ~RpcChannel();
 C++
 virtual void CallMethod(const MethodDescriptor* method,
 RpcController* controller, const Message* reques
 t,
 Message* response, Closure* done) = 0;
 private:
 GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(RpcChannel);
 };
```

 **MprpcChannel 类**

```c++
class MyRpcChannel:public google::protobuf::RpcChannel
{
public:
    void CallMethod(const google::protobuf::MethodDescriptor* method,
                    google::protobuf::RpcController* controller, 
                    const google::protobuf::Message* request,
                    google::protobuf::Message* response, 
                    google::protobuf::Closure* done);
    void sendRpcRequest(google::protobuf::RpcController* controller,
                        std::string sendRpcStr,
                        google::protobuf::Message* response,
                        std::string methodPath);
};
```

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

**CallMethod**：所有通过stub代理对象调⽤的RPC⽅法都⾛到了这⾥，统⼀做RPC⽅法调⽤的数据序列化和⽹络发送；封装服务名、方法名和请求参数，然后发送给服务端，接下来服务端就根据服务名和方法名找到对应的方法，然后调用

**获取客户端请求的⽅法和序列化**

> 1. 从 CallMethod的参数 method  获取 service_name  和 method_name  ; 
> 2. 将获取到的参数序列化为字符串，并获取他的⻓度。
> 3. 定义RPC的请求 header  . 
> 4. 组织待发送的RPC请求的字符串 

```c++
    const google::protobuf::ServiceDescriptor* sd=method->service();
    std::string serviceName=sd->name();
    std::string methodName=method->name(); 
    // 获取参数序列长度
    int argsSize=0;
    std::string argsStr;
    if(request->SerializeToString(&argsStr))
    {
        argsSize=argsStr.size();
    }else{
        controller->SetFailed("serialize request error");
    }
    //封装rpc请求头
    mprpcHeader::RpcHeader rpcHeader;
    rpcHeader.set_servicename(serviceName);
    rpcHeader.set_methodname(methodName);
    rpcHeader.set_argssize(argsSize);

    uint32_t headerSize=0;
    std::string rpcHeaderStr;
    if(rpcHeader.SerializeToString(&rpcHeaderStr))
    {
        headerSize=rpcHeaderStr.size();
    }
    else
    {
        // std::cout<<rpcHeaderStr<<std::endl;
        controller->SetFailed("serialize rpcHeaderStr error");
        return ;
    }

    // 组织需要发送的字符串
    std::string sendRpcStr;
    //固定在sendRpcStr的前四个字节处写入headerSize信息
    /*
    std::string((char *)&headerSize, 4):
    - 将内存地址 &headerSize 强制转换为 char* 类型，并创建了一个 std::string 对象。
    - 其中第二个参数 4 表示 std::string 要从这个指针开始，读取 4 个字节的数据。
        表示读取 headerSize 所占用的前 4 个字节并将其作为字符串来存储。
    
    因此sendRpcStr.insert()就表示:
    将上面创建的字符串插入到 sendRpcStr 的开头（位置 0）
    */
    sendRpcStr.insert(0,std::string((char *)&headerSize,4));
    sendRpcStr+=rpcHeaderStr;
    sendRpcStr+=argsStr;
```

**使⽤TCP编程完成RPC⽅法的远程调⽤**

> 因为 CallMethod  ⽅法⽤于客户端远程调⽤服务端的⽅法，考虑到这⾥不需要⾼并发，故没有 使⽤muduo⽹络库。
>
> 1. 通过 socket 函数在内核中创建⼀个套接字 
> 2. RPC调⽤⽅法想要调⽤ service_name  的 method_name  服务，需要到zookeeper上查询 该服务的所在的 host  信息。 
> 3. 查询到了 mathod_name  服务的IP和PORT后，连接RPC服务结点  
> 4. 发送RPC请求  
> 5. 接收RPC请求的响应值  
> 6. 最后反序列化服务器发回来的响应数据

```c++
    int clientfd=socket(AF_INET,SOCK_STREAM,0);
    if(clientfd<0)
    {
        close(clientfd);
        char errbuf[512]={0};
        sprintf(errbuf,"socket create error:%d",errno);
        controller->SetFailed(errbuf);
        return;
    }

```

```c++
    ZkClient zkc;
    zkc.start();
    std::string hostData=zkc.getData(methodPath.c_str());
    if(hostData=="")
    {
        controller->SetFailed(methodPath+" is not exists!");
        return;
    }
    int idx=hostData.find(":");
    if(idx==-1)
    {
        controller->SetFailed(methodPath+" is invalied!");
        return;
    }

    std::string ip=hostData.substr(0,idx);
    uint32_t port=atoi(hostData.substr(idx+1,hostData.size()-idx).c_str());

```

```c++
    // 封装rpc服务端地址信息
    struct sockaddr_in seraddr;
    seraddr.sin_family=AF_INET;
    seraddr.sin_addr.s_addr=inet_addr(ip.c_str());
    seraddr.sin_port=htons(port);

    // 连接rpc服务器节点
    if(connect(clientfd,(struct sockaddr*)&seraddr,sizeof(seraddr))<0)
    {
        close(clientfd);
        char errbuf[512]={0};
        sprintf(errbuf,"connect rpcServer error:%d",errno);
        controller->SetFailed(errbuf);
        return ;
    }
    // 发送信息
    if(send(clientfd,sendRpcStr.c_str(),sendRpcStr.size(),0)<0)
    {
        close(clientfd);
        char errbuf[512]={0};
        sprintf(errbuf,"send message error:%d",errno);
        controller->SetFailed(errbuf);
        return ;
    }

    //接收rpc服务端的响应
    char* recvbuf[1024]={0};
    int recv_size=0;
    if((recv_size=recv(clientfd,recvbuf,1024,0))==-1)
    {
        close(clientfd);
        char errbuf[512]={0};
        sprintf(errbuf,"recv error:%d",errno);
        controller->SetFailed(errbuf);
        return;
    }

    // 解析响应字符串，并通过response字符串传出给客户端
    if(!response->ParseFromArray(recvbuf,recv_size))
    {
        close(clientfd);
        char errbuf[512]={0};
        sprintf(errbuf,"parse error! responseStr:%s",recvbuf);
        controller->SetFailed(errbuf);
        return;
    }
    close(clientfd);
```

## 日志模块

异步⽇志与队列

- 对于异步⽇志我们借助队列来实现：**⼯作线程写⽇志到队列，队列本身使⽤条件变量为 通知机制，当有数据⼊队列时就通知消费者线程去消费⽇志，此时再写入磁盘**，模型如下：

![image-20250209110033347](https://raw.githubusercontent.com/jl-sky/imageDatesets/master/2025/02/upgit_20250209_1739070033.png)

```c++
//在另一个线程中，将日志信息写入到文件中
Logger::Logger()
{
    std::thread writeLogTask(
        [&]()
        {
            while(1)
            {
                //获取当天日期
                time_t now=time(nullptr);
                tm *nowtm=localtime(&now);
                char filename[512]={0};
                sprintf(filename,
                        "%d-%d-%d-log.txt",
                        nowtm->tm_year+1900,nowtm->tm_mon+1,nowtm->tm_mday);

                char path[1024];
                if (getcwd(path, sizeof(path)) == NULL)
                {
                    LOG_ERR("Failed to get current working directory!");
                    return false;
                }
                //获取项目根目录
                std::string curPath=std::string(path);
                int idx=curPath.find_last_of("/");
                std::string str=curPath.substr(idx+1);
                if(str!="mpRPCFrame")
                {
                    curPath=curPath.substr(0,idx);
                }
                std::string logPath=curPath+"/log/"+std::string(filename);
                // std::cout<<"logPath:"<<logPath<<std::endl;
                FILE* pf=fopen(logPath.c_str(),"a+");
                if(!pf)
                {
                    std::cout<<"log file "<<filename<<" open error!"<<std::endl;
                    exit(EXIT_FAILURE);
                }
                // 从日志缓冲队列中取出日志信息
                std::string logMsg=_lckQueue.pop();
                //在日志信息中添加时间信息
                char timebuf[512]={0};
                sprintf(timebuf,
                        "%d-%d-%d=>%s:",
                        nowtm->tm_hour,nowtm->tm_min,nowtm->tm_sec,
                        (_logLevel==INFO?"info":"error")
                        );
                
                logMsg.insert(0,timebuf);
                logMsg.append("\n");
                
                // 写入日志信息
                fputs(logMsg.c_str(),pf);
                fclose(pf);
            }
        }
    );
    writeLogTask.detach();
}

//写日志,将日志信息写入到日志队列缓冲区中
void Logger::log(std::string msg)
{
    _lckQueue.push(msg);
}
```





- 另外，日志系统可以缓存一些状态信息（如日志文件的文件描述符、缓冲区等），为了减少重复的初始化和资源分配操作，提高日志记录的性能。需要将**日志系统设置为单例**

  ```c++
  Logger& Logger::getInstance()
  {
      static Logger logger;
      return logger;
  }
  ```

  

- 线程安全的队列

  ```c++
  template <typename T>
  class LockQueue
  {
  public:
      void push(const T& data)
      {
          std::lock_guard<std::mutex> lock(_qmutex);
          _queue.push(data);
          _qcondvariable.notify_one();
      }
      T pop()
      {
          std::unique_lock<std::mutex> lock(_qmutex);
          while(_queue.empty())
          {
              _qcondvariable.wait(lock);
          }
          T data=_queue.front();
          _queue.pop();
          return data;
      }
  private:
      std::queue<T> _queue;
      std::mutex _qmutex;
      std::condition_variable _qcondvariable;
  };
  ```

  

## ZooKeeper配置管理中心

### 概述

 ZooKeeper是一个分布式服务框架，为分布式应用提供一致性协调服务的中间件。

> 在这个项目中，callee将【对外提供的服务对象及其方法】以及【网络地址信息】注册在ZooKeeper服务上，caller则通过访问ZooKeeper在整个分布式环境中获取自己想要调用的远端服务对象方法【在哪一台设备上（网络地址信息）】，并向该设备直接发送服务方法调用请求。



  ZooKeeper提供了两个库，分别是zookeeper_st（单线程库）和zookeeper_mt（多线程库），一般使用zookeeper_mt库提供的API。
 采用zookeeper_mt库的zookeeper客户端使用了三个线程：

> - 主线程：用户调用API的线程。
>
>
> - IO线程：负责网络通信的线程。
>
>
> - completion线程：对于异步请求（Zookeeper中提供的异步API，一般都是以zoo_a开头的api）以及watcher的响应回调，io线程会发送给completion线程完成处理。关于watcher机制后面会介绍。
>

ZooKeeper是怎么存储数据（服务对象和方法）？

 ZooKeeper相当于是一个特殊的文件系统，不过和普通文件系统不同的是，这些节点都可以设置关联的数据，而文件系统中只有文件节点可以存放数据，目录节点不行。ZooKeeper内部为了保持高吞吐和低延迟，再内存中维护了一个树状的目录结构，这种特性使ZooKeeper不能存放大量数据，每个节点存放数据的上线为1M。
![image-20250209111114362](https://raw.githubusercontent.com/jl-sky/imageDatesets/master/2025/02/upgit_20250209_1739070674.png)

### watcher机制

**watcher机制就是ZooKeeper客户端对某个znode建立一个watcher事件，当该znode发生变化时，这些ZK客户端会收到ZK服务端的通知，然后ZK客户端根据znode的变化来做出业务上的改变。**

Watcher实现由三个部分组成：

```bash
         1. ZooKeeper服务端 
         2. ZooKeeper客户端（callee）
         3. 客户端的ZkWatchManager对象
```

Watcher机制类似于设计模式中的**观察者模式**，也可以看作是观察者模式在分布式场景下的实现方式。

> - 针对每一个znode的操作，都会有一个watcher。
> - 当监控的某个对象（znode）发生了变化，则触发watcher事件。父节点、子节点增删改都能触发其watcher。
> - ZooKeeper中的watcher是一次性的，触发后立即销毁。
>    

ZooKeeper客户端（Callee）首先将Watcher注册到服务端，同时把Watcher对象保存到客户端的Watcher管理器中。当ZooKeeper服务端监听到ZooKeeper中的数据状态发生变化时，服务端主动通知客户端（告知客户端事件类型和状态类型），接着客户端的Watch管理器会触发相关Watcher来回调相应处理逻辑（GlobalWatcher），从而完成整体的数据发布/订阅流程。

![image-20250209113852940](https://raw.githubusercontent.com/jl-sky/imageDatesets/master/2025/02/upgit_20250209_1739072333.png)
