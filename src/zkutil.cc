#include"zkutil.h"
#include "mprpcapplication.h"

//客户端链接的回调函数,zkserver给zkclient的通知
/*
typedef void (*watcher_fn)(zhandle_t *zh, int type, 
        int state, const char *path,void *watcherCtx);
*/
void global_watcher(zhandle_t *zh,int type,int state,const char *path,void *watcherCtx)
{
    if(type==ZOO_SESSION_EVENT)//回调的消息类型是和会话相关的消息类型
    {
        //zkclient和zkserver链接成功，释放锁
        if(state==ZOO_CONNECTED_STATE)
        {
            sem_t *sem=(sem_t*)zoo_get_context(zh);
            sem_post(sem);//信号量资源加一
        }
    }
}

ZkClient::ZkClient():_zhandle(nullptr)
{}
ZkClient::~ZkClient()
{
    if(_zhandle)
    {
        zookeeper_close(_zhandle);
    }
}

//连接zk服务器，启动zk客户端
void ZkClient::start()
{
    std::string zkIp=MpRpcApplication::getInstance().getConfig().load("zkIp");
    std::string zkPort=MpRpcApplication::getInstance().getConfig().load("zkPort");
    std::string zkHost=zkIp+":"+zkPort;

    //调用原生API，端口与IP，回调函数，会话超时时间
    /*
    ZOOAPI zhandle_t *zookeeper_init(const char *host, watcher_fn fn,
    int recv_timeout, const clientid_t *clientid, void *context, int flags);
  */
 
  /*
    zookeeper_mt:多线程版本
    zookeeper的API客户端程序提供了三个线程
    API调用线程
    网络I/O线程：专门在一个线程里处理网络I/O
    watcher回调线程
  */
     _zhandle=zookeeper_init(zkHost.c_str(),global_watcher,30000,nullptr,nullptr,0);
    // 此处仅判断_zhandle句柄的内存是否开辟成功，并不代表zk客户端与zk连接成功
     if(nullptr==_zhandle)
    {
        std::cout<<"zookeeper_init error!"<<std::endl;
        exit(EXIT_FAILURE);
    }

    // zk客户端（rpcServer）与zk的连接在另一个线程中进行
    // 因此此处需要阻塞等待连接建立成功之后才能继续向下执行
    // 当连接成功之后会执行global_watcher释放sem信号量
    sem_t sem;
    sem_init(&sem,0,0);//初始化资源为0
    zoo_set_context(_zhandle,&sem);//设置上下文，添加额外信息

    sem_wait(&sem);
    std::cout<<"zookeeper_init success!"<<std::endl;
}
// 创建zk节点,zkserver在指定的path上创建node节点
void ZkClient::createNode(const char *path,const char *data,int datalen,int state)
{
    char path_buffer[128];
    int bufferlen=sizeof(path_buffer);

    /*ZOOAPI int zoo_exists(zhandle_t *zh, const char *path, int watch, struct Stat *stat);*/
    int flag=zoo_exists(_zhandle,path,0,nullptr);
    if(flag==ZNONODE)//节点不存在
    {
        /*
        int zoo_create(zhandle_t *zh, const char *path, const char *value,
        int valuelen, const struct ACL_vector *acl, int flags,
        char *path_buffer, int path_buffer_len);


        */
       
        flag=zoo_create(_zhandle,path,data,datalen,&ZOO_OPEN_ACL_UNSAFE,state,
                    path_buffer,bufferlen);
        if(flag==ZOK)
        {
            std::cout<<"znode create success!"<<std::endl;
        }else{
            std::cout<<"flag:"<<flag<<std::endl;
            std::cout<<"znode create errno...path:"<<path<<std::endl;
            exit(EXIT_FAILURE);
        }
    }
}
// 获取zk节点数据
std::string ZkClient::getData(const char*path)
{
    char buffer[64];
    int bufferlen=sizeof(buffer);
    /*
    int zoo_get(zhandle_t *zh, const char *path, int watch, char *buffer,   
                   int* buffer_len, struct Stat *stat);
    */
    int flag=zoo_get(_zhandle,path,0,buffer,&bufferlen,nullptr);
    if(!flag==ZOK)
    {
        std::cout<<"get znode data errno!"<<std::endl;
        return "";
    }else{
        return buffer;
    }
}