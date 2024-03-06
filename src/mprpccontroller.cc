#include "mprpccontroller.h"


MyRpcController::MyRpcController()
{
    _failed=false;
    _errText="";   
}
void MyRpcController::Reset()
{
    _failed=false;
    _errText=""; 
}
bool MyRpcController::Failed() const
{
    return _failed;
}
std::string MyRpcController::ErrorText() const
{
    return _errText;
}

void MyRpcController::SetFailed(const std::string& reason)
{
    _failed=true;
    _errText=reason;
}

void MyRpcController::StartCancel(){}
bool MyRpcController::IsCanceled() const{}
void MyRpcController::NotifyOnCancel(google::protobuf::Closure* callback){}