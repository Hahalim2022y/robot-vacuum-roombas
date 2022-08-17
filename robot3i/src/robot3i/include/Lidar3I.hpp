#ifndef __LIDAR3I_HPP_
#define __LIDAR3I_HPP_

#include <string>
#include "TCPSocketClient.hpp"
#include "LidarCommPacket.hpp"

class Lidar3I
{
public:
    class LidarPoint
    {
        public:
        uint16_t angle; //0.01
        uint8_t  signal;
        uint16_t distance;
    };
private:
    /* data */
#pragma pack(1)
    class MSG_t 
    {
        public:

        uint8_t     head;
        uint16_t    len;
        uint8_t     address_code;
        uint8_t     type;
        uint8_t     cmd;
        uint16_t    msg_len;
        uint8_t     data[0];
    };

    class MSG_0xAE_t : public MSG_t
    {
        public:

        uint8_t     speed;
    };

    class MSG_0xAD_t : public MSG_t
    {
        public:

        uint8_t     speed;
        uint16_t    offset_angle;
        uint16_t    start_angle;
        uint16_t    end_angle;
        struct {
            uint8_t  signal;
            uint16_t distance;
        }data[0];
    };
#pragma pack()

    
    #define BigLitfleSwap16(A) ((((uint16_t)(A) >> 8) & (0xff << 0 )) | \
                                (((uint16_t)(A) << 8) & (0xff << 8 )))

    #define BigLitfleSwap32(A) ((((uint32_t)(A) >> 24) & (0xff << 0 )) | \
                                (((uint32_t)(A) >> 8) & (0xff << 8 )) | \
                                (((uint32_t)(A) << 24) & (0xff << 24 )) | \
                                (((uint32_t)(A) << 8) & (0xff << 16 )))

    TCPSocketClient             *client;
    LidarCommPacket             *lidar_comm_packet;
    std::pair<float, timespec>  speed;
    uint8_t         lidar_mode = 0;
    std::queue<LidarPoint> queue_lidar_point;

    void sensorCallback(uint16_t cmd, void *data, uint16_t len)
    {
        MSG_t *packet = NULL;
        int16_t length = 0;
        length = lidar_comm_packet->GetPacket((uint8_t*&)packet);
        if(packet == NULL)
        {
            return ;
        }
        static int num = 0;
        switch (packet->cmd)
        {
            case 0xad:
            {
                MSG_0xAD_t *msg = (MSG_0xAD_t*)packet; 
                msg->msg_len     = BigLitfleSwap16(msg->msg_len);
                msg->start_angle = BigLitfleSwap16(msg->start_angle);
                msg->end_angle   = BigLitfleSwap16(msg->end_angle);
                msg->offset_angle= BigLitfleSwap16(msg->offset_angle);
                speed.first = (float)(msg->speed) * 0.05f;
                clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&speed.second);

                LidarPoint lidar_point;
                uint8_t num = ((msg->msg_len) - (sizeof(MSG_0xAD_t) - sizeof(MSG_t))) / 3;
                uint8_t num_error = 0;
                for(int i = 0; i < num; i++)
                {
                    num_error += (msg->data[i].signal == 0xff ? 1 : 0);
                }

                uint16_t angle_diff = (msg->end_angle - msg->start_angle)/(num - num_error - 1);
                for(int i = 0,n = 0; i < num; i++)
                {
                    if(msg->data[i].signal == 0xff) continue;
                    lidar_point.angle    = msg->start_angle + angle_diff * n++;
                    lidar_point.signal   = msg->data[i].signal;
                    lidar_point.distance = BigLitfleSwap16(msg->data[i].distance);

                    if(queue_lidar_point.size() <= 3000)
                    {
                        queue_lidar_point.push(lidar_point);
                    }
                }
            } break;
            case 0xae:
            {
                MSG_0xAE_t *msg = (MSG_0xAE_t*)packet; 
                speed.first = (float)(msg->speed) * 0.05f;
                clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&speed.second);
            } break;
            default:
                break;
        }
    }
public:
    Lidar3I(/* args */)
    :client(new TCPSocketClient("192.168.5.1",5001)),
    lidar_comm_packet(new LidarCommPacket(client))
    {

    };

    ~Lidar3I()
    {
        delete lidar_comm_packet;
        delete client;
    };

    ssize_t Init()
    {
        client->Init();
        client->connect();
        lidar_comm_packet->Init();
        lidar_comm_packet->ReceiveRegister(0x11,&Lidar3I::sensorCallback,this);
        return 1;
    };

    ssize_t Run()
    {
        return 1;
    };  

    ssize_t SetMode(uint8_t mode)
    {
        return 1;
    };

    std::queue<LidarPoint> &getLaserScan() 
    {
        return queue_lidar_point;
    };

    std::pair<float, timespec>  GetSpeed()
    {
        return speed;
    };
};






#endif