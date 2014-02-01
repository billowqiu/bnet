#include "echo_client.h"
#include <boost/thread/thread.hpp>
#include <iostream>
#include "addressbook.pb.h"

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
    tutorial::AddressBook addrbook;
    bool ret = addrbook.ParseFromString(packet.substr(4));
    if(ret)
    {
        LOG << addrbook.DebugString() << std::endl;
    }
    Write(packet.data(), packet.size());
	Close();
	
    return 0;
}

void EchoSession::OnClose(const boost::system::error_code& ec)
{
    TCPConnection::OnClose(ec);
    LOG << "connect close: " << ec.message() << std::endl;
}
