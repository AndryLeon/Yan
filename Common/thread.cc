
#include <syscall.h>
#include <zconf.h>
#include <cassert>
#include "thread.h"
#include "log.h"

namespace Yan {

    namespace Common {

        namespace {
            __thread int t_cached_tid_t = 0;
        }

        static void CacheTid() {
            if(t_cached_tid_t == 0) {
                t_cached_tid_t = static_cast<int>(::syscall(SYS_gettid));
            }
        }

        int Thread::GetPid(){
            CacheTid();
            return t_cached_tid_t;
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
            pthread_create(&pid_, NULL, &startThread, data);
            LOG_TRACE("Thread::Start : %d, pid : %d\n", GetPid(), pid_);
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