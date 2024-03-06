#include "logger.h"
#include <time.h>
#include<iostream>
#include<unistd.h>

Logger& Logger::getInstance()
{
    static Logger logger;
    return logger;
}

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

                std::string rootPath="/home/pcl/cProject/projects/myPro/mpRPC";
                std::string logPath=rootPath+"/log/"+std::string(filename);
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

//设置日志级别
void Logger::setLogLevel(logLevel level)
{
    _logLevel=level;
}
//写日志,将日志信息写入到日志队列缓冲区中
void Logger::log(std::string msg)
{
    _lckQueue.push(msg);
}