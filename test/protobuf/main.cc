#include<iostream>
#include<string>
#include<test.pb.h>
using namespace std;
using namespace rpcProto;

void test()
{
    friendListResponse fres;
    resultCode* pc=fres.mutable_resmsg();
    pc->set_errcode(0);
    pc->set_errmsg("请求无错");

    // 使用add_列表变量名获取列表消息指针
    User *user1=fres.add_friendlist();
    user1->set_name("zhangsan");
    user1->set_age(20);
    user1->set_sex(User::man);

    User *user2=fres.add_friendlist();
    user2->set_name("lisi");
    user2->set_age(22);
    user2->set_sex(User::woman);
    cout<<fres.friendlist_size()<<endl;
    
}

int main()
{
    test();
    return 0;
}