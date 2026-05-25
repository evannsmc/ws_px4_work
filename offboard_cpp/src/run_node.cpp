#include <rclcpp/rclcpp.hpp>
#include <iostream>

#include "offboard_cpp/offboard_node.hpp"

using std::cout;
using std::endl;

int main(int argc, char *argv[]){

  std::cout << "Starting offboard control node..." << std::endl;
	setvbuf(stdout, NULL, _IONBF, BUFSIZ);
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<OffboardNode>());
  rclcpp::shutdown();

  return 0;
}