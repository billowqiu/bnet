#include "bnet/tcpclient.h"
#include <boost/bind.hpp>
using namespace boost::asio::ip;

namespace bnet
{

TCPClient::TCPClient(AsyncProcessor* processor, 
                     const std::string& host, 
                     const std::string& port):
TCPChannel(processor), 
resolver_(processor->Service()), 
resolver_query_(host, port),
server_address_(host),
server_port_(::atoi(port.c_str()))
{
}

void TCPClient::HandleResolve(const boost::system::error_code& error, 
                              tcp::resolver::iterator it_endpoint)
{
    if (!error)
    {
        tcp::endpoint endpoint = *it_endpoint;
        socket_.async_connect(endpoint, boost::bind(&TCPClient::HandleConnect, this, 
                              boost::asio::placeholders::error, 
                              ++it_endpoint));
    }
    else
    {
        OnClose(error);
    }
}

void TCPClient::HandleConnect(const boost::system::error_code& error, 
                              tcp::resolver::iterator it_endpoint)
{
    if (!error)
    {
        OnConnect();
    }
    //继续连接下一个endpoint
    else if (it_endpoint != tcp::resolver::iterator())
    {
        Close();
        tcp::endpoint endpoint = *it_endpoint;
        socket_.async_connect(endpoint, boost::bind(&TCPClient::HandleConnect, this, 
                              boost::asio::placeholders::error, 
                              ++it_endpoint));
    }
    else
    {
        OnClose(error);
    }
}

void TCPClient::SyncConnect()
{
    tcp::resolver::iterator it = resolver_.resolve(resolver_query_);
    tcp::resolver::iterator itEnd;
    boost::system::error_code ec;
    while (!socket_.is_open() && it != itEnd)
    {
        socket_.close();
        //connect内部发现没有打开socket,会自动打开
        socket_.connect(*it, ec);

        if (!ec)
        {
            OnConnect();
        }
        else
        {
            OnClose(ec);
        }
    }
}

void TCPClient::AsyncConnect()
{
    resolver_.async_resolve(resolver_query_, boost::bind(&TCPClient::HandleResolve, this, 
                            boost::asio::placeholders::error, 
                            boost::asio::placeholders::iterator));
}

}
