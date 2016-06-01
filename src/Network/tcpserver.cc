
#include "tcpserver.h"
#include <netinet/in.h>

namespace Yan {

    TcpServer::TcpServer(const InetAddress& bindAddr, EventPool* eventPool)
        :isStart_(false),
         connectionNum_(0),
         bindAddr_(bindAddr),
         acceptor_(eventPool),
         eventPool_(eventPool)
    {
        acceptor_.SetOnNewConnectionCallback(std::bind(&TcpServer::onNewConnection, this,
                                                       std::placeholders::_1, std::placeholders::_2));
    }

    TcpServer::~TcpServer() {
        Common::MutexLock m(mutex_);
        for(auto iter = connectionMap_.begin(); iter != connectionMap_.end();
                ++iter){
            iter->second->Disable();
            iter->second.reset();
        }
    }

    void TcpServer::Start(){
        if(!isStart_){
            isStart_ = true;
            acceptor_.SetAndBind(bindAddr_);
            acceptor_.Listen();
        }
    }

    void TcpServer::RemoveConnection(const TcpConnectionPtr& conn){
        eventPool_->PutTask(std::bind(&TcpServer::removeConnection, this, conn), conn->GetChannelID());
    }

    void TcpServer::onNewConnection(int fd, const InetAddress& peeraddr) {
        InetAddress local(Socket::GetLocalSockAddr(fd));
        TcpConnectionPtr tmpPtr(new TcpConnection(fd, local, peeraddr, eventPool_));

        tmpPtr->SetConnectionCallback(connectionCallback_);
        tmpPtr->SetWriteCallback(writeCompleteCallback_);
        tmpPtr->SetReadCallback(readCompleteCallback_);
        tmpPtr->SetCloseCallback(std::bind(&TcpServer::onClose, this, std::placeholders::_1));

        tmpPtr->Build();

        Common::MutexLock m(mutex_);
        connectionMap_[tmpPtr->GetID()] = tmpPtr;
    }

    void TcpServer::onClose(const TcpConnectionPtr& conPtr) {
        {
            Common::MutexLock m(mutex_);
            LOG_TRACE("TcpServer::OnClose\n");
            auto iter = connectionMap_.find(conPtr->GetID());
            assert(iter != connectionMap_.end());
            connectionMap_.erase(iter);
        }
        conPtr->Disable();  //can be delete
    }

    void TcpServer::removeConnection(const TcpConnectionPtr& conPtr) {
        {
            Common::MutexLock m(mutex_);
            auto iter = connectionMap_.find(conPtr->GetID());
            assert(iter != connectionMap_.end());
            connectionMap_.erase(iter);
        }
        conPtr->ConnectDestroyed();
    }
}

