
#include <signal.h>
#include <stdio.h>
#include "TCPSocketClient.hpp"

int ok =  1;

void int_handler(int sign)
{
    ok = 0;
}

int main(int argc, char* argv[])
{
    uint8_t data[500];
    int len;
    int time = 0;
    TCPSocketClient *client_motion(new TCPSocketClient("192.168.5.1",5000));
    TCPSocketClient *client_lidar(new TCPSocketClient("192.168.5.1",5001));

    client_motion->Init();
    client_lidar->Init();

    client_motion->connect();
    client_lidar->connect();

    while (ok)
    {
        len = client_motion->Recv(data,500);
        if(len > 0)
        {
            printf("%5d motion len: %3d data:",time,len);
            for(int i = 0; i < len ; i++)
            {
                printf("%02x ",data[i]);
            }
            printf(" \r\n");
        }
        len = client_lidar->Recv(data,500);
        if(len > 0)
        {
            printf("%5d lidar len: %3d data:",time,len);
            for(int i = 0; i < len ; i++)
            {
                printf("%02x ",data[i]);
            }
            printf(" \r\n");
        }
        time++;
        usleep(10 * 1000);
    }
    delete client_motion;
    delete client_lidar;
    return 0;
}

