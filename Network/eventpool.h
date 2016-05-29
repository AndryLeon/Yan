
#ifndef YAN_EVENTPOOL_H
#define YAN_EVENTPOOL_H

#include <atomic>
#include <boost/ptr_container/ptr_vector.hpp>

#include "epoller.h"
#include "../Common/noncopyable.h"
#include "../Common/threadpool.h"
#include "../Common/blockingqueue.h"

namespace Yan {

    class Socket;
    class Channel;

    class EventPool : noncopyable {
    public:
        typedef std::function<void()> Task;
        typedef Epoller::ChannelList ChannelList;

        typedef std::function<void()> Task;
        typedef Common::BlockingQueue<Task> IOTaskQueue;

        EventPool(int epollers, int workers, int epollMs = 10000);
        ~EventPool();

        void Start();
        void Stop();
        void Wakeup();

        void AddChannel(Channel* ch);
        void DisableChannel(Channel* ch);
        void DeleteChannel(Channel* ch);

        void PutTask(const Task& t, const Channel& ch);
        void PutTask(const Task& t, int which = 0);

        void PutIOTask(const Task& t, const Channel& ch);
        void PutIOTask(const Task& t, int which = 0);

    private:
        void PollFunc(int which);

        const int epollMs_;
        volatile std::atomic_bool isRunning_;
        const int epollersNum_;
        const int workersNum_;

        boost::ptr_vector<Epoller> epollers_;
        boost::ptr_vector<Socket> wakeupSockets_;
        boost::ptr_vector<Channel> wakeupChannel_;
        boost::ptr_vector<IOTaskQueue> ioTaskQueues_;

        Common::ThreadPool epollerThreads_;
        Common::ThreadPool workerThreads_;

    };

}

#endif //YAN_EVENTPOOL_H
