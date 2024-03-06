#include<iostream>
#include<string>
#include"user.pb.h"
#include"mprpcapplication.h"
#include "rpcprovider.h"
using namespace mpRpc;
using namespace std;

class UserService:public UserServiceRPC
{
public:
    //本地方法
    //登录
    bool login(string name,string passwd)
    {
        cout<<"doing login service"<<endl;
        if(name=="zhangsan" && passwd=="123456")
        {
            cout<<"login success!\t"<<"name:"<<name<<" passwd:"<<passwd<<endl;
            return true;
        }else{
            std::cout<<"login error:"<<name<<" passwd:"<<passwd<<std::endl;
            return false;
        }
    }

    //注册
    bool reg(uint32_t id,string name,string passwd)
    {
        cout<<"doing register serivice!"<<endl;
        cout<<"login success!\tname:"<<name<<" passwd:"<<passwd<<endl;
        return true;
    }

    //rpc框架方法
    //从网络中收到login的参数值后解析给本地login函数，并将结果返回给rpc
    void login(::google::protobuf::RpcController* controller,
                       const ::mpRpc::LoginRequest* request,
                       ::mpRpc::LoginResponse* response,
                       ::google::protobuf::Closure* done)
    {
        // 获取客户端的调用参数
        string name=request->name();
        string passwd=request->passwd();
        // 使用客户端的参数调用本地服务端的对应方法
        bool isLogin=login(name,passwd);
        //封装响应
        resultErrcode* pc=response->mutable_result();
        if(isLogin)
        {
            pc->set_errcode(0);
            pc->set_errmsg("");
            response->set_success(1);
        }else{
            pc->set_errcode(1);
            pc->set_errmsg("login error!");
            response->set_success(0);
        }
        //响应数据序列化与网络发送
        done->Run();
    }

    void reg(::google::protobuf::RpcController* controller,
                       const ::mpRpc::RegisterRquest* request,
                       ::mpRpc::RegisterResponse* response,
                       ::google::protobuf::Closure* done)
    {
        uint32_t id=request->id();
        string name=request->name();
        string passwd=request->passwd();

        bool ret=reg(id,name,passwd);
        mpRpc::resultErrcode*pc=response->mutable_result();
        pc->set_errcode(0);
        pc->set_errmsg("");
        response->set_success(1);
        done->Run();
    }
};

int main(int argc,char** argv)
{
    //初始化rpc框架，加载rpc框架配置文件
    MpRpcApplication::Init(argc,argv);

    // RpcProvider即为rpc服务端
    RpcProvider provider;
    //注册rpc服务相关信息
    provider.notifyService(new UserService());
    //启动rpc服务节点，开始提供rpc远程调用服务
    provider.run();
    return 0;
}
