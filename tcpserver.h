//
// Copyright (c) billowqiu (billowqiu@163.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//  See www.boost.org/libs/asio for documentation.
//

#ifndef BNET_TCPSERVER_H_
#define BNET_TCPSERVER_H_

#include <stdint.h>
#include <set>
#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>
#include "bnet/processorpool.h"
namespace bnet 
{

class AsyncProcessor;
class TCPConnection;

class TCPServer : public boost::noncopyable
{
public:
    TCPServer(AsyncProcessor* processor, uint16_t port);
    virtual ~TCPServer();

public:
    /** 
     *  设置处理I/O的线程数，必须在Start之前调用
     *  @param[in] numthreads.
     *  0,所有的I/O都在当前线程中
     *  >0,出accept新连接外，所有I/O通过round-robin算法选取一个processor处理
     *  @return void.
    */
    void SetThreadNum(std::size_t numthreads);
    void Start();
    void Stop();
    void CloseAcceptor();
    void DestroyConnection(TCPConnection* pSocketSession);
    
private:
    void AsyncAccept();
    void HandleAccept(TCPConnection* pSession, 
                      const boost::system::error_code& error);

protected:
    virtual TCPConnection* CreateConnection(AsyncProcessor* processor) = 0;

private:
    AsyncProcessor* base_processor_;    //主要作为accept
    ProcessorPool processor_pool_;
    boost::asio::ip::tcp::acceptor acceptor_;    
    std::set<TCPConnection*> sessions_set_;
};
}

#endif // BNET_TCPSERVER_H_
