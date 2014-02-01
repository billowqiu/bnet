#include "server.h"
#include "echo_client.h"

static int EchoParser(const char* buf, std::size_t size)
{
    return size;
}

EchoServer::EchoServer( bnet::AsyncProcessor* processor, uint16_t port ):
bnet::TCPServer(processor, port)
{
}

bnet::TCPConnection* EchoServer::CreateConnection(bnet::AsyncProcessor* processor)
{
    bnet::TCPConnection* conn = new EchoSession(processor, this);
    conn->SetProtoParser(EchoParser);

    return conn;
}

