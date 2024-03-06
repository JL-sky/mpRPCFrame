set(SRC_LIST calleeuserservice.cc ../user.pb.cc)
add_executable(provider ${SRC_LIST})
# 将可执行文件与需要的动态库进行连接，其中mprpc是自定义的框架动态库
target_link_libraries(provider mprpc protobuf)