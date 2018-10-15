#include "socket.h"
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string>
#include <stdexcept>
#include <string.h>
#include <vector>
#include <iostream>


namespace sockets
{
    /*
     * Definitions for BaseSocket methods
     */
    BaseSocket::BaseSocket(
        int socketId
    ) : m_socketId(socketId)
    {
        if (m_socketId == BaseSocket::invalidSocketId)
        {
            std::string err = "BaseSocket error: ";
            err += strerror(errno);
            throw std::runtime_error(err);
        }
    }

    BaseSocket::~BaseSocket()
    {
        if (m_socketId == BaseSocket::invalidSocketId)
        {
            return;
        }
        CloseSocket();
    }

    void BaseSocket::CloseSocket()
    {
        if (m_socketId == BaseSocket::invalidSocketId)
        {
            std::string err = "BaseSocket error in CloseSocket, socket already removed";
            throw std::runtime_error(err);
        }
        close(m_socketId);
        m_socketId = BaseSocket::invalidSocketId;
    }

    int BaseSocket::GetSocketId() const
    {
        return m_socketId;
    }

    // ServerSocket code

    ServerSocket::ServerSocket(
        int port,
        int qLen,
        uint32_t options
    ) : BaseSocket(::socket(PF_INET, SOCK_STREAM | options, 0))
    {
        // int opt = 1;
        // if (::setsockopt(GetSocketId(), SOL_SOCKET, 
        //                         SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
        // {
        //     CloseSocket();
        //     std::string err = "setsockopt error: ";
        //     err += strerror(errno);
        //     throw std::runtime_error(err);
        // }

        m_serverAddr.sin_addr.s_addr = INADDR_ANY;
        m_serverAddr.sin_family = AF_INET;
        m_serverAddr.sin_port = htons(port);

        if (::bind(GetSocketId(), (struct sockaddr *)&m_serverAddr, sizeof(sockaddr)) < 0)
        {
            CloseSocket();
            std::string err = "bind error: ";
            err += strerror(errno);
            throw std::runtime_error(err);
        }

        if (::listen(GetSocketId(), qLen) < 0)
        {
            CloseSocket();
            std::string err = "listen error: ";
            err += strerror(errno);
            throw std::runtime_error(err);
        }
    }

    int ServerSocket::Accept(
        struct sockaddr* addr,
        socklen_t *addrLen
    )
    {
        int clientFd = ::accept(GetSocketId(), addr, addrLen);
        if (clientFd <= 0)
        {
            std::string err = "accept error: ";
            err += strerror(errno);
            throw std::runtime_error(err);
        }
        return clientFd;
    }

    void ServerSocket::BlockingRead(
        int clientFd,
        std::vector<uint8_t>& msg,
        int& numBytes
    )
    {
        size_t bytesRead = 0;
        for (;;)
        {
            ssize_t ret = ::read(clientFd, msg.data() + bytesRead, ServerSocket::MAX_READ_SIZE);
            if (ret == -1)
            {
                std::string err = "BlockingRead error in ServerSocket - ";
                err += strerror(errno);
                throw std::runtime_error(err);
            }
            else if (ret == 0)
            {
                break;
            }
            bytesRead += static_cast<size_t>(ret);
        }
    }

    void ServerSocket::BlockingWrite(
        int clientFd,
        const std::vector<uint8_t>& msg
    )
    {
        size_t bytesWritten = 0;

        while (bytesWritten < msg.size())
        {
            ssize_t ret = ::write(clientFd, msg.data() + bytesWritten, msg.size() - bytesWritten);
            if (ret == -1)
            {
                std::string err = "BlockingWrite error in ServerSocket - ";
                err += strerror(errno);
                throw std::runtime_error(err);
            }
            bytesWritten += static_cast<size_t>(ret);
        }
    }

}   // sockets