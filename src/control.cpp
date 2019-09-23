#include <iostream>
#include <ros/ros.h>
#include <mavros_msgs/PositionTarget.h>
#include <geometry_msgs/Point.h>
#include <geometry_msgs/Point32.h>


geometry_msgs::Point positionmsg;
bool got_position = false;
mavros_msgs::PositionTarget waypoint;
void callback(const geometry_msgs::Point32& msg) {
  double x = msg.x;
  double y = msg.y;
  double z = msg.z;
  positionmsg.x = x;
  positionmsg.y = y;
  positionmsg.z = z + (2.1 - z);
  got_position = true;
  waypoint.position = positionmsg;
  ROS_INFO("Message received Control");
  ROS_WARN_STREAM(waypoint);
  
}



int main(int argc, char** argv){
    ros::init(argc, argv, "control_node");
    ros::NodeHandle n;
    ros::Subscriber sub = n.subscribe("/control/position_setpoint", 1, callback);
    ros::Publisher pub = n.advertise<mavros_msgs::PositionTarget>("/mavros/setpoint_raw/local", 1);


// Main loop
    ROS_INFO("control is running");
    ros::Rate rate(10);
    while (ros::ok()) {
        if(got_position == true){
            pub.publish(waypoint);
            ROS_INFO("Publishes Control Message");
        }
      ros::spinOnce();
      
      rate.sleep();
    }


    return 0;
}


