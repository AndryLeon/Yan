
#ifndef YAN_EPOLLER_H
#define YAN_EPOLLER_H

#include <vector>

#include <sys/epoll.h>

#include "../Common/noncopyable.h"


namespace Yan {

    class Channel;

    class Epoller :noncopyable {
    public:
        typedef std::vector<epoll_event> EventList;
        typedef std::vector<Channel*> ChannelList;

        Epoller();
        ~Epoller();

        void AddChannel(Channel* c);
        void DisableChannel(Channel* c);
        void DeleteChannel(Channel* c);

        void Poll(int timeout_ms, ChannelList* ch);

    private:
        int epollFd_;
        EventList eventList_;
    };

}

#endif //YAN_EPOLLER_H
