#ifndef __TCP_SOCKET_CLIENT_HPP
#define __TCP_SOCKET_CLIENT_HPP

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <atomic>
#include <stdbool.h>
#include <string>

class TCPSocketClient
{
private:
    /* data */

public:
    TCPSocketClient(const std::string ServerIp, int Port);
    ~TCPSocketClient();
    ssize_t Init();
    int  Send(const uint8_t* buffer, uint32_t bufferLen);
    int  Recv(uint8_t* buffer, uint32_t bufferSizeMax);
    ssize_t  connect();

protected:


protected:
    struct sockaddr_in m_address;
    std::string m_ServerIp;
    int   m_Port;
    int   m_socketfd;
    bool  m_disconnect_flag = false;
};



#endif