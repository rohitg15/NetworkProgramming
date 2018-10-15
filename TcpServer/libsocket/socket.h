#ifndef _SOCKET_H_
#define _SOCKET_H_

#include <string>
#include <vector>
#include <arpa/inet.h>


namespace sockets
{
    class BaseSocket
    {
    public:
        virtual ~BaseSocket();

        void CloseSocket();

        BaseSocket(
            BaseSocket&& move
        ) noexcept;

        BaseSocket& operator=(
            BaseSocket&& move
        ) noexcept;

        BaseSocket(
            const BaseSocket&
        ) = delete;

        BaseSocket& operator=(
            const BaseSocket&
        ) = delete;
        
    protected:
        BaseSocket(
            int socketId
        );

        int GetSocketId() const;

    private:
        int m_socketId;
        static int constexpr invalidSocketId = -1;
    };

    
    class ServerSocket : public BaseSocket
    {
    public:
        ServerSocket(
            int port,
            int qLen,
            uint32_t options = 0
        );

        int Accept(
            struct sockaddr *addr,
            socklen_t *addrlen
        );

        void BlockingRead(
            int clientFd,
            std::vector<uint8_t>& msg,
            int& numBytes
        );

        void BlockingWrite(
            int clientFd,
            const std::vector<uint8_t>& msg
        );

    private:
        sockaddr_in m_serverAddr;
        static int constexpr MAX_READ_SIZE = 1024;

    };

}   //  sockets

#endif  //  _SOCKET_H_