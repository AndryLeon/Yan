
#ifndef YAN_THREAD_H
#define YAN_THREAD_H

#include <functional>
#include "noncopyable.h"

namespace Yan {

    namespace Common {

        class Thread : noncopyable {
        public:
            typedef std::function<void()> ThreadFunc;

            static int GetPid();

            explicit Thread(const ThreadFunc& tf_);
            ~Thread();

            void Start();
            void Join();

        private:
            pthread_t pid_;
            ThreadFunc threadFunc_;
            bool isStarted_;
            bool isJoined_;
        };

    }

}

#endif //YAN_THREAD_H
