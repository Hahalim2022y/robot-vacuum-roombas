# robot-vacuum-roombas
校机器人大赛扫地机器人slam代码 

robot3i使用教程
全程命令
1.roslaunch robot3i robot_ros.launch          (开启雷达数据,手柄)

2.roslaunch robot3i slam_gmapping.launch     (gmapping算法建图)
3.rosrun rviz rviz               			   (建图)
4.rosrun map_server map_saver -f ~/map/111  (保存地图,111是图片名字,gmapping和cartographer(改过了)此命令相同，cartographer还有其他保存地图的命令，但ubuntu16.04不支持)
5.roslaunch robot3i move_base.launch         (导航)
6.cd opencv-3.3.1/test/build                 (opencv识别图片并输出中心坐标)
  cmake ..
  make
  ./testing
（g++ task1.cpp -o task1 `pkg-config --cflags --libs opencv`）


2.roslaunch cartographer_ros demo_revo_lds.launch （谷歌算法建图，加载机器人模型）
3.roslaunch nav_demo map_saver.launch     (=rosrun map_server map_saver -f ~/nav_demo/map/nav 注意保存地图的路径和地图的名字)
4.cd opencv-3.3.1/test 1/build                 (opencv识别图片并输出中心坐标)
  ./testing
5.roslaunch nav_demo amcl_test.launch        (在保存的地图中定位)
7.roslaunch robot3i move_base.launch         (导航)
