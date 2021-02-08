/**
* listener_zmq.cpp
* Author: Ravi Joshi
* Date: 2018/01/18
*/

#include <ros/ros.h>
#include <zmq.hpp>

int main(int argc, char** argv)
{
    ros::init(argc, argv, "listener_zmq", ros::init_options::AnonymousName);

    zmq::context_t context(1);
    zmq::socket_t subscriber(context, ZMQ_SUB);

    std::string TOPIC = "";
    subscriber.setsockopt(ZMQ_SUBSCRIBE, TOPIC.c_str(), TOPIC.length()); // allow all messages

    int linger = 0; // Proper shutdown ZeroMQ
    subscriber.setsockopt(ZMQ_LINGER, &linger, sizeof(linger));

    subscriber.connect("tcp://10.211.55.2:6666");

    int count = 0;
    while (ros::ok())
    {
        zmq::message_t message;
        int rc = subscriber.recv(&message);
        char* ptr = (char*)message.data();
        if (rc)
        {
            count++;
            ROS_INFO("Received %d %d %d %d %d", count, (int)ptr[0], (int)ptr[100], (int)ptr[1000], (int)ptr[100000]);
            //ROS_INFO("Received %d %s", count, ptr);
        }
    }

    // Clean up your socket and context here
    subscriber.close();
    context.close();
    return 0;
}
