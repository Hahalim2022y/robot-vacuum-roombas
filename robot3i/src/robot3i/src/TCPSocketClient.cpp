#include <fcntl.h>
#include "TCPSocketClient.hpp"


TCPSocketClient::TCPSocketClient(const std::string ServerIp, int Port)
{
    m_ServerIp = ServerIp;
    m_Port     = Port;
    m_disconnect_flag = false;
    m_socketfd = -1;
}

TCPSocketClient::~TCPSocketClient()
{
    if(m_socketfd > 0)
    {
        close(m_socketfd);
        m_socketfd = -1;
    }
}

ssize_t TCPSocketClient::Init()
{
    if(m_disconnect_flag == true)
    {
        return 0;
    }
    if(m_socketfd > 0)
    {
        close(m_socketfd);
        m_socketfd = -1;
        sleep(1);
    }
    if( (m_socketfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("create socket error: %s(errno: %d)\n", strerror(errno),errno);
        return 0;
    }
    // int flags = fcntl( m_socketfd, F_GETFL, 0 );
    // fcntl( m_socketfd, F_SETFL, flags | O_NONBLOCK );
    memset(&m_address, 0, sizeof(m_address));
    m_address.sin_family = AF_INET;
    m_address.sin_port = htons(m_Port);
    if( inet_pton(AF_INET, m_ServerIp.data(), &m_address.sin_addr) <= 0)
    {
        printf("inet_pton error for %s\n",m_ServerIp.data());
        return 0;
    }
    return 1;
}

int TCPSocketClient::Send(const uint8_t* buffer, uint32_t bufferLen)
{
    if(m_disconnect_flag == false)
    {
        Init();
        connect();
        return -1;
    }

    int sendResult = 0;
    sendResult     = static_cast<int>(send(m_socketfd, buffer, bufferLen, MSG_DONTWAIT));
    return sendResult;
}

int TCPSocketClient::Recv(uint8_t* buffer, uint32_t bufferSizeMax)
{

    if(m_disconnect_flag == false)
    {
        Init();
        connect();
        return -1;
    }

    int recvResult  = 0;
    int recvDataLen = 0;
    recvResult = static_cast<int>(recv(m_socketfd, buffer, bufferSizeMax, MSG_DONTWAIT));

    if (recvResult > 0) 
    {
        recvDataLen = recvResult;
        return recvDataLen;
    }
    else if(recvResult < 0 && ( errno == EAGAIN )) //errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN)) 
    {
        return 0;
    }
    else 
    {
        m_disconnect_flag = false;
        return -1;
    }
}

ssize_t TCPSocketClient::connect()
{
    if(m_disconnect_flag == true)
    {
        return 0;
    }

    if( ::connect(m_socketfd, (struct sockaddr*)&m_address, sizeof(m_address)) < 0)
    {
        printf("connect %s:%d error: %s(errno: %d)\n",m_ServerIp.data(),m_Port,strerror(errno),errno);
        m_disconnect_flag = false;
        return 0;
    }
    else 
    {
        m_disconnect_flag = true;
        return 1;
    }
}
