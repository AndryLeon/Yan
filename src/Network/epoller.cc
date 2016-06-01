
#include <zconf.h>
#include <cstring>
#include <cerrno>

#include "epoller.h"
#include "channel.h"
#include "Common/log.h"

namespace Yan {

    namespace {
        const int New = 0;
        const int Add = 1;
        const int Disable = 2;
        const int Delete = 3;
    }

    Epoller::Epoller()
        :epollFd_(::epoll_create1(EPOLL_CLOEXEC)),
         eventList_(1024)
    { }

    Epoller::~Epoller() {
        ::close(epollFd_);
    }

    void Epoller::AddChannel(Channel *ch) {
        epoll_event ev;

        bzero(&ev, sizeof(ev));
        ev.events = ch->GetEvents();
        ev.data.ptr = ch;

        ch->SetStatus(Add);

        LOG_TRACE("Channel::Register fd : %d\n", ch->GetFd());

        if(::epoll_ctl(epollFd_, EPOLL_CTL_ADD, ch->GetFd(), &ev) < 0) {
            LOG_WARN("Epoller::AddChannel, error: %s", strerror(errno));
        }
    }

    void Epoller::DisableChannel(Channel *ch) {
        if(ch->GetStatus() == Add){
            epoll_event ev;

            bzero(&ev, sizeof(ev));
            ev.events = 0;
            ev.data.ptr = ch;

            ch->SetStatus(Disable);

            if(::epoll_ctl(epollFd_, EPOLL_CTL_MOD, ch->GetFd(), &ev) < 0){
                LOG_WARN("Epoller::DisableChannel, error: %s", strerror(errno));
            }
        }
    }

    void Epoller::DeleteChannel(Channel* ch){
        if(ch->GetStatus() != Delete && ch->GetStatus() != New){
            ch->SetStatus(Delete);
            LOG_TRACE("Channel::Delete fd : %d\n", ch->GetFd());
            if(::epoll_ctl(epollFd_, EPOLL_CTL_DEL, ch->GetFd(), NULL) < 0){
                LOG_WARN("Epoller::DeleteChannel, error: %s", strerror(errno));
            }
        }
    }

    void Epoller::Poll(int timeout_ms, ChannelList* activeChannels){
        int numEvents = ::epoll_wait(
                epollFd_, &*eventList_.begin(), eventList_.size(), timeout_ms
        );

        if(numEvents > 0){
            LOG_TRACE("Get %d events\n", numEvents);
            activeChannels->reserve(numEvents);
            for(int i = 0; i < numEvents; i++){
                Channel* channel = static_cast<Channel*>(eventList_[i].data.ptr);
                activeChannels->push_back(channel);
                channel->SetRevents(eventList_[i].events);
            }
            if(numEvents == eventList_.size()){
                eventList_.resize(numEvents << 1);
            }
        }else if(numEvents == 0){
            LOG_TRACE("Nothing happened in epoll_wait\n");
        }
    }

}
