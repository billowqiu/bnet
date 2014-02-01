#ifndef ECHO_CLIENT_H_
#define ECHO_CLIENT_H_

#include <bnet/tcpconnection.h>

class EchoSession : public bnet::TCPConnection
{
public:
    EchoSession(bnet::AsyncProcessor* processor, bnet::TCPServer* server);

protected:
    virtual void OnConnect();
    virtual int ProcessPacket(const std::string& packet);
    virtual void OnClose(const boost::system::error_code& ec);
};

#endif // ECHO_CLIENT_H_

