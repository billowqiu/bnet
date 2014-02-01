#include "echo_client.h"
#include <boost/thread/thread.hpp>
#include <iostream>

EchoSession::EchoSession( bnet::AsyncProcessor* processor, bnet::TCPServer* server ):
bnet::TCPConnection(processor, server)
{

}

void EchoSession::OnConnect()
{
    LOG << "current thread:" << boost::this_thread::get_id() << std::endl;
    InitRead();
}

int EchoSession::ProcessPacket(const std::string& packet)
{
    LOG << "current thread: " << boost::this_thread::get_id()
        << " pack size: "     << packet.size() << std::endl;
    socket_.write_some(boost::asio::buffer(packet.data(), packet.size()));

    return 0;
}

void EchoSession::OnClose(const boost::system::error_code& ec)
{
    TCPConnection::OnClose(ec);
    LOG << "connect close: " << ec.message() << std::endl;
}
