#pragma once

#include <iostream>
#include <rclcpp/rclcpp.hpp>

#include <px4_msgs/msg/offboard_control_mode.hpp>
#include <px4_msgs/msg/vehicle_command.hpp>
#include <px4_msgs/msg/vehicle_control_mode.hpp>
#include <px4_msgs/msg/trajectory_setpoint.hpp>
#include <px4_msgs/msg/vehicle_rates_setpoint.hpp>



using namespace px4_msgs::msg;
using std::cout;
using std::endl;



class OffboardNode : public rclcpp::Node
{

    public:
        OffboardNode();
        ~OffboardNode() override;


    private:

	    uint64_t offboard_setpoint_counter_;   //!< counter for the number of setpoints sent

        // Publishers
        rclcpp::Publisher<OffboardControlMode>::SharedPtr offboard_control_mode_publisher_;
	    rclcpp::Publisher<VehicleCommand>::SharedPtr vehicle_command_publisher_;
        rclcpp::Publisher<VehicleRatesSetpoint>::SharedPtr control_publisher_;
        rclcpp::Publisher<TrajectorySetpoint>::SharedPtr trajectory_setpoint_publisher_;


        // Run Timers
        rclcpp::TimerBase::SharedPtr offboard_control_mode_timer_;
        void offboard_mode_timer_callback();
        rclcpp::TimerBase::SharedPtr control_timer_;
        void publish_control_timer_callback();


        void publish_offboard_control_heartbeat_signal_position();
        void publish_offboard_control_heartbeat_signal_bodyrate();
        void publish_vehicle_command(uint16_t command, float param1 = 0.0, float param2 = 0.0);
        void publish_position_setpoint(float x, float y, float z, float yaw);
        void arm();

        
};