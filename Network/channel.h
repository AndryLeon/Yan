
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

        static volatile int ID;

        Channel(int fd, EventPool* eventPool);
        ~Channel();

        void SetReadCallback(const ReadCallback& rcb);
        void SetWriteCallback(const WriteCallback& wcb);
        void SetCloseCallback(const CloseCallback& ccb);

        int GetFd() const;
        int GetEvents() const;
        void SetRevents(int revent);
        int GetStatus() const;
        void SetStatus(int status);
        void Tie(const std::shared_ptr<void>& obj);

        void Register();
        void Unregister();
        void Disable();

        void EventHandle();

    private:
        void SafeEventHandle();

        const int id_;
        const int fd_;
        const int events_;
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
