cmake_minimum_required(VERSION 3.8)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

message(status "bnet CMAKE_SOURCE_DIR is " ${CMAKE_SOURCE_DIR})
message(status "bnet PROJECT_BINARY_DIR is " ${CMAKE_BINARY_DIR})

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(HunterGate)
HunterGate(
      URL "https://github.com/ruslo/hunter/archive/v0.23.165.tar.gz"
      SHA1 "5a73f91df5f6109c0bb1104d0c0ee423f7bece79"
)

project(bnet)

hunter_add_package(Boost)
find_package(Boost CONFIG REQUIRED)

hunter_add_package(glog)
find_package(glog CONFIG REQUIRED)

set(SOURCE_FILES
    asyncprocessor.cpp asyncprocessor.h
    processorpool.cpp processorpool.h
    tcpchannel.cpp tcpchannel.h
    tcpclient.cpp tcpclient.h
    tcpconnection.cpp tcpconnection.h
    tcpserver.cpp tcpserver.h
)

include_directories(${CMAKE_SOURCE_DIR}/../)
#set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
add_library(bnet ${SOURCE_FILES})
target_link_libraries(bnet Boost::boost glog::glog)

# set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
hunter_add_package(gflags)
find_package(gflags CONFIG REQUIRED)

add_executable(echo_server 
    example/io_service-per-cpu_demo/echo_client.cpp 
    example/io_service-per-cpu_demo/server.cpp
    example/io_service-per-cpu_demo/main.cpp)
target_link_libraries(echo_server bnet Boost::boost gflags)
