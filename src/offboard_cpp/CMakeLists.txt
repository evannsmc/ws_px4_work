cmake_minimum_required(VERSION 3.8)
project(offboard_cpp)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()


find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(px4_msgs REQUIRED)
find_package(std_msgs REQUIRED)

add_executable(run_node
  src/run_node.cpp
  src/offboard_node.cpp
)

target_include_directories(run_node PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

ament_target_dependencies(run_node
  rclcpp
  px4_msgs
  std_msgs
)

install(TARGETS run_node
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()