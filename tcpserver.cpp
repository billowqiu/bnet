#include "bnet/tcpserver.h"

#include <boost/bind.hpp>
#include <boost/checked_delete.hpp>

#include "bnet/asyncprocessor.h"
#include "bnet/tcpconnection.h"
using namespace boost::asio::ip;

namespace bnet 
{

TCPServer::TCPServer(AsyncProcessor* processor, uint16_t port):
base_processor_(processor),
acceptor_(base_processor_->Service(), tcp::v4()),
port_(port)
{
}

TCPServer::~TCPServer()
{
}

void TCPServer::SetThreadNum( std::size_t numthreads )
{
    processor_pool_.SetPoolSize(numthreads);
}

void TCPServer::SetListenPort( uint16_t newport )
{
    port_ = newport;
}

uint16_t TCPServer::GetListenPort() const
{
    return port_;
}

void TCPServer::Start()
{
    boost::asio::ip::tcp::acceptor::reuse_address reuse_addr(true);
    boost::system::error_code ec;
    acceptor_.set_option(reuse_addr, ec);
    boost::asio::detail::throw_error(ec, "reuse_address");
    
    tcp::endpoint bindaddr(tcp::v4(), port_);    
    acceptor_.bind(bindaddr, ec);
    boost::asio::detail::throw_error(ec, "bind");
    
    acceptor_.listen(boost::asio::socket_base::max_connections, ec);    
    boost::asio::detail::throw_error(ec, "listen");
    
    AsyncAccept();
    //最后再启动线程池
    processor_pool_.Start();
}

void TCPServer::Stop()
{
    processor_pool_.Stop();
    
    acceptor_.close();    
    //删除所有的
    for (std::set<TCPConnection*>::iterator it=sessions_set_.begin(); 
         it != sessions_set_.end(); ++it)
    {
        //最终会调用DestroyConnection
        (*it)->Close();
    }
    sessions_set_.clear();       
}

void TCPServer::AsyncAccept()
{
    //选取一个processor
    AsyncProcessor* processor = processor_pool_.GetAsyncProcessor();
    if (NULL == processor)
    {
        processor = base_processor_;
    }
    
    TCPConnection* pSession = CreateConnection(processor);
    
    acceptor_.async_accept(pSession->socket(), 
                           boost::bind(&TCPServer::HandleAccept, 
                           this, 
                           pSession, 
                           boost::asio::placeholders::error));
}

void TCPServer::HandleAccept(TCPConnection* pSession, 
                             const boost::system::error_code& error)
{
    if(!error)
    {
        sessions_set_.insert(pSession);        
        pSession->OnConnect();
        //继续下一个链接
        AsyncAccept();
    }
    else
    {
        pSession->OnClose(error);
        delete pSession;
    }
}

void TCPServer::CloseAcceptor()
{
    acceptor_.close();
}

void TCPServer::DestroyConnection(TCPConnection* pSocketSession)
{
    sessions_set_.erase(pSocketSession);
    //删除了
    delete pSocketSession;
}

}
