#include "ros/ros.h"
#include "ball_chaser/DriveToTarget.h"
#include <sensor_msgs/Image.h>

// to request services, we need to set up a client
ros::ServiceClient client;

// to request service DriveToTarget
void drive_robot (float lin_x, float ang_z)
{
    ball_chaser::DriveToTarget srv;
    srv.request.linear_x = lin_x;
    srv.request.angular_z = ang_z;

    if (!client.call(srv))
	ROS_ERROR("Failed to call service");
}

// to identify the white ball and drive towards it usign drive_robot
void process_image_callback (const sensor_msgs::Image img)
{

    int white_pixel = 255;
    int column = 0;
    bool found_ball = false;

    for (int i=0; i < img.height*img.step; i += 3)
    {
	if ((img.data[i] == 255) && (img.data[i+1] == 255) && (img.data[i+2] == 255))
	{
	    column = i % img.step;

	    if (column < img.step/3)
	    {
		drive_robot(0.5, 1);		
	    }
	    else if (column < (img.step/3 * 2))
	    {
		drive_robot(0.5, 0);
	    }
	    else
	    {
		drive_robot(0.5, -1);
	    }
	    found_ball = true;
	    break;
	}
    }

    if (found_ball == false)
	drive_robot(0,0);
}

int main (int argc, char** argv)
{
    // initializes process_image node and creates a handle to it
    ros::init(argc, argv, "process_image");
    ros::NodeHandle n;

    // defines a client service capable of requesting services from the command_robot
    client = n.serviceClient<ball_chaser::DriveToTarget>("ball_chaser/command_robot");

    // subscribe to /camera/rgb/image_raw topic to reaad the image data inside the process_iamge_ callback
    ros::Subscriber sub = n.subscribe("/camera/rgb/image_raw", 10, process_image_callback);

    // handle ros communication
    ros::spin();

    return 0;
}
