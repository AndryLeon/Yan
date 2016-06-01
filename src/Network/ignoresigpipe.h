

#ifndef YAN_IGNORESIGPIPE_H
#define YAN_IGNORESIGPIPE_H

#include <signal.h>

namespace Yan {
    class IgnoreSigPipe {
    public:
        IgnoreSigPipe(){
            ::signal(SIGPIPE, SIG_IGN);
        }

    };
}



#endif //YAN_IGNORESIGPIPE_H
