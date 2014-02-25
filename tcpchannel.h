//
// Copyright (c) billowqiu (billowqiu@163.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//  See www.boost.org/libs/asio for documentation.
//

#ifndef BNET_TCPCHANNEL_H_
#define BNET_TCPCHANNEL_H_
#include <string>
#include <iostream>
#include <deque>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/noncopyable.hpp>
#include "bnet/asyncprocessor.h"

#define LOG std::cerr << __FILE__ << ":" <<  __LINE__ << " " <<  __FUNCTION__ << "---"

namespace bnet
{

/**
包解析器
0   数据包还未接收完整，继续接收
>0  表示数据已经接收完整且该值表示数据包的长度
<0  表示出错
 */
typedef boost::function<int (const char*, std::size_t)> ProtoParserFunc;
typedef boost::function<void (const boost::system::error_code, std::size_t)> AsyncIoHandler;

int PrefixLenParser(const char* buf, std::size_t size);

class TCPChannel : public boost::noncopyable
{
protected:
    TCPChannel(AsyncProcessor* processor);
    virtual ~TCPChannel();

public:
    void SetProtoParser(const ProtoParserFunc& parser)
    {
        parser_ = parser;
    }

    virtual int ProcessPacket(const std::string& packet) = 0;

    void InitRead()
    {
        AsyncReadSome(temp_recvbuf_, 2048, boost::bind(&TCPChannel::HandleInput, this, _1, _2));
    }

    void HandleInput(const boost::system::error_code& error, std::size_t bytes_transferred);

    void HandleOutput(const boost::system::error_code& error, std::size_t bytes_transferred);

    /**
     *  异步发送数据
     *  @param[in] buffer.
     *  @param[in] size.
     *  @return void.
    */
    void Write(const char* buffer, std::size_t size);

    /**
     *  同步发送数据
     *  @param[in] buffer.
     *  @param[in] size.
     *  @return int.
    */
    std::size_t SyncWrite(const char* buffer, std::size_t size, boost::system::error_code& ec);

    bool Connected() const;
    virtual void Close();

protected:
    boost::asio::ip::tcp::socket& socket()
    {
        return socket_;
    }

    virtual void OnConnect();
    virtual void OnClose(const boost::system::error_code& ec);

private:
    void AsyncReadSome(char* buffer, std::size_t size,  const AsyncIoHandler& handler);
    void AsyncWriteSome(const char* buffer, std::size_t size, const AsyncIoHandler& handler);

protected:
    AsyncProcessor* ower_processor_;
    boost::asio::ip::tcp::socket socket_;
    bool connected_;
    std::string remote_address_;
    uint16_t remote_port_;
    std::string recvbuf_;
    //临时的收包buf
    char temp_recvbuf_[2048];
    //发送消息队列
    typedef std::deque<std::string> send_message_queue;
    send_message_queue send_messages_;
    std::string sendbuf_;
    ProtoParserFunc parser_;
};

}
#endif // BNET_TCPCHANNEL_H_
