
#ifndef YAN_INET_ADDRESS_H
#define YAN_INET_ADDRESS_H

#include <arpa/inet.h>
#include <netinet/in.h>
#include <string>

/*
      struct sockaddr_in{
          sa_family_t  sin_family;  // address family : AF_INET
          uint16_t     sin_port;    // port in network byte order
          struct in_addr  sin_addr; // internet address
      };

      typedef uint32_t in_addr_t
      struct  in_addr{
          in_addr_t   s_addr;
      };
*/

namespace Yan {

        class InetAddress {
        public:
            typedef uint16_t  Port;

            InetAddress(Port port){
                inet_addr_.sin_family = AF_INET;
                inet_addr_.sin_addr.s_addr = INADDR_ANY;
                inet_addr_.sin_port = port;
            }

            InetAddress(const std::string& ip, Port port){
                inet_addr_.sin_family = AF_INET;
                inet_addr_.sin_port = htons(port);
                ::inet_pton(AF_INET, ip.c_str(), &inet_addr_.sin_addr);
            }

            InetAddress(const struct sockaddr_in& inet_addr)
                    :inet_addr_(inet_addr){
            }

            std::string GetIp() const {
                char buf[32];
                ::inet_ntop(AF_INET, &inet_addr_.sin_addr, buf, sizeof(buf));
                return buf;
            }

            uint16_t GetPort() const {
                return inet_addr_.sin_port;
            }

            const struct sockaddr_in& Sockaddr() const {
                return inet_addr_;
            }

            std::string ToString() const {
                return GetIp() + " : " + std::to_string(inet_addr_.sin_port);
            }

            void SetSockAddr(const struct sockaddr_in& sockaddr){
                inet_addr_ = sockaddr;
            }

        private:
            struct sockaddr_in inet_addr_;
        };

}

#endif //YAN_INET_ADDRESS_H