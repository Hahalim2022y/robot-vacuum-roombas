#include "ros/ros.h"
#include "stdio.h"
#include <geometry_msgs/TwistStamped.h>
#include <nav_msgs/Odometry.h>
#include <std_msgs/UInt8.h>
#include <sensor_msgs/PointCloud.h>
#include <sensor_msgs/LaserScan.h>
#include <sensor_msgs/Joy.h>
#include <sensor_msgs/BatteryState.h>
#include "tf/transform_datatypes.h"
#include "Lidar3I.hpp"
#include "Motion3I.hpp"
#include <tf/tf.h>
#include <tf/transform_broadcaster.h>
#include <iostream>
#include <algorithm>

class Robot
{
private:
  /* data */

  ros::NodeHandle m_nh;
  ros::Subscriber m_sub_base_cmd;
  ros::Subscriber m_sub_joy;
  ros::Publisher m_pub_base_cmd;
  ros::Publisher m_pub_wheel_odom;
  ros::Publisher m_pub_bumper;
  ros::Publisher m_laser_pub;
  ros::Publisher m_cloud_pub;
  ros::Publisher m_battery_pub;
  ros::Publisher tff;

  sensor_msgs::LaserScan scan;

  void subCallbackBaseCmd(const geometry_msgs::Twist::ConstPtr &in)
  {
    Motion3I::Velocity_t vel;
    vel.x_vel = in->linear.x;
    vel.y_vel = 0;
    vel.yaw_vel = in->angular.z;
    motion->SetVelocit(vel);
  }

  void subCallbackJoy(const sensor_msgs::Joy::ConstPtr &in)
  {
    geometry_msgs::Twist base_cmd;
    static bool safety = 0;
    static double speedalpha = 0.5;

    if (safety != in->buttons[4])
    {
      safety = !safety;
    }

    safety = 1;
    if (in->buttons[4])
    {
      speedalpha *= 0.9;
    }
    if (in->buttons[5])
    {
      speedalpha *= 1.1;
    }

    if (safety)
    {
      base_cmd.linear.x = in->axes[1] * speedalpha;
      base_cmd.linear.y = in->axes[0] * speedalpha;
      base_cmd.linear.z = 0;
      base_cmd.angular.x = 0;
      base_cmd.angular.y = 0;
      base_cmd.angular.z = in->axes[2] * 0.5;
      m_pub_base_cmd.publish(base_cmd);
    }
    else
    {
      base_cmd.linear.x = 0;
      base_cmd.linear.y = 0;
      base_cmd.linear.z = 0;
      base_cmd.angular.x = 0;
      base_cmd.angular.y = 0;
      base_cmd.angular.z = 0;
      m_pub_base_cmd.publish(base_cmd);
    }
  }

  geometry_msgs::TransformStamped tx;

  void pubWheelOdom()
  {
    nav_msgs::Odometry odom;
    Motion3I::Velocity_t vel;
    Motion3I::Position_t get_pos;

    motion->GetVelocit(vel);
    motion->GetOdometer(get_pos);

    odom.header.frame_id = "odom";
    odom.child_frame_id = "base_link";
    odom.header.stamp = ros::Time::now();

    odom.twist.twist.linear.x = vel.x_vel;
    odom.twist.twist.linear.y = vel.y_vel;
    odom.twist.twist.angular.z = vel.yaw_vel;
    odom.pose.pose.position.x = get_pos.x;
    odom.pose.pose.position.y = get_pos.y;
    odom.pose.pose.position.z = 0.0;
    odom.pose.pose.orientation = tf::createQuaternionMsgFromYaw(get_pos.yaw);
    odom.twist.covariance[0] = 0.0025 * abs(odom.twist.twist.linear.x);
    odom.twist.covariance[7] = 0.0025 * abs(odom.twist.twist.linear.y);
    odom.twist.covariance[35] = 0.0025 * abs(odom.twist.twist.angular.z);
    m_pub_wheel_odom.publish(odom);
    
    geometry_msgs::TransformStamped tx;
    tx.header.frame_id = "odom";
    tx.child_frame_id = "base_link";

    //odom变换
    static tf::TransformBroadcaster odo_broadcaster;
    //平移变换
    tx.transform.translation.x = get_pos.x;
    tx.transform.translation.y = get_pos.y;
    tx.transform.translation.z = 0;
    //旋转变换
    tx.transform.rotation = tf::createQuaternionMsgFromYaw(get_pos.yaw);
    //时间戳
    tx.header.stamp = odom.header.stamp;
    odo_broadcaster.sendTransform(tx);

    //雷达变换
    static tf::TransformBroadcaster laser_broadcaster;
    tf::Transform laser_transform;
    laser_transform.setOrigin(tf::Vector3(0, 0, 0)); //
    tf::Quaternion q;
    q.setRPY(0, 0, 1.7889624 - 0.2 * odom.twist.twist.angular.z); 
    laser_transform.setRotation(q);
    laser_broadcaster.sendTransform(tf::StampedTransform(laser_transform, odom.header.stamp, "base_link", "laser_frame"));
    //以base_link为父节点，laser为子节点
  }

  void pubBumper()
  {
    std_msgs::UInt8 bumper;
    uint8_t left, right;
    motion->GetBumper(left, right);
    bumper.data = left << 1 & right;
    m_pub_bumper.publish(bumper);
  }

  void PublishCloud()
  {
    sensor_msgs::PointCloud cloud;
  }

  void PublishLaserScanFan()
  {
    Lidar3I::LidarPoint lidar_point;
    static uint16_t angle = 0;
    std::queue<Lidar3I::LidarPoint> &laser_scan = lidar->getLaserScan();

    while (!laser_scan.empty())
    {
      lidar_point = laser_scan.front();
      laser_scan.pop();
      if (lidar_point.angle >= angle)
      {
        scan.intensities.push_back(lidar_point.signal);
        scan.ranges.push_back(lidar_point.distance / 10000.0 * 2.61);
      }
      else
      {
        int num = scan.ranges.size();
        scan.header.frame_id = "laser_frame";
        scan.header.stamp = ros::Time::now();
        reverse(scan.intensities.begin(), scan.intensities.end());
        reverse(scan.ranges.begin(), scan.ranges.end());
        scan.angle_min = 0;
        scan.angle_max = 6.28;
        scan.angle_increment = (6.28 / (num - 1));
        scan.time_increment = 1.0 / (6.0 * num);      //(1 / hz) //////////////
        scan.scan_time = (scan.time_increment * 360); //////////////////
        scan.range_min = 0.07;
        scan.range_max = 8;
        m_laser_pub.publish(scan);
        scan.intensities.clear();
        scan.ranges.clear();
        scan.intensities.push_back(lidar_point.signal);
        scan.ranges.push_back(lidar_point.distance / 10000.0 * 2.61);
        printf("%f %f \r\n", lidar->GetSpeed().first, lidar_point.distance / 10000.0 * 2.61);
      }
      angle = lidar_point.angle;
    }
  }

  void pubBatteryState()
  {
    static int time = 0;
    if (time++ > 100)
    {
      time = 0;
      sensor_msgs::BatteryState battery;
      uint8_t voltage, state;
      motion->GetBattery(voltage, state);
      battery.voltage = voltage / 10.0;
      battery.charge = state;
      m_battery_pub.publish(battery);
    }
  }

  Lidar3I *lidar;
  Motion3I *motion;

public:
  Robot(/* args */) : lidar(new Lidar3I()), motion(new Motion3I())
  {
  }

  ~Robot()
  {
    delete (lidar);
    delete (motion);
  }

  void Init()
  {
    lidar->Init();
    motion->Init();

    m_sub_base_cmd = m_nh.subscribe("cmd_vel", 1, &Robot::subCallbackBaseCmd, this);
    //    std:: cout<<"asd\n";
    m_sub_joy = m_nh.subscribe("/joy", 1, &Robot::subCallbackJoy, this);
    //  std::cout<<"asd\n";
    m_pub_base_cmd = m_nh.advertise<geometry_msgs::Twist>("/cmd_vel", 50);
    m_pub_wheel_odom = m_nh.advertise<nav_msgs::Odometry>("/odom", 2);
    m_pub_bumper = m_nh.advertise<std_msgs::UInt8>("/bumper", 1);
    m_cloud_pub = m_nh.advertise<sensor_msgs::PointCloud>("/cloud", 50);
    m_laser_pub = m_nh.advertise<sensor_msgs::LaserScan>("/scan", 50);
    m_battery_pub = m_nh.advertise<sensor_msgs::BatteryState>("/battery", 1);

    lidar->SetMode(1);
    motion->SetMode(1);
  }

  void Run()
  {
    lidar->Run();
    motion->Run();

    pubWheelOdom();
    pubBumper();
    PublishLaserScanFan();
    pubBatteryState();

    ros::spinOnce();
  }
};

#include <signal.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
  ros::init(argc, argv, "robot3i_node");
  ros::start();

  ros::Rate m_loopRate(100);
  Robot *rebot = new Robot();

  ros::NodeHandle n;
  rebot->Init();
  while (ros::ok())
  {
    rebot->Run();
    m_loopRate.sleep();
  }
  delete (rebot);
  sleep(1);
  return 0;
}
