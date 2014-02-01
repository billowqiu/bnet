#include "bnet/tcpchannel.h"
using namespace boost::asio::ip;

namespace bnet
{

int PrefixLenParser(const char* buf, std::size_t size)
{
    if(size >= sizeof(uint32_t))
    {
        uint32_t packet_len;
        memcpy(&packet_len, buf, sizeof(uint32_t));
        packet_len = ntohl(packet_len);
        if(packet_len > 1024*10)
        {
            return -1;
        }
        //有一个完整包
        if(packet_len <= size)
        {
            return packet_len;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return 0;
    }
}

TCPChannel::TCPChannel(AsyncProcessor* processor):
ower_processor_(processor),
socket_(processor->Service()),
connected_(false),
recvbuf_offset_(0),
sendbuf_offset_(0)
{
	recvbuf_.reserve(1024*10);
	sendbuf_.reserve(1024*10);
}

TCPChannel::~TCPChannel()
{
}

void TCPChannel::Close()
{
    if(socket_.is_open())
    {
        //先shutdown再close,asio文档上面有说
        boost::system::error_code error;
        socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_send, error);
        socket_.close();

        OnClose(error);
    }
}

bool TCPChannel::Connected() const
{
    return socket_.is_open() && connected_;
}

std::size_t TCPChannel::SyncWrite(const char* buffer,
                                  std::size_t size,
                                  boost::system::error_code& ec)
{
    return boost::asio::write(socket_, boost::asio::buffer(buffer, size), ec);
}

void TCPChannel::OnConnect()
{
    boost::system::error_code ec;
    tcp::endpoint remote_peer = socket_.remote_endpoint(ec);
    if (!ec)
    {
        remote_address_ = remote_peer.address().to_string(ec);
        remote_port_ = remote_peer.port();

        connected_ = true;
    }
}

void TCPChannel::OnClose(const boost::system::error_code& ec)
{
    connected_ = false;
}

}
