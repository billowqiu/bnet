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
acceptor_(base_processor_->Service(), tcp::endpoint(tcp::v4(), port))
{
}

TCPServer::~TCPServer()
{
}

void TCPServer::SetThreadNum( std::size_t numthreads )
{
    processor_pool_.SetPoolSize(numthreads);
}

void TCPServer::Start()
{
    processor_pool_.Start();
    AsyncAccept();
}

void TCPServer::Stop()
{
    acceptor_.close();    
    //删除所有的
    for (std::set<TCPConnection*>::iterator it=sessions_set_.begin(); 
         it != sessions_set_.end(); ++it)
    {
        //最终会调用DestroyConnection
        (*it)->Close();
    }
    sessions_set_.clear();   

    processor_pool_.Stop();
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
