#include "echo_client.h"
#include <boost/thread/thread.hpp>
#include <iostream>

EchoSession::EchoSession( bnet::AsyncProcessor* processor, bnet::TCPServer* server ):
bnet::TCPConnection(processor, server)
{

}

void EchoSession::OnConnect()
{
    std::cout << __FUNCTION__ <<  " current thread:" << boost::this_thread::get_id() << std::endl;
    //先读取16字节的包长度信息
    ReadSome(recv_buf_, 1024,
        boost::bind(&EchoSession::HandleRead, this,
        boost::asio::placeholders::error,
        boost::asio::placeholders::bytes_transferred));  
}

void EchoSession::HandleRead( const boost::system::error_code& e, std::size_t bytes_transferred )
{
    std::cout << __FUNCTION__ <<  " current thread:" << boost::this_thread::get_id() << std::endl;
    if (!e)
    {
        //echo 
        socket_.write_some(boost::asio::buffer(recv_buf_, bytes_transferred));
        ReadSome(recv_buf_, 1024,
            boost::bind(&EchoSession::HandleRead, this,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));  
    }
}
