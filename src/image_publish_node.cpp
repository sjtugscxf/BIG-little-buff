#include <cv_bridge/cv_bridge.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <ros/ros.h>
#include <sstream> // for converting the command line parameter to integer

#include <sys/time.h>
timeval timestart;

using namespace cv;

int main(int argc, char** argv)
{
    //Check if video source has been passed as a parameter
    ROS_INFO("Start!\n");
    if (argv[1] == NULL) {
        ROS_INFO("argv[1]=NULL, no video source\n");
        return 1;
    }

    ros::init(argc, argv, "image_publisher");
    ros::NodeHandle nh;
    image_transport::ImageTransport it(nh);
    image_transport::Publisher pub = it.advertise("camera/image", 2);

    cv::Mat frame;
    sensor_msgs::ImagePtr msg;
    ros::Rate loop_rate(30);
    while (nh.ok()) {
        cv::VideoCapture cap;
        if ('0' <= argv[1][0] && argv[1][0] <= '9')
            cap.open(argv[1][0] - '0');
        else
            cap.open(argv[1]);

        if (!cap.isOpened()) {
            ROS_INFO("can not opencv video device\n");
            return 1;
        }
        ROS_INFO("open successfully\n");


        while (cap.read(frame) && ros::ok()) {

            resize(frame, frame, Size(640, 480));
            imshow("src", frame);

            // Check if grabbed frame is actually full with some content
            if (!frame.empty()) {
                msg = cv_bridge::CvImage(std_msgs::Header(), "bgr8", frame).toImageMsg();
                pub.publish(msg);
            }

            waitKey(0);
            ros::spinOnce();
            loop_rate.sleep();
        }
    }
}
