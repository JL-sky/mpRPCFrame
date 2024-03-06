#include<iostream>
#include "mprpcapplication.h"
#include<unistd.h>
#include<string>
using namespace std;

MpRpcConf MpRpcApplication::_configMsg{};
MpRpcApplication& MpRpcApplication::getInstance()
{
    MpRpcApplication app;
    return app;
}

void showArgsHelp()
{
    cout<<"format:command -i <configfile>"<<endl;
}

void MpRpcApplication::Init(int argc,char** argv)
{
    if(argc<2)
    {
        showArgsHelp();
        exit(EXIT_FAILURE);
    }
    //用于存放 getopt() 函数返回的命令行选项字符。
    //当 getopt() 函数执行完毕后，它会返回下一个选项字符，
    //直到所有选项都被处理完毕，此时返回 -1。
    int c=0;
    string configFile;//配置文件路径（名称）
    //opt框架解析命令行参数,冒号 : 用于指示选项i后需要一个参数
    while((c=getopt(argc,argv,"i:"))!=-1)
    // while((c=getopt(argc,argv,"i"))!=-1)
    {
        switch (c)
        {
            case 'i':
                configFile=optarg;//optarg为选项i后的参数
                break;
            case '?':
                cout<<"invalid args!"<<endl;
                showArgsHelp();
                exit(EXIT_FAILURE);
            case ':':
                showArgsHelp();
                exit(EXIT_FAILURE);
            default:
                break;
        }
    }

    //开始加载配置文件
    _configMsg.loadConfigFile(configFile.c_str());
    /*
    cout<<"rpcServerIp:"<<_configMsg.load("rpcServerIp")<<endl;
    cout<<"rpcServerPort:"<<_configMsg.load("rpcServerPort")<<endl;
    cout<<"zkIp:"<<_configMsg.load("zkIp")<<endl;
    cout<<"zkPort:"<<_configMsg.load("zkPort")<<endl;
    */
}


MpRpcConf& MpRpcApplication::getConfig()
{
    return _configMsg;
}