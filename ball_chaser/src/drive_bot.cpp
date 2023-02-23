#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "ball_chaser/DriveToTarget.h"
#include <string>
#include <sstream>

ros::Publisher motor_command_publisher;
ros::ServiceServer service;

// takes in request and response field from DriveToTarget.srv
bool drive_request (ball_chaser::DriveToTarget::Request& req, ball_chaser::DriveToTarget::Response& res)
{
	
	// creates object to publish
	geometry_msgs::Twist motor_command;

	// defines velocity and angle
	motor_command.linear.x = req.linear_x;
	motor_command.angular.z = req.angular_z;

	// publishes to robot
	motor_command_publisher.publish(motor_command);

	// defines response message
	res.msg_feedback = "request recieved: linear_x = %1.2f  , angluar_z = %1.2f", motor_command.linear.x, motor_command.angular.z;
	
	return true;
}

int main (int argc, char** argv)
{
	// initializes drive bot node
	ros::init(argc, argv, "ball_chaser");

	// creates a handle to the drive bot node
	ros::NodeHandle n;

	// informs ros master that we'll publish messages on the robot actuation topic
	motor_command_publisher = n.advertise<geometry_msgs::Twist>("/cmd_vel", 10);

	
	service = n.advertiseService("/ball_chaser/command_robot", drive_request);

	ros::spin();

	return 0;
}
