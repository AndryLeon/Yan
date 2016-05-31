
#include "../src/Network/tcpserver.h"

using namespace Yan;

class EchoServer {
public:
    EchoServer(EventPool* eventPool, const InetAddress& bindAddress)
        :eventPool_(eventPool),
         tcpServer_(bindAddress, eventPool)
    {
        tcpServer_.SetConnectionCallback(std::bind(&EchoServer::OnConnection, this, std::placeholders::_1));
        tcpServer_.SetReadCallback(std::bind(&EchoServer::OnRead, this, std::placeholders::_1, std::placeholders::_2));
    }

    void Start(){
        printf("Start EchoServer\n");
        tcpServer_.Start();
    }

private:
    void OnConnection(const TcpConnectionPtr& conn){
        LOG_TRACE("Get a new Connection, connection ID:%d\n", conn->GetID());
    }

    void OnRead(const TcpConnectionPtr& conn, Buffer* buffer){
        std::string s(buffer->RetriveString());
        LOG_TRACE("ID:%d Get:%s\n", conn->GetID(), s.c_str());
        conn->Write(s);
    }

    EventPool* eventPool_;
    TcpServer tcpServer_;
};


int main(){
    EventPool event_pool(1, 2);
    event_pool.Start();

    InetAddress bindaddr("127.0.0.1", 9527);
    EchoServer server(&event_pool, bindaddr);
    server.Start();

    while (true) {
        ::usleep(1000);
    }

}