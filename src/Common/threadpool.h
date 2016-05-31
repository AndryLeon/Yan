
#ifndef YAN_THREADPOOL_H
#define YAN_THREADPOOL_H

#include <boost/ptr_container/ptr_vector.hpp>

#include <atomic>
#include <functional>

#include "noncopyable.h"
#include "blockingqueue.h"
#include "thread.h"

namespace Yan {

    namespace Common {

        class ThreadPool : noncopyable {
        public:
            typedef std::function<void()> Task;
            typedef BlockingQueue<Task> TaskQueue;

            ThreadPool(int workers);
            ~ThreadPool();

            void Start();
            void Stop();

            bool Schedule(const Task& task, int which);

        private:
            void TaskHandler(int which);
            void QuitTask();

            volatile std::atomic_bool running_;
            volatile std::atomic_int workers_;
            volatile std::atomic_int doneJobs_;

            boost::ptr_vector<Thread> threads_;
            boost::ptr_vector<TaskQueue> taskQueues_;
        };
    }

}

#endif //YAN_THREADPOOL_H
