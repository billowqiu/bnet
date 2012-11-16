//
// Copyright (c) billowqiu (billowqiu@163.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//  See www.boost.org/libs/asio for documentation.
//

#ifndef BNET_TCPCLIENT_H
#define BNET_TCPCLIENT_H

#include <boost/asio.hpp>
#include "bnet/tcpchannel.h"
namespace bnet
{

class AsyncProcessor;
class TCPClient : public TCPChannel
{
public:
    TCPClient(AsyncProcessor* processor, 
              const std::string& host, 
              const std::string& port);
    void SyncConnect();
    void AsyncConnect();

private:
    void HandleResolve(const boost::system::error_code& error, 
                       boost::asio::ip::tcp::resolver::iterator it_endpoint);
    void HandleConnect(const boost::system::error_code& error, 
                       boost::asio::ip::tcp::resolver::iterator it_endpoint);

private:
    boost::asio::ip::tcp::resolver         resolver_;
    boost::asio::ip::tcp::resolver::query  resolver_query_;

protected:
    std::string server_address_;
    uint16_t server_port_;
};

}

#endif // BNET_TCPCLIENT_H
