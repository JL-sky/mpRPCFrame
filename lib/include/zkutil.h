#pragma once

#include<semaphore.h>
#include<zookeeper/zookeeper.h>
#include<string>

class ZkClient
{
public:
    ZkClient();
    ~ZkClient();

    //连接zk服务器，启动zk客户端
    void start();
    // 创建zk节点
    /*
        path：节点路径
        data：节点数据
        datalen：节点数据长度
    */
    void createNode(const char *path,const char *data,int datalen,int state=0);
    // 获取zk节点数据
    std::string getData(const char*path);
private:
    zhandle_t* _zhandle;
};