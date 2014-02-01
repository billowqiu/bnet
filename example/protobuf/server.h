#ifndef SERVER_H_
#define SERVER_H_

#include <bnet/tcpserver.h>
class EchoServer : public bnet::TCPServer
{
public:
    EchoServer(bnet::AsyncProcessor* processor, uint16_t port);

protected:
    virtual bnet::TCPConnection* CreateConnection(bnet::AsyncProcessor* processor);
};

#endif // SERVER_H_

