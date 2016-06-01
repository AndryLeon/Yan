
#ifndef YAN_TCPCLIENT_H
#define YAN_TCPCLIENT_H

#include <atomic>
#include "Common/noncopyable.h"
#include "Common/mutex.h"
#include "eventpool.h"
#include "connector.h"
#include "tcpconnection.h"

namespace Yan {

    class TcpClient : noncopyable {
    public:
        TcpClient(const InetAddress& remoteAddr, EventPool* eventPool);
        ~TcpClient();

        void Connect();
        void Disconnect();

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
        void onNewConnection(int fd);
        void onClose(const TcpConnectionPtr&);

        volatile std::atomic_bool isConnected;
        Common::Mutex mutex_;

        EventPool* eventPool_;
        Connector connector_;
        TcpConnectionPtr tcpConnectionPtr_;

        ConnectionCallback connectionCallback_;
        WriteCompleteCallback writeCompleteCallback_;
        ReadCompleteCallback readCompleteCallback_;
    };

}

#endif //YAN_TCPCLIENT_H
