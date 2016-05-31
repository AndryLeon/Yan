
#include "threadpool.h"
#include "log.h"

namespace Yan {

    namespace Common {

        ThreadPool::ThreadPool(int workers)
            :running_(false),
             workers_(workers),
             doneJobs_(0) {
        }

        ThreadPool::~ThreadPool() {
            if(running_){
                Stop();
            }
        }

        void ThreadPool::Start() {
            assert(!running_);
            assert(threads_.empty());

            running_ = true;
            threads_.reserve(workers_);
            for(int i = 0; i < workers_; i++){
                taskQueues_.push_back(new TaskQueue);
                threads_.push_back(new Thread( std::bind(&ThreadPool::TaskHandler, this, i)));
                threads_[i].Start();
            }
            LOG_TRACE("ThreadPool::Start(), Finish!\n");
        }

        void ThreadPool::Stop() {
            if(running_) {
                for(int i = 0; i < threads_.size(); i++) {
                    bool ok = taskQueues_[i].Push(
                            std::bind(&ThreadPool::QuitTask, this));
                    if(ok){
                        doneJobs_++;
                    }
                }
                for(int i = 0; i < threads_.size(); i++) {
                    threads_[i].Join();
                }
                running_ = false;
                assert(doneJobs_ == 0);
            }
        }



        bool ThreadPool::Schedule(const Task& task, int which = -1){
            if(!running_)
                return false;
            doneJobs_++;
            int n = doneJobs_;
            which = which == -1? which%n : which;
            if(taskQueues_[which].Push(task)){
                return true;
            }else {
                doneJobs_--;
                return false;
            }
        }

        void ThreadPool::TaskHandler(int which){
            BlockingQueue<Task>::Queue queue;
            while(running_ || taskQueues_[which].GetSize()){
                queue.clear();
                taskQueues_[which].TakeAll(&queue);
                for(int i = 0; i < queue.size(); i++){
                    queue[i]();
                    doneJobs_--;
                }
            }
            taskQueues_[which].SetInvalid();
        }

        void ThreadPool::QuitTask() {
            LOG_TRACE("ThreadPool::QuitTask\n");
        }
    }

}