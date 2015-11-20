#include "bnet/tcpchannel.h"
#include <glog/logging.h>
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
connected_(false)
{
	recvbuf_.reserve(1024*10);
	sendbuf_.reserve(1024*10);
}

TCPChannel::~TCPChannel()
{
}

void TCPChannel::HandleInput(const boost::system::error_code& error, std::size_t bytes_transferred)
{
    if(!error)
    {
        //parse protocol
        if(bytes_transferred > 0)
        {
            LOG(INFO) << "recv msg size: " << bytes_transferred;

            recvbuf_.append(temp_recvbuf_, bytes_transferred);
            //循环处理收到的包，有可能一次recv收了多个业务包
            while(true)
            {
                if(!parser_)
                {
                    LOG(ERROR) << "protocol parser not set!!!";
                    Close();

                    return;
                }
                int ret = parser_(recvbuf_.data(), recvbuf_.size());
                if(ret >0)
                {
                    std::string packet(recvbuf_.data(), ret);
                    LOG(INFO) << "process a full packet size: " << packet.size();
                    //处理掉
                    ProcessPacket(packet);
                    //调整大小
                    recvbuf_ = recvbuf_.substr(ret);
                    LOG(INFO) << "after process packet recvbuf size: " << recvbuf_.size();
                }
                else if(ret ==0)
                {
                    LOG(INFO) << "the pack is partial, size: " << recvbuf_.size();
                    //一次最多读取2KB大小的包
                    AsyncReadSome(temp_recvbuf_, 2048, boost::bind(&TCPChannel::HandleInput, this, _1, _2));
                    break;
                }
                else
                {
                    //包解析失败，关掉链接吧
                    LOG(ERROR) << "packet parse fail";
                    break;
                }
            }
        }
        else
        {
            //fixme
            LOG(ERROR) << "Fatal error, bytes_transferred is invalid: " << bytes_transferred;
            Close();
        }
    }
    else
    {
        //fixme
        LOG(ERROR) << "Recv msg error: " << error.message();
        Close();
    }
}

void TCPChannel::HandleOutput(const boost::system::error_code& error, std::size_t bytes_transferred)
{
    if(!error)
    {
        //删除队头已经发送完的数据
        send_messages_.pop_front();
        //如果还有数据 ，那么继续发送
        if(!send_messages_.empty())
        {
            //这里采用的发送完所有的数据才回调
            boost::asio::async_write(socket_,
                                     boost::asio::buffer(send_messages_.front().data(),
                                                         send_messages_.front().length()),
                                     boost::bind(&TCPChannel::HandleOutput, this, _1, _2));

        }
    }
    else
    {
        LOG(ERROR) << "send msg error: " << error.message()  << " close it";
        Close();
    }
}

void TCPChannel::Write(const char* buffer, std::size_t size)
{
    //要先判断发送队列是否为空
    bool write_in_progress = !send_messages_.empty();
    send_messages_.push_back(std::string(buffer, size));
    //队列之前没有待发送数据，那么可以再次发送了
    if(!write_in_progress)
    {
        //这里采用的发送完所有的数据才回调
        boost::asio::async_write(socket_,
                                 boost::asio::buffer(send_messages_.front().data(),
                                                     send_messages_.front().length()),
                                 boost::bind(&TCPChannel::HandleOutput, this, _1, _2));
    }
}

void TCPChannel::AsyncReadSome(char* buffer, std::size_t size,  const AsyncIoHandler& handler)
{
    if (socket_.is_open())
    {
        socket_.async_read_some(boost::asio::buffer(buffer, size),handler);
    }
    else
    {
        //throw Exception
        LOG(ERROR) << "socket is not opened!!!";
    }
}

void TCPChannel::AsyncWriteSome(const char* buffer, std::size_t size, const AsyncIoHandler& handler)
{
    if (socket_.is_open())
    {
        socket_.async_write_some(boost::asio::buffer(buffer, size),handler);
    }
    else
    {
        //throw Exception
        LOG(ERROR) << "socket is not opened!!!";
    }
}

void TCPChannel::Close()
{
    if(socket_.is_open())
    {
        //先shutdown再close,asio文档上面有说
        boost::system::error_code error;
        socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_send, error);
        socket_.close();
        //callback
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

