
#include "../Network/tcpclient.h"

using namespace Yan;

class EchoClient {
public:
    EchoClient(EventPool* eventPool, const InetAddress& remoteAddr)
            :eventPool_(eventPool),
             tcpClient_(remoteAddr, eventPool){

    }

    ~EchoClient(){
        tcpClient_.DisConnect();
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


