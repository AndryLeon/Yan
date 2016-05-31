
#ifndef YAN_CONDITION_H
#define YAN_CONDITION_H

#include <pthread.h>
#include <stdint.h>
#include <errno.h>
#include "mutex.h"

namespace Yan {

    namespace Common {

        class Condition : noncopyable {
        public:
            explicit Condition(Mutex &mutex)
                    : mutex_(mutex) {
                pthread_cond_init(&pcond_, NULL);
            }

            ~Condition() {
                pthread_cond_destroy(&pcond_);
            }

            void Wait() {
                pthread_cond_wait(&pcond_, &mutex_.mutex_);
            }

            bool WaitForSeconds(double seconds){
                struct timespec abstime;

                clock_gettime(CLOCK_REALTIME, &abstime);

                const int64_t kNanoSecondPersSecond = 1e9;

                int64_t nanoseconds = static_cast<int64_t>(seconds * kNanoSecondPersSecond);

                abstime.tv_sec += static_cast<time_t>((abstime.tv_nsec + nanoseconds) / kNanoSecondPersSecond);

                abstime.tv_nsec = (abstime.tv_nsec + nanoseconds) % kNanoSecondPersSecond;

                return ETIMEDOUT == pthread_cond_timedwait(&pcond_, &mutex_.mutex_, &abstime);
            }

            void Signal() {
                pthread_cond_signal(&pcond_);
            }

            void Broadcast() {
                pthread_cond_broadcast(&pcond_);
            }

        private:
            Mutex &mutex_;
            pthread_cond_t pcond_;
        };
    }
}

#endif //YAN_CONDITION_H