#include "rpcprovider.h"
#include<string>
#include "rpcheader.pb.h"
#include "zkutil.h"

//服务端需要注册（存储）服务（类），将来客户端进行对这些服务进行查询调用
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

// rpc网络模块
void RpcProvider::run()
{
    //封装服务端地址
    std::string ip=MpRpcApplication::getInstance().getConfig().load("rpcServerIp");
    uint16_t port=atoi(MpRpcApplication::getInstance().getConfig().load("rpcServerPort").c_str());
    muduo::net::InetAddress rpcProInetAdd(ip,port);
    //创建muduo服务器对象
    muduo::net::TcpServer server(&_loop,rpcProInetAdd,"rpcProvider");
    
    //设置网络连接建立事件回调
    server.setConnectionCallback(std::bind(
        &RpcProvider::connectionCallback,this,
        std::placeholders::_1));
    //设置网络连接读写事件回调
    server.setMessageCallback(std::bind(
        &RpcProvider::messageCallback,this,
        std::placeholders::_1,std::placeholders::_2,std::placeholders::_3));

    // std::cout<<"rpcProvier server start!"<<std::endl;
    LOG_INFO("rpcProvier server start!");
    //一个IO线程，3个工作线程
    server.setThreadNum(4);

    // 将发布的rpc服务注册到zk上，从而使得rpc客户端可以在zk上发现服务
    ZkClient zkc;
    zkc.start();
    for(auto &sp:_serviceMap)
    {
        std::string serviceName=sp.first;
        std::string servicePath="/"+serviceName;
        // 创建永久性对象节点，即使节点挂掉也不会被删除
        zkc.createNode(servicePath.c_str(),nullptr,0);
        for(auto &mp:sp.second._methodMap)
        {
            std::string methodPath="/"+serviceName+"/"+mp.first;
            char databuffer[128]={0};
            sprintf(databuffer,"%s:%d",ip.c_str(),port);
            // ZOO_EPHEMERAL表示方法节点是一个临时节点
            zkc.createNode(methodPath.c_str(),databuffer,strlen(databuffer),ZOO_EPHEMERAL);
        }
    }


    server.start();
    _loop.loop();
}

void RpcProvider::connectionCallback(const muduo::net::TcpConnectionPtr& conn)
{
    if(!conn->connected())
    {
        conn->shutdown();
    }
}

/*
1.解析客户端要调用的服务名、方法名和参数
2.服务端将根据客户端的服务名、方法名和参数在_serviceMap中查询服务端注册的服务和方法
3.callMethod调用对应的服务方法（根据方法索引判断调用login还是reg），然后在调用结束之后回调sendRpcResponse
*/
void RpcProvider::messageCallback(const muduo::net::TcpConnectionPtr& conn,
                            muduo::net::Buffer* buffer,
                            muduo::Timestamp time)
{
/*=================== 解析客户端发送请求数据(buffer) =====================*/
/*
网络数据：
headerSize(4B存储)+headerStr(serviceName+methodName+argsSize)+argsStr
*/
    /*将接受到的网络数据转换为string，
    然后从字符流中抽取远程服务（UserServiceRPC）及其方法名（login）和参数信息
    */
    std::string recv_buf=buffer->retrieveAllAsString();
    // 从recv_buf读取前4个字节的数据，并将其作为字节流复制到headerSize
    // &headerSize，获取 headerSize 变量的地址，并将其类型转换为 char*，即字节指针
    uint32_t headerSize=0;
    recv_buf.copy((char*)&headerSize,4,0);

    // 读取网络字节头部信息
    std::string headerStr=recv_buf.substr(4,headerSize);
    mprpcHeader::RpcHeader rpcHeader;
    // 字符串反序列化
    std::string serviceName;
    std::string methodName;
    int argsSize;
    if(rpcHeader.ParseFromString(headerStr))
    {
        serviceName=rpcHeader.servicename();
        methodName=rpcHeader.methodname();
        argsSize=rpcHeader.argssize();
    }
    else//反序列化失败
    {
        // std::cout<<"headerStr:"<<headerStr<<std::endl;
        LOG_ERR("headerStr:%s",headerStr.c_str());
        return ;
    }

    std::string args=recv_buf.substr(4+headerSize,argsSize);
    
    std::cout<<"============================"<<std::endl;
    std::cout<<"headerSize:"<<headerSize<<std::endl;
    std::cout<<"headerStr:"<<headerStr<<std::endl;
    std::cout<<"serviceName:"<<serviceName<<std::endl;
    std::cout<<"methodName:"<<methodName<<std::endl;
    std::cout<<"argsSize:"<<argsSize<<std::endl;
    std::cout<<"args:"<<args<<std::endl;
    std::cout<<"============================"<<std::endl;

/*=================== 根据客户端的请求数据，找到服务端注册的服务对象、方法对象 =====================*/

    // 获取服务对象和方法对象
    // 其中一个服务对应多个方法，首先根据服务名找到对应的服务，然后再根据该服务找到方法
    auto mit=_serviceMap.find(serviceName);
    if(mit==_serviceMap.end())
    {
        // std::cout<<serviceName<<" is not exist!"<<std::endl;
        LOG_ERR("%s is not exist!",serviceName.c_str());
        return ;
    }
    // 获取服务对象
    google::protobuf::Service* pService=mit->second._service;

    auto it=mit->second._methodMap.find(methodName);
    if(it==mit->second._methodMap.end())
    {
        // std::cout<<serviceName<<":"<<methodName<<" is not exist!"<<std::endl;
        LOG_ERR("%s:%s is not exist!",serviceName.c_str(),methodName.c_str());
        return;
    }
    // 获取服务对应下的方法对象
    const google::protobuf::MethodDescriptor* pMethod=it->second;

/*=================== 以下在框架上根据客户端rpc请求，调用当前rpc节点(UserService)上注册的方法(pMethod) =====================*/
    /*
    void login(::google::protobuf::RpcController* controller,
                       const ::mpRpc::LoginRequest* request,
                       ::mpRpc::LoginResponse* response,
                       ::google::protobuf::Closure* done)
    */
    // 生成rpc调用需要的request参数与和response参数
    // 生成指定rpc方法的请求request对象
    google::protobuf::Message *request=pService->GetRequestPrototype(pMethod).New();
    // 生成rpc请求参数（将参数字符串重新封装为pb对象，供login函数调用）
    if(!request->ParseFromString(args))
    {
        // std::cout<<"request args parse error!"<<std::endl;
        LOG_ERR("request args parse error!");
        return;
    }
    // 生成指定rpc方法的响应消息对象
    google::protobuf::Message *response=pService->GetResponsePrototype(pMethod).New();
    /*
    template <typename Class, typename Arg1, typename Arg2>
    inline Closure* NewCallback(Class* object, void (Class::*method)(Arg1, Arg2),
                                Arg1 arg1, Arg2 arg2)
    */
    
    /*
    给下⾯的CallMethod的⽅法的调⽤
    绑定⼀个Closure的自定义回调函数，设置服务端处理完客户端请求后要做的事情
    在这里我们设置为将响应信息回传给客户端
    */
    google::protobuf::Closure *done=google::protobuf::NewCallback<RpcProvider,
                                const muduo::net::TcpConnectionPtr&,
                                google::protobuf::Message*>
                                            (this,
                                            &RpcProvider::sendRpcResponse,
                                            conn,
                                            response);

    /*
    在框架上根据客户端rpc请求，调用当前rpc节点上注册的方法
    new UserService().login(controller,request,done)
    */
    pService->CallMethod(pMethod,nullptr,request,response,done);
    /*
void UserServiceRPC::CallMethod(const ::PROTOBUF_NAMESPACE_ID::MethodDescriptor* method,
                             ::PROTOBUF_NAMESPACE_ID::RpcController* controller,
                             const ::PROTOBUF_NAMESPACE_ID::Message* request,
                             ::PROTOBUF_NAMESPACE_ID::Message* response,
                             ::google::protobuf::Closure* done) {
  GOOGLE_DCHECK_EQ(method->service(), file_level_service_descriptors_user_2eproto[0]);
  switch(method->index()) {
    case 0:
      login(controller,
             ::PROTOBUF_NAMESPACE_ID::internal::DownCast<const ::mpRpc::LoginRequest*>(
                 request),
             ::PROTOBUF_NAMESPACE_ID::internal::DownCast<::mpRpc::LoginResponse*>(
                 response),
             done);
      break;
    case 1:
      reg(controller,
             ::PROTOBUF_NAMESPACE_ID::internal::DownCast<const ::mpRpc::RegisterRquest*>(
                 request),
             ::PROTOBUF_NAMESPACE_ID::internal::DownCast<::mpRpc::RegisterResponse*>(
                 response),
             done);
      break;
    default:
      GOOGLE_LOG(FATAL) << "Bad method index; this should never happen.";
      break;
  }
}
    */
}

// Closure回调，用于序列化rpc的响应和网络发送
void RpcProvider::sendRpcResponse(const muduo::net::TcpConnectionPtr& conn,
                                    google::protobuf::Message* response)
{
    std::string responseStr;
    //将响应数据序列化为字符串
    if(response->SerializeToString(&responseStr))
    {
        conn->send(responseStr);
    }
    else{
        // std::cout<<"response str serialize error!"<<std::endl;
        LOG_ERR("response str serialize error!");
    }
    conn->shutdown();
}