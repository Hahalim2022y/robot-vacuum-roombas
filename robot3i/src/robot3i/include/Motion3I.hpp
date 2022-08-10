#ifndef __MOTION3I_HPP_
#define __MOTION3I_HPP_


#include <string>
#include "TCPSocketClient.hpp"
#include "CommPacketV3.hpp"


class Motion3I
{
public:
    typedef struct
    {
        float x_vel   = 0.0;
        float y_vel   = 0.0;
        float yaw_vel = 0.0;
    }Velocity_t;

    typedef struct
    {
        float x   = 0.0;
        float y   = 0.0;
        float yaw = 0.0;
    }Position_t;

private:
    #define CMD_CHASSIS 1
    #define CMD_ROBOT_CONTROL_TYPE 2
    #define CMD_MOTOR_CONTROL_VELOCITY 3



    #pragma pack(1)
    typedef struct {
        uint8_t bumper_left;
        uint8_t bumper_right;
        float   x;
        float   y;
        float   yaw;
        float   x_vel;
        float   yaw_vel;
        uint8_t	battery_voltage;
        uint8_t charge_state;
    } TChassisSensor;

    typedef struct {
        uint8_t mode;
    } TRbotoControlType;

    typedef struct {
        float   x_vel;
        float   yaw_vel;
    } TMotorVelocity;

    #pragma pack()
    /* data */
    TCPSocketClient *client;
    CommPacketV3    *commPacketV3;
    TChassisSensor  chassis_sensor_info;

public:
    Motion3I(/* args */)
    :client(new TCPSocketClient("192.168.5.1",5000)),
    commPacketV3(new CommPacketV3(client))
    {

    };

    ~Motion3I()
    {
        Velocity_t vel;
        vel.x_vel = 0;
        vel.y_vel = 0;
        vel.yaw_vel = 0;
        SetVelocit(vel);
        SetMode(0);
        sleep(1);
        delete(client);
    };

    ssize_t Init()
    {
        client->Init();
        client->connect();
        commPacketV3->Init();
        commPacketV3->ReceiveRegister(CMD_CHASSIS,&Motion3I::sensorCallback,this);
        return 1;
    };

    ssize_t Run()
    {
        return 1;
    };

    ssize_t SetMode(uint8_t mode)
    {
        commPacketV3->SendMessage(CMD_ROBOT_CONTROL_TYPE,mode);
        return 1;
    };

    ssize_t SetVelocit(Velocity_t vel)
    {
        TMotorVelocity chassis_vel;
        chassis_vel.x_vel = vel.x_vel;
        chassis_vel.yaw_vel = vel.yaw_vel;
        commPacketV3->SendMessage(CMD_MOTOR_CONTROL_VELOCITY,chassis_vel);
        return 1;
    };

    ssize_t GetVelocit(Velocity_t &vel)
    {
        vel.x_vel = chassis_sensor_info.x_vel;
        vel.yaw_vel = chassis_sensor_info.yaw_vel;
        return 1;
    };

    ssize_t GetOdometer(Position_t &odom)
    {
        odom.x = chassis_sensor_info.x;
        odom.y = chassis_sensor_info.y;
        odom.yaw = chassis_sensor_info.yaw;
        return 1;
    };

    void sensorCallback(uint16_t cmd, void *data, uint16_t len)
    {
        chassis_sensor_info = *(TChassisSensor*)data;
    };

    void GetBumper(uint8_t &left,uint8_t &right)
    {
        left = chassis_sensor_info.bumper_left;
        right = chassis_sensor_info.bumper_right;
    }

    void GetBattery(uint8_t &voltage,uint8_t &state)
    {
        voltage = chassis_sensor_info.battery_voltage;
        state = chassis_sensor_info.charge_state;
    }
};






#endif