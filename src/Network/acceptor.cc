
#include <fcntl.h>
#include <zconf.h>
#include "acceptor.h"
#include "Common/log.h"

namespace Yan {

    Acceptor::Acceptor(EventPool *eventPool)
        :isBind_(false),
         acceptSocket_(Socket::CreateNonblockingSocket()),
         acceptChannel_(acceptSocket_.GetFd(), eventPool),
         eventPool_(eventPool),
         idleFd_(::open("/dev/null", O_RDONLY | O_CLOEXEC))
    {
        acceptChannel_.SetReadCallback(std::bind(&Acceptor::OnListen, this));
    }

    Acceptor::~Acceptor() {
        acceptChannel_.Unregister();
    }

    void Acceptor::SetAndBind(const InetAddress& bindAddr) {
        isBind_ = true;
        acceptSocket_.SetReuseAddr(true);
        acceptSocket_.SetTcpNoDelay(true);
        acceptSocket_.Bind(bindAddr);
    }

    void Acceptor::Listen() {
        if(isBind_){
            acceptSocket_.Listen();
            acceptChannel_.Register();
        }
    }

    void Acceptor::OnListen() {
        InetAddress peeraddr(0);
        int newFd = acceptSocket_.Accept(&peeraddr);

        if (newFd >= 0) {
            LOG_TRACE("New connection, fd = %d\n", newFd);
            if (onNewConnectionCallback_) {
                onNewConnectionCallback_(newFd, peeraddr);
            } else {
                ::close(newFd);
                LOG_WARN("On new connection callback should be set");
            }
        } else {
            LOG_WARN("acceptSocket.Accept failed, fd=%d\n", acceptSocket_.GetFd());
            if (errno == EMFILE) {
                LOG_FATAL("Reached max file descriptor\n");
                ::close(idleFd_);
                idleFd_ = ::accept(acceptSocket_.GetFd(), NULL, NULL);
                ::close(idleFd_);
                idleFd_ = ::open("/dev/null", O_RDONLY | O_CLOEXEC);
            }
        }
    }
}

