#include "server.h"
#include <sys/socket.h>
#include <iostream>


ClientContext::ClientContext()
{
    m_state = ClientStates::FREE;
}

sockaddr_in* ClientContext::GetPtrSockAddr()
{
    return &m_clientSockAddr;
}


socklen_t* ClientContext::GetPtrSockLen()
{
    return &m_clientSockAddrLen;
}

ClientStates ClientContext::GetState() const
{
    return m_state;
}

TcpServer::TcpServer(
    int port,
    int qLen
)
{
    // remember: SOCK_NONBLOCK
    m_pServerSock = std::make_unique<sockets::ServerSocket>(port, qLen);
    // m_epollFd = epoll_create1(0);
}

void TcpServer::ListenForever()
{
    for (;;)
    {
        // for now, we assume we have a small number of clients
        ClientContext *pCtx = NULL;
        for (auto const& ctx: m_clients)
        {
            if (ctx.GetState() == ClientStates::FREE)
            {
                pCtx = const_cast<ClientContext*>(&ctx);
                break;
            }
        }
        if (!pCtx)
        {
            //sleep(5);
            continue;
        }
        sockaddr* pClientSockAddr = (sockaddr*)(pCtx->GetPtrSockAddr());
        socklen_t* pClientSockLen = static_cast<socklen_t*>(pCtx->GetPtrSockLen());
        int clientFd = m_pServerSock->Accept(pClientSockAddr, pClientSockLen);
        if (clientFd < 0)
        {
            std::cout << "Accept failed : " << clientFd << std::endl;
            continue;
        }

        // add this clientFd to list of sockets 
    }
}