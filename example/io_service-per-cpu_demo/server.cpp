#include "server.h"
#include "echo_client.h"

EchoServer::EchoServer( bnet::AsyncProcessor* processor, uint16_t port ):
bnet::TCPServer(processor, port)
{

}

bnet::TCPConnection* EchoServer::BuildSession( bnet::AsyncProcessor* processor )
{
    return new EchoSession(processor, this);
}
