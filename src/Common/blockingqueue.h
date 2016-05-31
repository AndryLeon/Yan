
#ifndef YAN_BLOCKINGQUEUE_H
#define YAN_BLOCKINGQUEUE_H

#include <queue>
#include "noncopyable.h"
#include "condition.h"
#include "mutex.h"

namespace Yan {

    namespace Common {

        template  <typename T>
        class BlockingQueue : noncopyable{
        public:
            typedef std::vector<T> Queue;

            explicit BlockingQueue()
                    :mutex_(),
                     condition_(mutex_),
                     queue_(),
                     valid_(true){

            }

            bool Push(const T& t){
                MutexLock m(mutex_);
                if(!valid_)
                    return false;
                if(queue_.empty())
                    condition_.Signal();
                queue_.push_back(t);
                return true;
            }

            bool TakeAll(Queue* q, double w = -1.0){
                MutexLock m(mutex_);
                while(queue_.empty()){
                    condition_.Wait();
                }
                queue_.swap(*q);
                return true;
            }

            void SetInvalid(){
                MutexLock m(mutex_);
                valid_ = false;
            }

            bool IsValid(){
                MutexLock m(mutex_);
                return valid_;
            }

            int GetSize(){
                return queue_.size();
            }

        private:
            Mutex mutex_;
            Condition condition_;
            Queue queue_;
            bool valid_;
        };

    }

}

#endif //YAN_BLOCKINGQUEUE_H
