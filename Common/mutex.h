
#ifndef YAN_MUTEX_H
#define YAN_MUTEX_H

#include <pthread.h>
#include "noncopyable.h"

namespace Yan{

    namespace Common{

        class Condition;

        class Mutex : noncopyable{
        public:
            Mutex(){
                pthread_mutex_init(&mutex_, NULL);
            }

            ~Mutex(){
                pthread_mutex_destroy(&mutex_);
            }

            void Lock(){
                pthread_mutex_lock(&mutex_);
            }

            void Unlock(){
                pthread_mutex_unlock(&mutex_);
            }

        private:
            friend class Condition;
            pthread_mutex_t mutex_;
        };

        class MutexLock : noncopyable{
        public:
            explicit MutexLock(Mutex& mutex)
                    :mutex_(mutex){
                mutex_.Lock();
            }

            ~MutexLock(){
                mutex_.Unlock();
            }

        private:
            Mutex& mutex_;
        };

    }
}

#endif //YAN_MUTEX_H