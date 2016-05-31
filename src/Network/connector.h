
#ifndef YAN_CONNECTOR_H
#define YAN_CONNECTOR_H

#include <atomic>

#include <boost/scoped_ptr.hpp>

#include "Common/noncopyable.h"
#include "inet_address.h"
#include "eventpool.h"

namespace Yan {

    class Connector : noncopyable{
    public:
        typedef std::function<void(int)> OnNewConnectionCallback;

        Connector(const InetAddress& inetAddress, EventPool* eventPool);

        void Connect();

        void SetOnNewConnectionCallback(const OnNewConnectionCallback& cb){
            onNewConnectionCallback_ = cb;
        }

    private:
        void queueConnect();
        void onConnect();

        volatile std::atomic_bool isConnected_;
        InetAddress remote_address_;
        EventPool* eventPool_;
        boost::scoped_ptr<Channel> conn_channel_;
        OnNewConnectionCallback onNewConnectionCallback_;
    };

}

#endif //YAN_CONNECTOR_H
