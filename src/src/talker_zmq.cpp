/**
* talker_zmq.cpp
* Author: Ravi Joshi
* Date: 2018/01/18
*/

#include <ros/ros.h>
#include <zmq.hpp>

int main(int argc, char** argv)
{
    ros::init(argc, argv, "talker_zmq", ros::init_options::AnonymousName);

    ros::Time::init(); // Workaround since we are not using NodeHandle
    ros::Rate loop_rate(60);

    //  Prepare our context and publisher
    zmq::context_t context(1);
    zmq::socket_t publisher(context, ZMQ_PUB);
    publisher.bind("tcp://*:6666");

constexpr size_t SIZE = 1929 * 1080 * 3 * sizeof(float);

    void* data = malloc(SIZE);
    memset(data, 1, SIZE);
    ROS_INFO("Data prepared %d", SIZE);
    int count = 0;
    while (ros::ok())
    {
        std::stringstream msg_str_stream;
        //msg_str_stream << "Hello_no._" << count;
        //std::string msg_str = msg_str_stream.str();

        zmq::message_t message(SIZE);
        ROS_INFO("Memcpy start");
        memcpy(message.data(), data, SIZE);
        ROS_INFO("Memcpy end");
        //ROS_INFO_STREAM(msg_str);
        ROS_INFO("Send start");
        publisher.send(message);
        ROS_INFO("Send end");
        loop_rate.sleep();
        ROS_INFO("Sent");
        count++;
        ROS_INFO("%d", count);
    }
    // Clean up your socket and context here
    publisher.close();
    context.close();

    return 0;
}
