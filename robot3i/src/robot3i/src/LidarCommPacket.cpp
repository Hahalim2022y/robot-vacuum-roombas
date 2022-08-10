#include "LidarCommPacket.hpp"
#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <string.h>

LidarCommPacket::LidarCommPacket(TCPSocketClient *comm)
{
    serial_device = comm;
    pthread_mutex_init(&thread_tx_mutex,NULL);
    pthread_mutex_init(&thread_rx_mutex,NULL);
}

LidarCommPacket::~LidarCommPacket()
{
    linux_run_flag = false;
    pthread_join(thread_tx_,NULL);
    pthread_join(thread_rx_,NULL);
    std::cout <<  "LidarCommPacket::~LidarCommPacket()" << std::endl;
}

ssize_t LidarCommPacket::Init()
{
    int ret_thrd = PthreadCreate(&thread_tx_, NULL,&LidarCommPacket::Serial_Comm_Send_Thread,this);
    if (ret_thrd != 0) 
    {
        // ROS_WARN_STREAM(__FUNCTION__ << ": " << "Thread creation failed");
        return -1;
    }
    ret_thrd = PthreadCreate(&thread_rx_, NULL,&LidarCommPacket::Serial_Comm_Recv_Thread,this);
    if (ret_thrd != 0) 
    {
        // ROS_WARN_STREAM(__FUNCTION__ << ": " << "Thread creation failed");
        return -1;
    }
    return 1;
}


uint32_t LidarCommPacket::CRC32_Calculate(uint8_t * data, uint16_t len)
{
    // new code, need to wait for PC code revised, then we can enable the below and remove the above
    long packet_sum = 0;
    while ( len --)
    {
        packet_sum += *data++;
    }
    return (uint16_t)(((packet_sum >> 8 ) & 0x00ff) | (( packet_sum << 8 ) & 0xff00));
}

// ===================================================================================
void *LidarCommPacket::Serial_Comm_Send_Thread(void)
{
    std::pair<uint8_t*, uint16_t>  message_send_pair;
    uint32_t    usb_serial_failure_count = 0;
    int         serial_port_status = 0;

    while (linux_run_flag) 
    {
        while (!message_send_queue.empty()) 
        {
            message_send_pair = message_send_queue.front();
            message_send_queue.pop();
            if (serial_port_status < 0) 
            {
                if ( (usb_serial_failure_count % 10) == 0) 
                {
                    // LOG(INFO) << "TIN-SCTF USB-serial connection failure";
                }
                usb_serial_failure_count ++;
            }
            else 
            {
                usb_serial_failure_count = 0;
            }
            serial_port_status = Serial_Comm_Send_Data(message_send_pair.first,message_send_pair.second);
            if (message_send_pair.first != NULL) 
            {
                free(message_send_pair.first);
                message_send_pair.first  = NULL;
                message_send_pair.second = 0;
            }
        }
        usleep(1000);
    }
    return NULL;
}

// ===================================================================================
void *LidarCommPacket::Serial_Comm_Recv_Thread(void)
{
    uint8_t data[1000];
    int     bytes_count = 0;
    uint8_t *begin = &data[500];
    uint8_t *end   = &data[500];

    char str[1000];
    int  count = 0;

    while (linux_run_flag) 
    {
        while((bytes_count = serial_device->Recv(&data[500],500)) > 0)
        {
            end         = &data[500] + bytes_count;
            bytes_count = (uint32_t)(end - begin);
            begin = Serial_Comm_Recv_Data(begin,bytes_count); // watch dog included
            if(begin != end)
            {
                bytes_count = (uint32_t)(end - begin);
                memcpy( &data[500 - bytes_count], begin,bytes_count);
                begin       = &data[500 - bytes_count];
            }
            else
            {
                begin = &data[500];
            }
        }
        usleep(1000);
    }
    return NULL;
}

int LidarCommPacket::Serial_Comm_Send_Data(uint8_t *data,uint16_t len)
{
    int ret = serial_device->Send(data,len); 
    return ret;
}

//aa 00 09 11 61 ae 00 01 29 01 fd
uint8_t *LidarCommPacket::Serial_Comm_Recv_Data(uint8_t *data,uint16_t len)
{
    MSG_t  *msg = (MSG_t*)data;
#if 1

    while((uint8_t*)msg <= (data + len - sizeof(MSG_t) - 2))
    {
        if(msg->sync1 == 0xAA && msg->sync2 == 0x00)
        {
            if(((uint8_t*)msg + msg->length + 2) <= (data + len))
            {
                uint16_t dtc1 = *(uint16_t*)(((uint8_t*)msg) + msg->length);
                uint16_t dtc2 = CRC32_Calculate((uint8_t*)msg,msg->length);
                if(dtc1 == dtc2)
                {
                    std::map<uint16_t, std::pair<void*, void (*)(void*,uint16_t,void *,uint16_t)> >::iterator begin = receive_register_map.find(msg->cmd);
                    if(begin != receive_register_map.end())
                    {
                        msg_data_ = (uint8_t*)msg;
                        msg_len_  =  msg->length + 2;
                        begin->second.second(begin->second.first,msg->cmd,msg->data,msg->length - 3);
                        msg_data_ = (uint8_t*)NULL;
                        msg_len_  =  0;
                    }
                    else
                    {
                        // ROS_WARN_STREAM("Serial_Comm_Recv_Data: message type " << hex << msg->type <<" not registered!");
                    }
                    msg = (MSG_t*)((uint8_t*)msg + msg->length + 2 - 1);
                }
                else 
                {
                    // ROS_WARN_STREAM("Serial_Comm_Recv_Data:" << "type:" << hex << msg->type << " Len:" << hex << msg->len << " Data[0]:" << hex << msg->data[0] );
                    // ROS_WARN_STREAM("Serial_Comm_Recv_Data: CRC32 check failed." << hex << dtc1 << " != " << hex << dtc2);
                }
            }
            else if(( msg->length + 10) <= 500) 
            {
                return (uint8_t*)msg;                
            }
            else 
            {
                // ROS_WARN_STREAM("Serial_Comm_Recv_Data: Len over 500.");
            }
        }
        else
        {
            // std::cout << "USART_PTL_V1_Packets_Process: Invalid data. Data :" << msg->sync1 << msg->sync2;
        }
        msg = (MSG_t*)((uint8_t*)msg + 1);
    }

    return  (uint8_t*)msg;
#else
    TSerialState msg_state = STATE_SYNC1;
    uint8_t ch = 0;

    switch (msg_state)
    {
        case STATE_SYNC1:
            if(ch == 0xfa)
            {
                msg_state = STATE_SYNC2;
            }
            else
            {
                msg_state = STATE_SYNC1;
            }
            break;
        case STATE_SYNC2:
            if(ch == 0xfb)
            {
                msg_state = STATE_LENGTH;
            }
            else
            {
                msg_state = STATE_SYNC1;
            }
            break;
        case STATE_LENGTH:
            msg_state = STATE_ACQUIRE_DATA;
            break;
        case STATE_ACQUIRE_DATA:

            msg_state = STATE_SYNC1;
            break;
        default:
            break;
    }
#endif
}   

// ===================================================================================
ssize_t LidarCommPacket::SendMessage(uint16_t msg_type,uint8_t * msg_data, size_t data_size)
{
    std::pair<uint8_t*, uint16_t> msg;
    uint16_t crc = 0;
    ssize_t ret = 0;

    pthread_mutex_lock(&thread_tx_mutex);
    msg.first = (uint8_t *)malloc(sizeof(MSG_t)  + data_size + sizeof(crc));
    if(msg.first != NULL)
    {
        ((MSG_t*)msg.first)->sync1   = 0xAA;
        ((MSG_t*)msg.first)->sync2   = 0x00;
        ((MSG_t*)msg.first)->length  = data_size + 1 + sizeof(crc); //( size(cmd) + size(data) + size(crc))
        ((MSG_t*)msg.first)->cmd     = msg_type;
        msg.second      = sizeof(MSG_t) + data_size + 2;
        memcpy(((MSG_t*)msg.first)->data,(uint8_t*)msg_data,data_size);
        crc = CRC32_Calculate(&((MSG_t*)msg.first)->cmd,((MSG_t*)msg.first)->length - 2);
        memcpy(((MSG_t*)msg.first)->data + data_size,(uint8_t*)&crc,2);
        message_send_queue.push(msg);
        ret = 1;
    }
    else 
    {
        ret = -1;
        // LOG(INFO) << "TIN-STSCQ Send_to_Serial_Comm_Queue() malloc failure";
        // ROS_WARN_STREAM("Send_to_Serial_Comm_Queue(): malloc() failed, no push!\n");
    }
    pthread_mutex_unlock(&thread_tx_mutex);
    return ret;
}

bool LidarCommPacket::state()
{
    return 1;
}
