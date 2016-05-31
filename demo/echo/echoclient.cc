
#include "../src/Network/tcpclient.h"

using namespace Yan;

class EchoClient {
public:
    EchoClient(EventPool* eventPool, const InetAddress& remoteAddr)
            :eventPool_(eventPool),
             tcpClient_(remoteAddr, eventPool){
        tcpClient_.SetConnectionCallback(
                std::bind(&EchoClient::OnConnection, this, std::placeholders::_1));
        tcpClient_.SetReadCallback(
                std::bind(&EchoClient::OnRead, this, std::placeholders::_1, std::placeholders::_2));
    }

    ~EchoClient(){
        tcpClient_.Disconnect();
    }

    void Connect(){
        tcpClient_.Connect();
    }

private:
    void OnConnection(const TcpConnectionPtr& conn){
        LOG_TRACE("ID:%d\n", conn->GetID());
        conn->Write("hello");
    }

    void OnRead(const TcpConnectionPtr& conn, Buffer* buffer){
        std::string s(buffer->RetriveString());
        LOG_TRACE("ID:%d Get:%s\n", conn->GetID(), s.c_str());
        conn->Write(s);
    }

    EventPool* eventPool_;
    TcpClient tcpClient_;
};


int main() {
    EventPool event_pool(1, 2);
    event_pool.Start();

    InetAddress remote("127.0.0.1", 9527);
    EchoClient client(&event_pool, remote);
    client.Connect();

    while (true) {
        ::usleep(1000);
    }
}
