#pragma once
#include "lockqueue.h"
#include <thread>

enum logLevel{
    INFO,//普通信息
    ERROR,//错误信息
};

class Logger
{
public:
    static Logger& getInstance();//获取日志单例对象
    void setLogLevel(logLevel level);//设置日志级别
    void log(std::string msg);//写日志
private:
    int _logLevel;//记录日志级别
    LockQueue<std::string> _lckQueue;//日志缓冲队列

    Logger();
    Logger(const Logger&)=delete;
    Logger(const Logger&&)=delete;
    Logger& operator=(const Logger&)=delete;
};


#define LOG_INFO(logMsgformat,...)\
    do\
    {\
        Logger &logger=Logger::getInstance();\
        logger.setLogLevel(INFO);\
        char buf[1024]={0};\
        snprintf(buf,1024,logMsgformat,##__VA_ARGS__);\
        logger.log(buf);\
    }while(0)

#define LOG_ERR(logmsgformat,...)\
    do\
    { \
        Logger &logger=Logger::getInstance();\
        logger.setLogLevel(ERROR);\
        char buf[1024]={0};\
        snprintf(buf,1024,logmsgformat,##__VA_ARGS__);\
        logger.log(buf);\
    }while(0)