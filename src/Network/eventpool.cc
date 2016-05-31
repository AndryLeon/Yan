
#include <sys/eventfd.h>
#include <cassert>

#include "eventpool.h"
#include "socket.h"
#include "channel.h"
#include "../Common/log.h"

namespace Yan {

    EventPool::EventPool(int epollers, int workers, int epollMs)
        :epollMs_(epollMs),
         isRunning_(false),
         epollersNum_(epollers),
         workersNum_(workers),
         epollerThreads_(epollers),
         workerThreads_(workers)
    {
        epollerThreads_.Start();
        workerThreads_.Start();

        for(int i = 0; i < epollers; i++){
            epollers_.push_back(new Epoller());

            int wakeupFd = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
            wakeupSockets_.push_back(new Socket(wakeupFd));
            wakeupChannel_.push_back(new Channel(wakeupFd, this));

            epollers_.back().AddChannel(&(wakeupChannel_.back()));
        }
    }

    EventPool::~EventPool() {
        if(isRunning_){
            Stop();
        }
        epollerThreads_.Stop();
        workerThreads_.Stop();
    }

    void EventPool::Start(){
        assert(isRunning_ == false);
        isRunning_ = true;

        for(int i = 0; i < epollersNum_; i++){
            epollerThreads_.Schedule(std::bind(&EventPool::pollFunc, this, i), i);
        }
    }

    void EventPool::Stop() {
        if(isRunning_){
            WakeupAll();
            isRunning_ = false;
        }
    }

    void EventPool::WakeupAll() {
        assert(isRunning_);
        for(int i = 0; i < epollersNum_; i++){
            uint64_t val = 1;
            int result = ::write( wakeupChannel_.at(i).GetFd(), &val, sizeof(val) );
            if (result != sizeof(val)) {
                LOG_WARN("EventPool::WakeupAll error: %s\n", strerror(errno) );
            }
        }
    }

    void EventPool::WakeupOne(int i){
        assert(isRunning_);
        uint64_t val = 1;
        int result = ::write(wakeupChannel_.at(i).GetFd(), &val, sizeof(val));
        if(result != sizeof(val)){
            LOG_WARN("EventPool::WakeupOne error:%s\n", strerror(errno) );
        }
    }

    void EventPool::AddChannel(Channel* ch){
        epollers_[ch->GetID()%epollersNum_].AddChannel(ch);
    }

    void EventPool::DisableChannel(Channel *ch) {
        epollers_[ch->GetID()%epollersNum_].DisableChannel(ch);
    }

    void EventPool::DeleteChannel(Channel *ch) {
        epollers_[ch->GetID()%epollersNum_].DeleteChannel(ch);
    }

    void EventPool::PutTask(const Task &t, const Channel& ch) {
        workerThreads_.Schedule(t, ch.GetID()%workersNum_);
    }

    void EventPool::PutTask(const Task &t, int which) {
        workerThreads_.Schedule(t, which);
    }


    void EventPool::pollFunc(int which) {
        ChannelList channelList;
        Epoller& epoller = epollers_[which];

        LOG_TRACE("EventPool::PoollFunc %d\n", which);
        while(isRunning_){
            LOG_TRACE("Working\n");
            channelList.clear();
            epoller.Poll(epollMs_, &channelList);

            for(auto iter = channelList.begin(); iter != channelList.end(); ++iter){
                PutTask(std::bind(&Channel::EventHandle, *iter), **iter);
            }
        }
    }

}