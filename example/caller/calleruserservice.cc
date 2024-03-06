#include<iostream>
#include "mprpcapplication.h"
#include "user.pb.h"
#include "mprpcchannel.h"

int main(int argc,char** argv)
{
    // 初始化rpc框架,即加载rpc框架配置文件
    MpRpcApplication::Init(argc,argv);
    mpRpc::UserServiceRPC_Stub stub(new MyRpcChannel());

    // rpc方法请求参数
    mpRpc::LoginRequest request;
    request.set_name("zhangsan");
    request.set_passwd("123456");
    /*
    std::string argsStr;
    if(request.SerializeToString(&argsStr))
    {
        std::cout<<argsStr<<std::endl;
    }
    */
    // rpc响应，传出参数
    mpRpc::LoginResponse response;
    //记录远程调用的状态信息
    MyRpcController myController;
    // rpc远程调用，底层调用MyChannel中的callMethod方法（纯虚函数）
    stub.login(&myController,&request,&response,nullptr);

    if(myController.Failed())
    {
        std::cout<<myController.ErrorText()<<std::endl;
    }else
    {
        if((response.result().errcode())==0)
        {
            std::cout<<"rpc login response success!"<<response.success()<<std::endl;
        }else{
            std::cout<<"rpc login response error!"<<response.result().errmsg()<<std::endl;
        }
    }

    mpRpc::RegisterRquest regRequest;
    regRequest.set_id(1);
    regRequest.set_name("李四");
    regRequest.set_passwd("666666");
    mpRpc::RegisterResponse regResponse;
    stub.reg(nullptr,&regRequest,&regResponse,nullptr);

    if(myController.Failed())
    {
        std::cout<<myController.ErrorText()<<std::endl;
    }else
    {
        if((regResponse.result().errcode())==0)
        {
            std::cout<<"register success!"<<regResponse.success()<<std::endl;
        }
    }
    return 0;
}