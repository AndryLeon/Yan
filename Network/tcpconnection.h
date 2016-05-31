
#ifndef YAN_TCPCONNECTION_H
#define YAN_TCPCONNECTION_H

#include <boost/enable_shared_from_this.hpp>

#include "../Common/noncopyable.h"
#include "util.h"
#include "socket.h"
#include "channel.h"
#include "inet_address.h"
#include "buffer.h"
#include "eventpool.h"

namespace Yan {

    class TcpConnection : noncopyable, public std::enable_shared_from_this<TcpConnection> {
    public:
        TcpConnection(int fd, const InetAddress& localAddr,
                      const InetAddress& remoteAddr, EventPool* eventPool);

        ~TcpConnection();

        void Write(const std::string& s);
        void Write(const char* data, size_t size);

        int GetID() const{
            return id_;
        }

        void Build();
        void Disable();
        void Shutdown();

        void SetConnectionCallback(const ConnectionCallback& cb){
            connectionCallback_ = cb;
        }

        void SetWriteCallback(const WriteCompleteCallback& cb){
            writeCompleteCallback_ = cb;
        }

        void SetReadCallback(const ReadCompleteCallback& cb){
            readCompleteCallback_ = cb;
        }

        void SetCloseCallback(const CloseCallback& cb){
            closeCallback_ = cb;
        }

        std::string GetLocalAddrString() const {
            return localAddr_.ToString();
        }

        std::string GetRemoteAddrString() const {
            return remoteAddr_.ToString();
        }


    private:
        void queueWrite(const std::string& s);
        void onWrite();
        void onRead();
        void onClose();

        static volatile std::atomic_int ID;
        const int id_;
        volatile std::atomic_bool isConnected_;
        //TcpConnection 对socket和channel进行管理
        //socket析构时关闭fd,TcpConnection析构时注销channel
        Socket socket_;
        Channel channel_;
        InetAddress localAddr_;
        InetAddress remoteAddr_;
        EventPool* eventPool_;

        Buffer inBuffer_;
        Buffer outBuffer_;

        ConnectionCallback connectionCallback_;  //called when connection is built
        WriteCompleteCallback writeCompleteCallback_;
        ReadCompleteCallback readCompleteCallback_;
        CloseCallback closeCallback_;
    };

}


#endif //YAN_TCPCONNECTION_H
