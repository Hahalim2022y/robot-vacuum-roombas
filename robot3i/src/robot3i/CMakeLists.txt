project(robot3i)
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

add_compile_options(-std=c++11)

##ROS
find_package(catkin REQUIRED COMPONENTS
  tf
  roscpp
  rospy
  std_msgs
  sensor_msgs
)
catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES bluesea
#  CATKIN_DEPENDS rosconsole roscpp sensor_msgs std_srvs
#  DEPENDS system_lib
)

AUX_SOURCE_DIRECTORY(${PROJECT_SOURCE_DIR}/src DIR_SRCS)

# ADD_SUBDIRECTORY( src )
include_directories(${PROJECT_SOURCE_DIR}/include )

include_directories(${catkin_INCLUDE_DIRS})

ADD_EXECUTABLE(robot3i_node ${PROJECT_SOURCE_DIR}/robot_ros.cpp ${DIR_SRCS})
TARGET_LINK_LIBRARIES( robot3i_node ${catkin_LIBRARIES}  -lpthread )

ADD_EXECUTABLE(robot_demo1 ${PROJECT_SOURCE_DIR}/robot_demo1.cpp ${DIR_SRCS})
TARGET_LINK_LIBRARIES( robot_demo1  -lpthread )

ADD_EXECUTABLE(robot_demo2 ${PROJECT_SOURCE_DIR}/robot_demo2.cpp ${DIR_SRCS})
TARGET_LINK_LIBRARIES( robot_demo2  -lpthread )



