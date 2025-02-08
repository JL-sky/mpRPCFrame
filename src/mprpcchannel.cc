#include"mprpcapplication.h"
#include "zkutil.h"
#include<string>
#include<arpa/inet.h>
#include<unistd.h>
/*
header_size+service_name+method_name+args_size+args
*/
void MyRpcChannel::CallMethod(const google::protobuf::MethodDescriptor* method,
                    google::protobuf::RpcController* controller, 
                    const google::protobuf::Message* request,
                    google::protobuf::Message* response, 
                    google::protobuf::Closure* done)
{
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

    // 打印调试信息
    std::cout<<"============================"<<std::endl;
    std::cout<<"headerSize:"<<headerSize<<std::endl;
    std::cout<<"serviceName:"<<serviceName<<std::endl;
    std::cout<<"methodName:"<<methodName<<std::endl;
    std::cout<<"argsSize:"<<argsSize<<std::endl;
    std::cout<<"argsStr:"<<argsStr<<std::endl;
    std::cout<<"============================"<<std::endl;

    std::string methodPath="/"+serviceName+"/"+methodName;
    // 向远端rpc服务器发送消息
    sendRpcRequest(controller,sendRpcStr,response,methodPath);
}

void MyRpcChannel::sendRpcRequest(google::protobuf::RpcController* controller,
                                std::string sendRpcStr,
                                google::protobuf::Message* response,
                                std::string methodPath)
{
    // 向服务端传输字符流信息
    int clientfd=socket(AF_INET,SOCK_STREAM,0);
    if(clientfd<0)
    {
        close(clientfd);
        char errbuf[512]={0};
        sprintf(errbuf,"socket create error:%d",errno);
        controller->SetFailed(errbuf);
        return;
    }

    // std::string ip=MpRpcApplication::getInstance().getConfig().load("rpcServerIp");
    // uint16_t port=atoi(MpRpcApplication::getInstance().getConfig().load("rpcServerPort").c_str());
    
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
}