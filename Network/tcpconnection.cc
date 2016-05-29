
#include "tcpconnection.h"
#include "../Common/slice.h"

namespace Yan {

    volatile std::atomic_int TcpConnection::ID(0);

    TcpConnection::TcpConnection(int fd, const InetAddress& localAddr,
                                 const InetAddress& remoteAddr, EventPool* eventPool)
        :id_(++ID),
         isConnected_(false),
         socket_(fd),
         channel_(fd, eventPool),
         localAddr_(localAddr),
         remoteAddr_(remoteAddr),
         eventPool_(eventPool) {

        socket_.SetKeepAlive(true);

        channel_.SetWriteCallback(std::bind(&TcpConnection::OnWrite, this));
        channel_.SetReadCallback(std::bind(&TcpConnection::OnRead, this));
        channel_.SetCloseCallback(std::bind(&TcpConnection::OnClose, this));
    }

    TcpConnection::~TcpConnection() {
        channel_.Unregister();
        LOG_TRACE("Connection destructor, %s:%d\n", remoteAddr_.GetIp().c_str(), remoteAddr_.GetPort());
    }

    void TcpConnection::Write(const std::string &s) {
        std::string data(s);
        eventPool_->PutTask(std::bind(&TcpConnection::QueueWrite, shared_from_this(), s), channel_);
    }

    void TcpConnection::Write(const char *data, size_t size) {
        Write(std::string(data, size));
    }

    void TcpConnection::Build(){
        isConnected_ = true;
        channel_.Register();
        channel_.Tie(shared_from_this());
        if(connectionCallback_)
            connectionCallback_(shared_from_this());
    }

    void TcpConnection::Disable() {
        if(isConnected_){
            isConnected_ = false;
            channel_.Disable();
            if(connectionCallback_)
                connectionCallback_(shared_from_this());
        }
    }

    void TcpConnection::Shutdown() {
        if (::shutdown(channel_.GetFd(), SHUT_WR) < 0) {
            LOG_WARN("TcpConnection::Shutdown error: %s\n", strerror(errno));
        }
        isConnected_ = false;
    }

    void TcpConnection::QueueWrite(const std::string &s) {
        if(!isConnected_){
            LOG_WARN("TcpConnection::QueueWrite, unable to write as disconnected\n");
            return;
        }

        const char* data = s.c_str();
        size_t size = s.size();

        if(outBuffer_.ReadableBytes() == 0){
            int result = ::write(channel_.GetFd(), data, size);
            if(result > 0){      //
                size -= result;
                if(size == 0 && writeCompleteCallback_){
                    eventPool_->PutTask(std::bind(writeCompleteCallback_, shared_from_this()), channel_);
                }
            }else{
                if(errno == EAGAIN) {
                    LOG_TRACE("Waiting for next write.");
                } else {
                    LOG_WARN("write error: %s", strerror(errno));
                }
            }
        }
        if(size > 0){
            outBuffer_.Append(data+s.size()-size, size);
        }
    }

    void TcpConnection::OnWrite(){
        if(outBuffer_.ReadableBytes() == 0){
            return;
        }

        Common::Slice slice = outBuffer_.ToSlice();
        int result = ::write(channel_.GetFd(), slice.Data(), slice.GetSize());

        if(result > 0){
            outBuffer_.ReadIndexInc(result);
            if(outBuffer_.ReadableBytes() == 0 && writeCompleteCallback_){
                eventPool_->PutTask(std::bind(writeCompleteCallback_, shared_from_this()), channel_);
            }
        }else{
            LOG_WARN("write error: %s", strerror(errno));
        }
    }

    void TcpConnection::OnRead() {
        int saved_errno;
        int result = inBuffer_.ReadFd(channel_.GetFd(), &saved_errno);
        if (result > 0 && readCompleteCallback_) {
            eventPool_->PutTask(std::bind(readCompleteCallback_, shared_from_this(), &inBuffer_), channel_);
        }else if(result == 0) {
            OnClose();
        }else{
            if(saved_errno != EAGAIN)
                LOG_WARN("OnRead error occur\n");
        }
    }

    void TcpConnection::OnClose() {
        if(isConnected_){
            isConnected_ = false;
            channel_.Disable();
            if(connectionCallback_)
                connectionCallback_(shared_from_this());
            if(closeCallback_)
                closeCallback_(shared_from_this());
        }
    }


}

