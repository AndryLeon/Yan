
#include <zconf.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <errno.h>
#include "socket.h"
#include "inet_address.h"
#include "Common/log.h"

namespace Yan {

    Socket::Socket(int fd)
            :socketFd_(fd){
    }

    Socket::~Socket(){
        ::close(socketFd_);
    }

    //创建非阻塞IO
    int Socket::CreateNonblockingSocket() {
        int fd = ::socket(AF_INET, SOCK_NONBLOCK | SOCK_CLOEXEC | SOCK_STREAM, IPPROTO_TCP);
        if(fd < 0) {
            LOG_FATAL("Socket::CreatrNonblockingSocket, error: %s\n", strerror(errno));
        }
        return fd;
    }

    int Socket::Connect(int fd, const struct sockaddr_in &addr) {
        return ::connect(fd, reinterpret_cast<const sockaddr*>(&addr), sizeof(addr));
    }

    struct sockaddr_in Socket::GetLocalSockAddr(int sockfd){
        struct sockaddr_in sa;
        socklen_t len = sizeof(sa);

        bzero(&sa, sizeof(sa));
        if (::getsockname(sockfd, reinterpret_cast<sockaddr*>(&sa), &len) < 0) {
            LOG_WARN("Socket::GetLocalSockAddr, error: %s\n", strerror(errno));
        }
        return sa;
    }

    struct sockaddr_in Socket::GetPeerSockAddr(int sockfd){
        struct sockaddr_in sa;
        socklen_t len = sizeof(sa);

        bzero(&sa, sizeof(sa));
        if (::getpeername(sockfd, reinterpret_cast<sockaddr*>(&sa), &len) < 0) {
            LOG_WARN("Socket::GetPeerSockAddr, error: %s\n", strerror(errno));
        }
        return sa;
    }

    int Socket::GetFd() const {
        return socketFd_;
    }

    void Socket::Bind(const InetAddress& address){
        if(::bind(socketFd_, reinterpret_cast<const sockaddr*>(&address.GetSockaddr()),
                  static_cast<socklen_t>(sizeof(sockaddr_in))) < 0){
            LOG_WARN("Socket::Bind, error: %s\n", strerror(errno));
        }
    }

    void Socket::Listen(){
        if(::listen(socketFd_, SOMAXCONN) < 0){
            LOG_WARN("Socket::Listen, error: %s\n", strerror(errno));
        }
    }

    int Socket::Accept(InetAddress *peer_address) {
        struct sockaddr_in sa;
        socklen_t len = sizeof(sa);
        int newFd;
        while(true){
            bzero(&sa, sizeof(sa));
            newFd = ::accept4(socketFd_, reinterpret_cast<sockaddr*>(&sa), &len,
                              SOCK_NONBLOCK | SOCK_CLOEXEC);

            if( newFd < 0 ){
                if (errno == EAGAIN || errno == ECONNABORTED){

                }else{
                    LOG_WARN("Socket::Accept, error: %s", strerror(errno));
                    break;
                }
            }else{
                peer_address->SetSockAddr(sa);
                break;
            }
        }
        return newFd;
    }

    void Socket::SetTcpNoDelay(bool opt) {
        int tmp =  opt ? 1 : 0;
        ::setsockopt(socketFd_, IPPROTO_TCP, TCP_NODELAY, &tmp, sizeof(tmp));
    }

    void Socket::SetReuseAddr(bool opt){
        int tmp = opt ? 1 : 0;
        ::setsockopt(socketFd_, SOL_SOCKET, SO_REUSEADDR, &tmp, sizeof(tmp));
    }

    void Socket::SetReusePort(bool opt) {
        #ifdef SO_REUSEPORT
            int tmp = opt ? 1 : 0;
            ::setsockopt(socketFd_, SOL_SOCKET, SO_REUSEPORT, &tmp, sizeof(tmp));
        #endif
    }

    void Socket::SetKeepAlive(bool opt) {
        int tmp = opt ? 1 : 0;
        ::setsockopt(socketFd_, SOL_SOCKET, SO_KEEPALIVE, &tmp, sizeof(tmp));
    }

}