
#include "connector.h"
#include "socket.h"
#include "channel.h"
#include "Common/log.h"

namespace Yan {

    Connector::Connector(const InetAddress& remoteAddress, EventPool* eventPool)
        :isConnected_(false),
         remote_address_(remoteAddress),
         eventPool_(eventPool) {
    }

    void Connector::Connect() {
        isConnected_ = true;
        eventPool_->PutTask(std::bind(&Connector::queueConnect, this));  //
    }

    void Connector::queueConnect(){
        if(isConnected_){
            int newFd = Socket::CreateNonblockingSocket();
            int result = Socket::Connect(newFd, remote_address_.GetSockaddr() );
            if(result == 0 || errno == EINPROGRESS){
                conn_channel_.reset(new Channel(newFd, eventPool_));
                conn_channel_->SetWriteCallback(std::bind(&Connector::onConnect, this));
                conn_channel_->Register();
            }else{
                ::close(newFd);
                LOG_WARN("Connector::QueueConnect error: %s\n", strerror(errno));
            }
        }
    }

    void Connector::onConnect() {
        if(isConnected_){
            conn_channel_->Unregister();
            int fd = conn_channel_->GetFd();
            onNewConnectionCallback_(fd);  //
        }
    }

}