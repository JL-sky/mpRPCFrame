#include"mprpcconf.h"
#include<unistd.h>
#include<fstream>

//解析配置文件
bool MpRpcConf::loadConfigFile(const char* configFile)
{
    FILE* fp=fopen(configFile,"r");
    // cout<<configFile<<endl;
    if(fp==nullptr)
    {
        // LOG("mpRpc config is not exists!");
        LOG_INFO("mpRpc config is not exists!");
        exit(EXIT_FAILURE);
    }

    while(!feof(fp))
    {
        char line[512]={0};
        //按行读取
        fgets(line,512,fp);
        string read_buf(line);
        //去除前后空格和多余的字符
        trim(read_buf);
        
        size_t idx=read_buf.find("=");
        //无效配置项
        if(idx==-1)
        {
            continue;
        }
        string key=read_buf.substr(0,idx);
        string value=read_buf.substr(idx+1,read_buf.size()-idx);
        _configMap.insert({key,value});
    }
    return true;
}
//查询配置文件信息
string MpRpcConf::load(string key)
{
    auto it=_configMap.find(key);
    if(it==_configMap.end())
    {
        return string("");
    }
    return it->second;
}

// 去除字符串的前后空格以及无关字符
void MpRpcConf::trim(string& str)
{
    //去除前空格
    size_t idx=str.find_first_not_of(" \t\n\r");//\r是回车
    if(idx!=string::npos)
    {
        str.substr(idx);
    }else{
        //字符串全是空格，直接清空
        str.clear();
        return;
    }
    //去除后空格
    idx=str.find_last_not_of(" \t\n\r");
    if(idx!=string::npos)
    {
        str=str.substr(0,idx+1);
    }
}