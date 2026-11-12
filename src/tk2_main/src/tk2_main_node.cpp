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
  //Kart sub, gets velocity data from the kart
  kart_sub_ = this->create_subscription<tk2_msgs::msg::Kartdata>("/kart_data", 10,
    std::bind(&TK2_Main_Node::kart_callback, this, std::placeholders::_1));
};

//  Lidar Callback, gets called when lidar_sub_ gets a new message (~10 times a second)
//  !!Put your main steering and navigation logic here!!
void TK2_Main_Node::lidar_callback(sensor_msgs::msg::LaserScan::SharedPtr msg){
  /*  This function passes the most recent LiDAR message as the 'msg' variable.
      You can access data from msg by using the arrow notation, as in 'msg-><data field>'
      For the most part, you will be grabbing the ranges data from the LiDAR (see: lidarOverview.md in the docs)
      Do this with 'msg->ranges[<index of scan ray>]'
  */

  /* Math Variables */
  constexpr double pi = 3.14159265358979323846; // Changed from M_PI
  constexpr double forward_half_angle = pi / 4.0;  // 45 degrees
  constexpr double stop_distance = 0.75;          // meters; Temporary rn. Needs proper testing

  /* Defining Obstacle Detected Variables */
  bool obstacle_detected = false;
  bool invalid_forward_reading = false;
  std::size_t forward_readings = 0;

  /* Check Data if it's Valid */
  const bool metadata_valid =
    // 1. All floats must be finite numbers
    std::isfinite(msg->angle_min) &&
    std::isfinite(msg->angle_max) &&
    std::isfinite(msg->angle_increment) &&
    std::isfinite(msg->range_min) &&
    std::isfinite(msg->range_max) &&

    // 2. Range constraints
    msg->range_min >= 0.0f &&
    msg->range_max > msg->range_min &&

    // 3. Increment must be non-zero and point in the direction of the scan
    msg->angle_increment != 0.0f &&
    ((msg->angle_max > msg->angle_min && msg->angle_increment > 0.0f) ||
     (msg->angle_max < msg->angle_min && msg->angle_increment < 0.0f));

  if (metadata_valid) {
      // Parse for Only the FRONT of TinyKart™
      for (std::size_t i = 0; i < msg->ranges.size(); ++i) {
          // Which Direction does it Point to
          const double angle =
              msg->angle_min + i * static_cast<double>(msg->angle_increment);
          // Turns to -180 < x < 180; Calculations in Radians
          const double wrapped_angle =
              std::atan2(std::sin(angle), std::cos(angle));
          // Skip readings outside the forward 90-degree region.
          if (std::abs(wrapped_angle) > forward_half_angle) {
              continue;
          }

          ++forward_readings;
          const double distance = msg->ranges[i];

          if (!std::isfinite(distance)
          ||distance < msg->range_min
          ||distance > msg->range_max) {
              invalid_forward_reading = true;
              continue;
          }

          if (distance <= stop_distance) {
            obstacle_detected = true;
          }
        }
      }
  // Act on the Readings!!
  const bool stop_required =
        !metadata_valid ||
        forward_readings == 0 ||
        invalid_forward_reading ||
        obstacle_detected;

    auto kartcmd = tk2_msgs::msg::Kartmsg();

    // Keep the kart stationary during development.
    kartcmd.throttle = 0;

    if (stop_required) {
      kartcmd.throttle = 0;

      RCLCPP_WARN_THROTTLE(
          this->get_logger(), *this->get_clock(), 2000,
          "Stop required: obstacle ahead or insufficient valid scan data");
    }

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
