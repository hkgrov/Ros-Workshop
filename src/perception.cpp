#include <ros/ros.h>
#include <geometry_msgs/PoseArray.h>
#include <geometry_msgs/Polygon.h>
// Define callbacks here



geometry_msgs::PoseArray boxmsg;
geometry_msgs::Point32 p;
geometry_msgs::Polygon polygonmsg;
void callback(const geometry_msgs::PoseArray& msg) {
  boxmsg = msg;
  geometry_msgs::Polygon poly;
  //ROS_WARN_STREAM(boxmsg.poses.size());
  for (int i = 0; i<boxmsg.poses.size(); i++){
    p.x = boxmsg.poses[i].position.x;
    p.y = boxmsg.poses[i].position.y;
    p.z = boxmsg.poses[i].position.z;
    poly.points.push_back(p);
  } 
  polygonmsg = poly;
  ROS_INFO("Message received");
}

int main(int argc, char** argv){
    // Initialise ROS
    ros::init(argc, argv, "perception_node");
    ros::NodeHandle n;
    ros::Subscriber sub = n.subscribe("/simulator/boxes", 1, callback);
    ros::Publisher pub = n.advertise<geometry_msgs::Polygon>("perception/boxes", 1);

    
    
    
    
    // Main loop
    ROS_INFO("perception is running");
    ros::Rate rate(30);
    while (ros::ok()) {
      
      ros::spinOnce();
      //polygonmsg.points.push_back(p);
      pub.publish(polygonmsg);
      rate.sleep();
    }

    return 0;
}


