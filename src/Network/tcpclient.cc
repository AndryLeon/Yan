
#include "tcpclient.h"

namespace Yan {

    TcpClient::TcpClient(const InetAddress& remoteAddr, EventPool* eventPool)
        : isConnected(false),
         eventPool_(eventPool),
         connector_(remoteAddr, eventPool_)
    {
        connector_.SetOnNewConnectionCallback(std::bind(&TcpClient::onNewConnection, this, std::placeholders::_1));
    }

    TcpClient::~TcpClient(){
        Common::MutexLock m(mutex_);
        if(isConnected && tcpConnectionPtr_){
            tcpConnectionPtr_->Disable();
        }
    }

    void TcpClient::Connect() {
        if(!isConnected){
            isConnected = true;
            connector_.Connect();
        }
    }

    void TcpClient::Disconnect() {
        if(isConnected && tcpConnectionPtr_){
            Common::MutexLock m(mutex_);
            isConnected = false;
            tcpConnectionPtr_->Shutdown();
        }
    }

    void TcpClient::onNewConnection(int fd) {
        InetAddress local(Socket::GetLocalSockAddr(fd));
        InetAddress remote(Socket::GetPeerSockAddr(fd));

        TcpConnectionPtr tmpPtr_(new TcpConnection(fd, local, remote, eventPool_));
        tmpPtr_->SetConnectionCallback(connectionCallback_);
        tmpPtr_->SetWriteCallback(writeCompleteCallback_);
        tmpPtr_->SetReadCallback(readCompleteCallback_);
        tmpPtr_->SetCloseCallback(std::bind(&TcpClient::onClose, this, std::placeholders::_1));

        tmpPtr_->Build();

        Common::MutexLock m(mutex_);
        tcpConnectionPtr_ = tmpPtr_;
    }

    void TcpClient::onClose(const TcpConnectionPtr& tcpCon){
        {
            Common::MutexLock m(mutex_);
            assert(tcpConnectionPtr_ == tcpCon);
            tcpConnectionPtr_.reset();
        }
        tcpCon->Disable();   //can be delete
    }

}