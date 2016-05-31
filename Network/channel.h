
#ifndef YAN_CHANNEL_H
#define YAN_CHANNEL_H

#include <atomic>
#include <memory>
#include "../Common/noncopyable.h"

namespace Yan {

    class EventPool;

    class Channel : noncopyable {
    public:
        typedef std::function<void()> ReadCallback;
        typedef std::function<void()> WriteCallback;
        typedef std::function<void()> CloseCallback;

        Channel(int fd, EventPool* eventPool);

        ~Channel();

        void SetReadCallback(const ReadCallback& rcb){
            readCallback_ = rcb;
        }

        void SetWriteCallback(const WriteCallback& wcb){
            writeCallback_ = wcb;
        }

        void SetCloseCallback(const CloseCallback& ccb){
            closeCallback_ = ccb;
        }

        int GetID() const {
            return id_;
        }

        int GetFd() const{
            return fd_;
        }

        int GetEvents() const{
            return events_;
        }

        void SetEvents(int events){
            events_ = events;
        }

        void SetRevents(int revents){
            revents_ =  revents;
        }

        int GetStatus() const{
            return status_;
        }

        void SetStatus(int status){
            status_ = status;
        }

        void Tie(const std::shared_ptr<void>& obj);

        void Register();
        void Unregister();
        void Disable();

        void EventHandle();

    private:
        void safeEventHandle();

        static volatile std::atomic_int ID;

        const int id_;
        const int fd_;
        int events_;
        volatile std::atomic_int revents_;         //must be volatile as is changed by IO thread but read by working thread
        volatile std::atomic_int status_;          //the same reason
        std::weak_ptr<void> tied_obj;  //volatile?
        bool isTied;                   //volatile?

        EventPool* eventPool_;
        ReadCallback readCallback_;
        WriteCallback writeCallback_;
        CloseCallback closeCallback_;
    };

}

#endif //YAN_CHANNEL_H
