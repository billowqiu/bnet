#include "echo_client.h"
#include <boost/thread/thread.hpp>
#include <glog/logging.h>
#include <iostream>

EchoSession::EchoSession( bnet::AsyncProcessor* processor, bnet::TCPServer* server ):
bnet::TCPConnection(processor, server)
{

}

void EchoSession::OnConnect()
{
    LOG(INFO) << "current thread:" << boost::this_thread::get_id();
    InitRead();
}

int EchoSession::ProcessPacket(const std::string& packet)
{
    LOG(INFO) << "current thread: " << boost::this_thread::get_id() << " pack size: "     << packet.size();
    Write(packet.data(), packet.size());

    return 0;
}

void EchoSession::OnClose(const boost::system::error_code& ec)
{
    TCPConnection::OnClose(ec);
    LOG(ERROR) << "connect close: " << ec.message();
}

