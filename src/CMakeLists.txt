aux_source_directory(. SRC_LIST)
# set(SRC_LIST mprpcapplication.cpp mprpcconf.cpp rpcheader.pb.cc rpcprovider.cpp mprpcchannel.cpp )
# 生成自定义静态链接
add_library(mprpc ${SRC_LIST})
target_link_libraries(mprpc muduo_net muduo_base pthread zookeeper_mt)