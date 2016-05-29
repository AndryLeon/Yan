
#ifndef YAN_ACCEPTOR_H
#define YAN_ACCEPTOR_H

#include <functional>
#include <atomic>
#include "../Common/noncopyable.h"
#include "inet_address.h"
#include "socket.h"
#include "channel.h"

namespace Yan {

    class Acceptor : noncopyable {
    public:
        typedef std::function<void(int, const InetAddress&)> OnNewConnectionCallback;

        Acceptor(EventPool* eventPool);
        ~Acceptor();

        void SetAndBind(const InetAddress& bindAddr);
        void Listen();

        void SetOnNewConnectionCallback(const OnNewConnectionCallback& cb){
            onNewConnectionCallback_ = cb;
        }
    private:
        void OnListen();

        volatile std::atomic_bool isBind_;
        Socket acceptSocket_;
        Channel acceptChannel_;
        EventPool* eventPool_;
        int idleFd_;
        OnNewConnectionCallback onNewConnectionCallback_;
    };

}

#endif //YAN_ACCEPTOR_H
