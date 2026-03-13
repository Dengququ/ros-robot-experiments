#include <ros/ros.h>
#include <std_msgs/String.h>

int main(int argc,char** argv){
	ros::init(argc,argv,"my_publisher");
	ros::NodeHandle nh;
	ros::Publisher pub = nh.advertise<std_msgs::String>("my_topic",10);
	ros::Rate rate(1.0);
	while(ros::ok)
	{
		std_msgs::String msg;
		msg.data = "Hello,world!";
		pub.publish(msg);
		rate.sleep();
	}
	return 0;
}
