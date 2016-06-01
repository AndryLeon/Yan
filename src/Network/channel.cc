
#include <sys/epoll.h>

#include "channel.h"
#include "eventpool.h"
#include "Common/log.h"

namespace Yan {

    volatile std::atomic_int Channel::ID(0);

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
                safeEventHandle();
            }else{
                LOG_TRACE("Channel::EventHandle, Unregister()\n");
                Unregister();
            }
        }else{
            safeEventHandle();
            //LOG_TRACE("Channel::EventHandle, untied\n");
        }
    }

    void Channel::safeEventHandle() {
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
