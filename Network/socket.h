//
// Created by andryleon.
// Email : andryleon@163.com
//

#ifndef YAN_SOCKET_H
#define YAN_SOCKET_H

namespace Yan {

        class InetAddress;

        class Socket {
        public:
            Socket(int fd);
            ~Socket();

            static int CreateNonblockingSocket();
            static int Connect(int fd, const struct sockaddr_in& addr);
            static struct sockaddr_in GetLocalSockAddr(int sockfd);
            static struct sockaddr_in GetPeerSockAddr(int sockfd);

            int GetFd() const;

            void Bind(const InetAddress& address);
            void Listen();
            int Accept(InetAddress* peer_address);

            void SetTcpNoDelay(bool opt);
            void SetReuseAddr(bool opt);
            void SetReusePort(bool opt);
            void SetKeepAlive(bool opt);

        private:
            const int socketFd_;
        };

}

#endif //YAN_SOCKET_H