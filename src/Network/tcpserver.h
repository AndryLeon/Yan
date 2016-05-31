
#ifndef YAN_TCPSERVER_H
#define YAN_TCPSERVER_H

#include <map>

#include "Common/noncopyable.h"
#include "channel.h"
#include "tcpconnection.h"
#include "acceptor.h"

namespace Yan {

    class TcpServer : noncopyable{
    public:
        typedef std::map<int, TcpConnectionPtr> ConnectionMap;

        TcpServer(const InetAddress& bindAddr, EventPool* eventPool);
        ~TcpServer();

        void Start();

        void SetConnectionCallback(const ConnectionCallback& cb){
            connectionCallback_ = cb;
        }

        void SetWriteCallback(const WriteCompleteCallback& cb){
            writeCompleteCallback_ = cb;
        }

        void SetReadCallback(const ReadCompleteCallback& cb){
            readCompleteCallback_ = cb;
        }

    private:
        void OnNewConnection(int fd, const InetAddress& peeraddr);
        void OnClose(const TcpConnectionPtr&);

        volatile std::atomic_bool isStart_;
        volatile std::atomic_int connectionNum_;
        InetAddress bindAddr_;
        Acceptor acceptor_;
        ConnectionMap connectionMap_;
        EventPool* eventPool_;
        Common::Mutex mutex_;
        ConnectionCallback connectionCallback_;
        WriteCompleteCallback writeCompleteCallback_;
        ReadCompleteCallback readCompleteCallback_;
    };

}

#endif //YAN_TCPSERVER_H
