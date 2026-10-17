#include <rclcpp/rclcpp.hpp>
#include <cstdio>
#include <chrono>
#include <memory>
#include <string>
#include <sensor_msgs/msg/laser_scan.hpp>
#include "rclcpp/logger.hpp"
#include "tk2_msgs/msg/kartmsg.hpp"
#include "tk2_msgs/msg/kartdata.hpp"

namespace tk2_main{

class TK2_Main_Node : public rclcpp::Node {
    public:
        explicit TK2_Main_Node();

    private:
        rclcpp::Publisher<tk2_msgs::msg::Kartmsg>::SharedPtr drive_pub_;
        rclcpp::Subscription<sensor_msgs::msg::LaserScan>::SharedPtr lidar_sub_;
        rclcpp::Subscription<tk2_msgs::msg::Kartdata>::SharedPtr kart_sub_;
        rclcpp::TimerBase::SharedPtr timer;

        double xvel;
        double yvel;
        double zvel;

        void lidar_callback(sensor_msgs::msg::LaserScan::SharedPtr msg);
        void kart_callback(tk2_msgs::msg::Kartdata::SharedPtr msg);
    };
}
