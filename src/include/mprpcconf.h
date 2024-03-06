#pragma
#include<iostream>
#include<string>
#include<unordered_map>
#include "logger.h"
using namespace std;

//框架读取配置文件
class MpRpcConf
{
public:
    //解析配置文件
    bool loadConfigFile(const char* configFile);
    // bool loadConfigFile();
    //查询配置文件信息
    string load(string key);
    //去除字符串前后无关字符
    void trim(string& str);
private:
    unordered_map<string,string> _configMap;
};