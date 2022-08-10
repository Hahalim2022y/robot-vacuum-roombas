#ifndef _COMM_PACKET_V3_HPP_
#define _COMM_PACKET_V3_HPP_

#include <stdint.h>
#include <queue>
#include <map>
#include <stdint.h>
#include "TCPSocketClient.hpp"

class CommPacketV3
{
private:
#pragma pack(1)
    typedef struct {
        uint8_t     sync1;
        uint8_t     sync2;
        uint8_t     cmd;
        uint16_t     length;
        uint8_t     data[0];
    } MSG_t;
#pragma pack(0)

    typedef enum
    {
        STATE_SYNC1 = 0,
        STATE_SYNC2 ,
        STATE_CMD ,
        STATE_LENGTH,
        STATE_ACQUIRE_DATA
    }TSerialState;

private:
    /* data */
    TCPSocketClient                   *serial_device;

    pthread_t                  thread_tx_;
    pthread_t                  thread_rx_;
    pthread_mutex_t            thread_tx_mutex;
    pthread_mutex_t            thread_rx_mutex;
    bool                       linux_run_flag = true;

    std::queue < std::pair<uint8_t*, uint16_t> >                                    message_send_queue;
    std::map<uint16_t, std::pair<void*, void (*)(void*,uint16_t,void *,uint16_t)> > receive_register_map;
    std::map<uint16_t, std::pair<uint8_t*, uint16_t> >                              message_register_map;

private:

    uint32_t CRC32_Calculate(uint8_t*,uint16_t);
    int      Serial_Comm_Send_Data(uint8_t*,uint16_t);
    uint8_t  *Serial_Comm_Recv_Data(uint8_t*,uint16_t);
    void     *Serial_Comm_Send_Thread(void);
    void     *Serial_Comm_Recv_Thread(void);

    template <typename T>
    int PthreadCreate(pthread_t* thread_id, const pthread_attr_t* attr, void* (T::*callback)(), T* arg)
    {
        void* (*__start_routine)(void*);
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wpmf-conversions"
        __start_routine = (void* (*)(void*))callback;
    #pragma GCC diagnostic pop
        return pthread_create(thread_id, attr, __start_routine, (void*)arg);
    }

public:
    CommPacketV3(TCPSocketClient *comm);
    ~CommPacketV3();

    ssize_t Init();
    bool state();

    ssize_t SendMessage(uint16_t msg_type,uint8_t * msg_data, size_t data_size);
    template<class T>
    void SendMessage(uint16_t msg_type,T &message)
    {
        SendMessage(msg_type,(uint8_t*)&message,sizeof(T));
    }

    template<class T>
    void ReceiveRegister(uint16_t msg_type,void (T::*callback)(uint16_t,void *,uint16_t),T *this_p)
    {
        std::pair<void*, void (*)(void*,uint16_t,void *,uint16_t)>  msg;
        msg.first  = this_p;
        #pragma GCC diagnostic push
        #pragma GCC diagnostic ignored "-Wpmf-conversions"
        msg.second = (void (*)(void*,uint16_t,void*,uint16_t))callback;
        #pragma GCC diagnostic pop
        receive_register_map[msg_type] = msg;
    };
};












#endif

