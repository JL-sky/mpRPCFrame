set(SRC_LIST calleruserservice.cc ../user.pb.cc)
add_executable(consumer ${SRC_LIST})
# 将可执行文件与需要的动态库进行连接，其中mprpc是自定义的框架动态库
target_link_libraries(consumer mprpc protobuf)