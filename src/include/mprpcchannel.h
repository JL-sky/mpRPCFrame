#pragma once
#include <google/protobuf/service.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/message.h>
#include "rpcheader.pb.h"

class MyRpcChannel:public google::protobuf::RpcChannel
{
public:
    void CallMethod(const google::protobuf::MethodDescriptor* method,
                    google::protobuf::RpcController* controller, 
                    const google::protobuf::Message* request,
                    google::protobuf::Message* response, 
                    google::protobuf::Closure* done);
    void sendRpcRequest(google::protobuf::RpcController* controller,
                        std::string sendRpcStr,
                        google::protobuf::Message* response,
                        std::string methodPath);
};

