#pragma once
#include"mprpcconf.h"
#include "mprpcchannel.h"
#include "mprpccontroller.h"

//mprpc框架的基础类，用于初始化框架
class MpRpcApplication
{
public:
    // void showArgsHelp();
    static void Init(int argc,char** argv);
    static MpRpcApplication& getInstance();
    static MpRpcConf& getConfig();

private:
    MpRpcApplication(){}
    MpRpcApplication(const MpRpcApplication&)=delete;
    MpRpcApplication(const MpRpcApplication&&)=delete;
    
    // rpc配置文件加载类
    static MpRpcConf _configMsg;
};