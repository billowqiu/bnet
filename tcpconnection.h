//
// Copyright (c) billowqiu (billowqiu@163.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//  See www.boost.org/libs/asio for documentation.
//

#ifndef BNET_TCPCONNECTION_H_
#define BNET_TCPCONNECTION_H_

#include <stdint.h>
#include "bnet/tcpchannel.h"

namespace bnet
{
class TCPServer;

class TCPConnection : protected TCPChannel
{
private:
    friend class TCPServer;

public:
    TCPConnection(AsyncProcessor* processor, TCPServer* server);
    virtual ~TCPConnection();

protected:
    virtual void Close();

private:
    TCPServer* socket_server_;
};

}

#endif // BNET_TCPCONNECTION_H_
