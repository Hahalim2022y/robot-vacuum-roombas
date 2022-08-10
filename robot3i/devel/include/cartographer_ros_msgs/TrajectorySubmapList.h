// Generated by gencpp from file cartographer_ros_msgs/TrajectorySubmapList.msg
// DO NOT EDIT!


#ifndef CARTOGRAPHER_ROS_MSGS_MESSAGE_TRAJECTORYSUBMAPLIST_H
#define CARTOGRAPHER_ROS_MSGS_MESSAGE_TRAJECTORYSUBMAPLIST_H


#include <string>
#include <vector>
#include <map>

#include <ros/types.h>
#include <ros/serialization.h>
#include <ros/builtin_message_traits.h>
#include <ros/message_operations.h>

#include <cartographer_ros_msgs/SubmapEntry.h>

namespace cartographer_ros_msgs
{
template <class ContainerAllocator>
struct TrajectorySubmapList_
{
  typedef TrajectorySubmapList_<ContainerAllocator> Type;

  TrajectorySubmapList_()
    : submap()  {
    }
  TrajectorySubmapList_(const ContainerAllocator& _alloc)
    : submap(_alloc)  {
  (void)_alloc;
    }



   typedef std::vector< ::cartographer_ros_msgs::SubmapEntry_<ContainerAllocator> , typename ContainerAllocator::template rebind< ::cartographer_ros_msgs::SubmapEntry_<ContainerAllocator> >::other >  _submap_type;
  _submap_type submap;





  typedef boost::shared_ptr< ::cartographer_ros_msgs::TrajectorySubmapList_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::cartographer_ros_msgs::TrajectorySubmapList_<ContainerAllocator> const> ConstPtr;

}; // struct TrajectorySubmapList_

typedef ::cartographer_ros_msgs::TrajectorySubmapList_<std::allocator<void> > TrajectorySubmapList;

typedef boost::shared_ptr< ::cartographer_ros_msgs::TrajectorySubmapList > TrajectorySubmapListPtr;
typedef boost::shared_ptr< ::cartographer_ros_msgs::TrajectorySubmapList const> TrajectorySubmapListConstPtr;

// constants requiring out of line definition



template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const ::cartographer_ros_msgs::TrajectorySubmapList_<ContainerAllocator> & v)
{
ros::message_operations::Printer< ::cartographer_ros_msgs::TrajectorySubmapList_<ContainerAllocator> >::stream(s, "", v);
return s;
}

} // namespace cartographer_ros_msgs

namespace ros
{
namespace message_traits
{



// BOOLTRAITS {'IsFixedSize': False, 'IsMessage': True, 'HasHeader': False}
// {'std_msgs': ['/opt/ros/kinetic/share/std_msgs/cmake/../msg'], 'cartographer_ros_msgs': ['/home/ubuntu123/333/src/cartographer_ros/cartographer_ros_msgs/msg'], 'geometry_msgs': ['/opt/ros/kinetic/share/geometry_msgs/cmake/../msg']}

// !!!!!!!!!!! ['__class__', '__delattr__', '__dict__', '__doc__', '__eq__', '__format__', '__getattribute__', '__hash__', '__init__', '__module__', '__ne__', '__new__', '__reduce__', '__reduce_ex__', '__repr__', '__setattr__', '__sizeof__', '__str__', '__subclasshook__', '__weakref__', '_parsed_fields', 'constants', 'fields', 'full_name', 'has_header', 'header_present', 'names', 'package', 'parsed_fields', 'short_name', 'text', 'types']




template <class ContainerAllocator>
struct IsFixedSize< ::cartographer_ros_msgs::TrajectorySubmapList_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::cartographer_ros_msgs::TrajectorySubmapList_<ContainerAllocator> const>
  : FalseType
  { };

template <class ContainerAllocator>
struct IsMessage< ::cartographer_ros_msgs::TrajectorySubmapList_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::cartographer_ros_msgs::TrajectorySubmapList_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::cartographer_ros_msgs::TrajectorySubmapList_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct HasHeader< ::cartographer_ros_msgs::TrajectorySubmapList_<ContainerAllocator> const>
  : FalseType
  { };


template<class ContainerAllocator>
struct MD5Sum< ::cartographer_ros_msgs::TrajectorySubmapList_<ContainerAllocator> >
{
  static const char* value()
  {
    return "408b96b1bf0386ee3f0562ab0ff3304a";
  }

  static const char* value(const ::cartographer_ros_msgs::TrajectorySubmapList_<ContainerAllocator>&) { return value(); }
  static const uint64_t static_value1 = 0x408b96b1bf0386eeULL;
  static const uint64_t static_value2 = 0x3f0562ab0ff3304aULL;
};

template<class ContainerAllocator>
struct DataType< ::cartographer_ros_msgs::TrajectorySubmapList_<ContainerAllocator> >
{
  static const char* value()
  {
    return "cartographer_ros_msgs/TrajectorySubmapList";
  }

  static const char* value(const ::cartographer_ros_msgs::TrajectorySubmapList_<ContainerAllocator>&) { return value(); }
};

template<class ContainerAllocator>
struct Definition< ::cartographer_ros_msgs::TrajectorySubmapList_<ContainerAllocator> >
{
  static const char* value()
  {
    return "# Copyright 2016 The Cartographer Authors\n\
#\n\
# Licensed under the Apache License, Version 2.0 (the \"License\");\n\
# you may not use this file except in compliance with the License.\n\
# You may obtain a copy of the License at\n\
#\n\
#      http://www.apache.org/licenses/LICENSE-2.0\n\
#\n\
# Unless required by applicable law or agreed to in writing, software\n\
# distributed under the License is distributed on an \"AS IS\" BASIS,\n\
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.\n\
# See the License for the specific language governing permissions and\n\
# limitations under the License.\n\
\n\
SubmapEntry[] submap\n\
\n\
================================================================================\n\
MSG: cartographer_ros_msgs/SubmapEntry\n\
# Copyright 2016 The Cartographer Authors\n\
#\n\
# Licensed under the Apache License, Version 2.0 (the \"License\");\n\
# you may not use this file except in compliance with the License.\n\
# You may obtain a copy of the License at\n\
#\n\
#      http://www.apache.org/licenses/LICENSE-2.0\n\
#\n\
# Unless required by applicable law or agreed to in writing, software\n\
# distributed under the License is distributed on an \"AS IS\" BASIS,\n\
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.\n\
# See the License for the specific language governing permissions and\n\
# limitations under the License.\n\
\n\
uint32 submap_version\n\
geometry_msgs/Pose pose\n\
\n\
================================================================================\n\
MSG: geometry_msgs/Pose\n\
# A representation of pose in free space, composed of position and orientation. \n\
Point position\n\
Quaternion orientation\n\
\n\
================================================================================\n\
MSG: geometry_msgs/Point\n\
# This contains the position of a point in free space\n\
float64 x\n\
float64 y\n\
float64 z\n\
\n\
================================================================================\n\
MSG: geometry_msgs/Quaternion\n\
# This represents an orientation in free space in quaternion form.\n\
\n\
float64 x\n\
float64 y\n\
float64 z\n\
float64 w\n\
";
  }

  static const char* value(const ::cartographer_ros_msgs::TrajectorySubmapList_<ContainerAllocator>&) { return value(); }
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

  template<class ContainerAllocator> struct Serializer< ::cartographer_ros_msgs::TrajectorySubmapList_<ContainerAllocator> >
  {
    template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
    {
      stream.next(m.submap);
    }

    ROS_DECLARE_ALLINONE_SERIALIZER
  }; // struct TrajectorySubmapList_

} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::cartographer_ros_msgs::TrajectorySubmapList_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const ::cartographer_ros_msgs::TrajectorySubmapList_<ContainerAllocator>& v)
  {
    s << indent << "submap[]" << std::endl;
    for (size_t i = 0; i < v.submap.size(); ++i)
    {
      s << indent << "  submap[" << i << "]: ";
      s << std::endl;
      s << indent;
      Printer< ::cartographer_ros_msgs::SubmapEntry_<ContainerAllocator> >::stream(s, indent + "    ", v.submap[i]);
    }
  }
};

} // namespace message_operations
} // namespace ros

#endif // CARTOGRAPHER_ROS_MSGS_MESSAGE_TRAJECTORYSUBMAPLIST_H
