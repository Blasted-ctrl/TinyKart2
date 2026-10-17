#include "tk2_main/tk2_main_node.hpp"

using namespace tk2_main;

//Node main function
TK2_Main_Node::TK2_Main_Node() : Node("tk2_main_node"){
  RCLCPP_INFO(this->get_logger(), "TK2 Main Node Started!");

  //Publishers and Subscribers

  //Drive pub, to publish messages for kart to use
  drive_pub_ = this->create_publisher<tk2_msgs::msg::Kartmsg>("/kart_cmd", 10);

  //Lidar sub, recieves messages from the LiDAR
  lidar_sub_ = this->create_subscription<sensor_msgs::msg::LaserScan>("scan", 10, 
    std::bind(&TK2_Main_Node::lidar_callback, this, std::placeholders::_1));

  kart_sub_ = this->create_subscription<tk2_msgs::msg::Kartdata>("/kart_data", 10, 
    std::bind(&TK2_Main_Node::kart_callback, this, std::placeholders::_1));
};

//Lidar Callback, gets called when lidar_sub_ gets a new message
void TK2_Main_Node::lidar_callback(sensor_msgs::msg::LaserScan::SharedPtr msg){
  //Create a blank message in the kartmessage format
  auto kartcmd = tk2_msgs::msg::Kartmsg();

  //Publishes the kartcmd message to the "/kart_cmd" topic
  this->drive_pub_->publish(kartcmd);
}

void TK2_Main_Node::kart_callback(tk2_msgs::msg::Kartdata::SharedPtr msg){
  xvel = msg->velocity.x;
  yvel = msg->velocity.y;
  zvel = msg->velocity.z;
}

//Starts up node
int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::NodeOptions options;
  rclcpp::spin(std::make_shared<tk2_main::TK2_Main_Node>());
  rclcpp::shutdown();
  return 0;
}