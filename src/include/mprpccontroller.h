#include<google/protobuf/service.h>

class MyRpcController:public google::protobuf::RpcController
{
public:
    MyRpcController();
    void Reset();
    bool Failed() const;
    std::string ErrorText() const;
    void StartCancel();

    void SetFailed(const std::string& reason);
    bool IsCanceled() const;
    void NotifyOnCancel(google::protobuf::Closure* callback);
private:
    bool _failed;
    std::string _errText;
};