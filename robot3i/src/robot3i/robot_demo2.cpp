#include <signal.h>
#include <stdio.h>
#include <vector>
#include "Lidar3I.hpp"
#include "Motion3I.hpp"
int ok =  1;

void int_handler(int sign)
{
    ok = 0;
}

typedef struct {
    float       angle_min;
    float       angle_max;
    float       angle_increment;
    float       range_min;
    float       range_max;
    std::vector<float> intensities;
    std::vector<float>  ranges;
} LaserScan;

LaserScan scan;
void update(Lidar3I  *lidar,Motion3I  *motion)
{
    Lidar3I::LidarPoint lidar_point;
    static uint16_t angle = 0;
    static uint32_t seq = 0;
    std::queue<Lidar3I::LidarPoint> &laser_scan = lidar->getLaserScan();

    while(!laser_scan.empty())
    {
        lidar_point = laser_scan.front();
        laser_scan.pop();
        if(lidar_point.angle < angle)
        {
            int num = scan.ranges.size();
            scan.angle_min       = 0;
            scan.angle_max       = 6.28;
            scan.angle_increment = (6.28 / (num - 1));
            scan.range_min       = 0.01;
            scan.range_max       = 6.0;
//debug
            timespec time;
            clock_gettime(CLOCK_MONOTONIC,&time);
            printf("header: \r\n");
            printf("seq: %6d \r\n",seq++);
            printf("stamp: \r\n");
            printf("    secs: %6ld \r\n",time.tv_sec);
            printf("    nsecs: %6ld \r\n",time.tv_nsec);
            printf("  frame_id: \"\"\r\n");
            printf("angle_min: %f \r\n",scan.angle_min);
            printf("angle_max: %f \r\n",scan.angle_max);
            printf("angle_increment: %f \r\n",scan.angle_increment);
            printf("range_min: %f \r\n",scan.range_min);
            printf("range_max: %f \r\n",scan.range_max);
            printf("ranges: [");
            for(int i = 0; i < num - 1; i++)
            {
                printf("%1.2f,",scan.ranges[i]);
            }
            printf("%1.2f ]\r\n",scan.ranges[num - 1]);
            printf("intensities: [");
            for(int i = 0; i < num - 1; i++)
            {
                printf("%1.2f,",scan.intensities[i]);
            }
            printf("%1.2f ]\r\n",scan.intensities[num - 1]);
//debug
            scan.intensities.clear();
            scan.ranges.clear();
        }
        scan.intensities.push_back(lidar_point.signal );
        scan.ranges.push_back(lidar_point.distance / 10000.0);
        angle = lidar_point.angle;
    }
}

int main(int argc, char* argv[])
{
    sigset( SIGINT, int_handler);
    Lidar3I       *lidar  = new Lidar3I();
    Motion3I      *motion = new Motion3I();

    lidar->Init();
    motion->Init();

    lidar->SetMode(1);
    motion->SetMode(1);

    while (ok)
    {
        lidar->Run();
        motion->Run();
        update(lidar,motion);
        usleep(100 * 1000);
    }
    return 0;
}

