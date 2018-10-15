#ifndef _SERVER_H_
#define _SERVER_H_

#include "socket.h"
#include <memory>
#include <vector>
#include <sys/socket.h>

// https://github.com/angrave/SystemProgramming/wiki/Networking,-Part-7:-Nonblocking-I-O,-select(),-and-epoll



enum class ClientStates
{
    FREE,
    CONNECTED,
    READING
};

class ClientContext
{
public:
    ClientContext();

    sockaddr_in* GetPtrSockAddr();

    socklen_t* GetPtrSockLen();

    ClientStates GetState() const;

private:
    sockaddr_in m_clientSockAddr;
    socklen_t m_clientSockAddrLen;
    ClientStates m_state;
};




class TcpServer
{
public:
    TcpServer(
        int port,
        int qLen = 10
    );

    TcpServer(
        const TcpServer& that
    ) = delete;

    TcpServer& operator=(
        const TcpServer& that
    ) = delete;

    void ListenForever();

private:
    std::unique_ptr<sockets::ServerSocket> m_pServerSock;
    std::vector<ClientContext> m_clients;
    int m_epollFd;
};



#endif  //  _SERVER_H_