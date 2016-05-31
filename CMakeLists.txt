cmake_minimum_required(VERSION 2.6)
project(Yan)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

set(SOURCE_FILES
        demo/echoserver.cc

        Common/blockingqueue.h
        Common/condition.h
        Common/log.cc
        Common/log.h
        Common/mutex.h
        Common/noncopyable.h
        Common/slice.h
        Common/thread.cc
        Common/thread.h
        Common/threadpool.cc
        Common/threadpool.h
        Common/timestamp.h

        Network/acceptor.cc
        Network/acceptor.h
        Network/buffer.h
        Network/channel.cc
        Network/channel.h
        Network/connector.cc
        Network/connector.h
        Network/epoller.cc
        Network/epoller.h
        Network/eventpool.cc
        Network/eventpool.h
        Network/inet_address.h
        Network/socket.cc
        Network/socket.h
        Network/tcpclient.cc
        Network/tcpclient.h
        Network/tcpconnection.cc
        Network/tcpconnection.h
        Network/tcpserver.cc
        Network/tcpserver.h
        Network/util.h
        Network/timequeue.h)

add_executable(Yan ${SOURCE_FILES})