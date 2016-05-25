
#include <syscall.h>
#include <zconf.h>
#include <cassert>
#include "thread.h"
#include "log.h"

namespace Yan {

    namespace Common {

        __thread int t_cached_pid_t = 0;

        int Thread::GetPid(){
            if(t_cached_pid_t == 0){
                t_cached_pid_t = static_cast<int>(::syscall(SYS_gettid));
                LOG_TRACE("Thread::GetPid : %d\n", t_cached_pid_t);
            }
            return t_cached_pid_t;
        }

        Thread::Thread(const ThreadFunc& tf_)
                :pid_ (0),
                 threadFunc_(tf_),
                 isStarted_(false),
                 isJoined_(false){
        }

        Thread::~Thread(){
            if (isStarted_ && !isJoined_) {
                pthread_detach(pid_);
            }
        }

        struct ThreadData {
            Thread::ThreadFunc threadFunc;
            ThreadData(Thread::ThreadFunc tf)
                    :threadFunc(tf){

            }
        };

        static void* startThread(void* data){
            ThreadData* td = reinterpret_cast<ThreadData*>(data);
            td->threadFunc();
            delete td;
            return NULL;
        }

        void Thread::Start(){
            assert(!isStarted_);
            isStarted_ = true;

            ThreadData* data = new ThreadData(this->threadFunc_);
            LOG_TRACE("Thread::Start : %d\n", GetPid());
            pthread_create(&pid_, NULL, &startThread, data);

        }

        void Thread::Join(){
            assert(isStarted_);
            assert(!isJoined_);
            isJoined_ = true;
            LOG_TRACE("Thread::Join : %d\n", GetPid());
            pthread_join(pid_, NULL);
        }

    }

}