cmake_minimum_required(VERSION 3.4)
project(Yan)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp Common/blockingqueue.h Common/log.h Common/condition.h Common/noncopyable.h Common/mutex.h Common/slice.h Common/thread.h Common/thread.cc Common/threadpool.h Common/threadpool.cc Common/timestamp.h Network/socket.h Network/socket.cc Network/inet_address.h Network/buffer.h)
add_executable(Yan ${SOURCE_FILES})