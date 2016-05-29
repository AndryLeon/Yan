
#include <sys/epoll.h>

#include "channel.h"
#include "eventpool.h"
#include "../Common/log.h"

namespace Yan {

    volatile int Channel::ID = 0;

    Channel::Channel(int fd, EventPool* eventPool)
        :id_(++ID),
         fd_(fd),
         events_(EPOLLIN | EPOLLOUT | EPOLLRDHUP | EPOLLERR | EPOLLET),
         revents_(0),
         status_(0),
         isTied(false),
         eventPool_(eventPool) {
    }

    Channel::~Channel() {
        LOG_TRACE("Channel::~Channel, id : %d, fd : %d\n", id_, fd_);
    }

    void Channel::SetReadCallback(const ReadCallback& rcb){
        readCallback_ = rcb;
    }

    void Channel::SetWriteCallback(const WriteCallback &wcb) {
        writeCallback_ = wcb;
    }

    void Channel::SetCloseCallback(const CloseCallback &ccb) {
        closeCallback_ = ccb;
    }

    int Channel::GetFd() const {
        return fd_;
    }

    int Channel::GetEvents() const {
        return events_;
    }

    void Channel::SetRevents(int revents){
        revents_ = revents;
    }

    int Channel::GetStatus() const {
        return status_;
    }

    void Channel::SetStatus(int status) {
        status_ = status;
    }

    void Channel::Tie(const std::shared_ptr<void>& obj){
        isTied = true;
        tied_obj = obj;
    }

    void Channel::Register(){
        eventPool_->AddChannel(this);
    }

    void Channel::Unregister(){
        eventPool_->DeleteChannel(this);
    }

    void Channel::Disable() {
        eventPool_->DisableChannel(this);
    }

    void Channel::EventHandle() {
        if(isTied){
            std::shared_ptr<void> guard = tied_obj.lock();
            if(guard){
                SafeEventHandle();
            }else{
                LOG_TRACE("Channel::EventHandle, Disable()\n");
                Disable();
            }
        }else{
            SafeEventHandle();
        }
    }

    void Channel::SafeEventHandle() {
        int revents = revents_;

        LOG_TRACE("id=%u, fd=%d\n", id_, fd_);

        if((revents & EPOLLRDHUP) && closeCallback_) {
            closeCallback_();
        }
        if((revents & (EPOLLIN | EPOLLPRI)) && readCallback_) {
            readCallback_();
        }
        if((revents & EPOLLOUT) && writeCallback_) {
            writeCallback_();
        }
    }
}
