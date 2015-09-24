#include "bnet/tcpconnection.h"
#include "bnet/tcpserver.h"
using namespace boost::asio::ip;

namespace bnet
{

TCPConnection::TCPConnection(AsyncProcessor* processor, TCPServer* server):
TCPChannel(processor),
socket_server_ (server)
{
}

TCPConnection::~TCPConnection()
{
}

void TCPConnection::Close()
{
    TCPChannel::Close();
    socket_server_->DestroyConnection(this);
}

}

