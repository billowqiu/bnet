#ifndef ECHO_CLIENT_H_
#define ECHO_CLIENT_H_

#include <bnet/tcpconnection.h>

class EchoSession : public bnet::TCPConnection
{
public:
    EchoSession(bnet::AsyncProcessor* processor, bnet::TCPServer* server);

protected:
    virtual void OnConnect();
    void HandleRead(const boost::system::error_code& e,
                    std::size_t bytes_transferred);

private:
    char recv_buf_[1024];
};

#endif // ECHO_CLIENT_H_

