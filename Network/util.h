
#ifndef YAN_UTIL_H
#define YAN_UTIL_H

namespace Yan {

    class Buffer;
    class TcpConnection;

    typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;

    typedef std::function<void (const TcpConnectionPtr&)> ConnectionCallback;
    typedef std::function<void (const TcpConnectionPtr&)> WriteCompleteCallback;
    typedef std::function<void (const TcpConnectionPtr&, Buffer*)> ReadCompleteCallback;
    typedef std::function<void (const TcpConnectionPtr&)> CloseCallback;
}

#endif //YAN_UTIL_H
