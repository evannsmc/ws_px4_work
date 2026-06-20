#include "offboard_cpp/offboard_node.hpp" 

#include <chrono>
#include <iostream>

using std::chrono::milliseconds;

OffboardNode::OffboardNode()
:   Node("minimal_publisher")
    {
        cout << "Initializing Node" << endl;

        offboard_control_mode_publisher_ = this->create_publisher<OffboardControlMode>("/fmu/in/offboard_control_mode", 10);
	    vehicle_command_publisher_ = this->create_publisher<VehicleCommand>("/fmu/in/vehicle_command", 10);
        control_publisher_ = this->create_publisher<VehicleRatesSetpoint>("/fmu/in/vehicle_rates_setpoint", 10);
        trajectory_setpoint_publisher_ = this->create_publisher<TrajectorySetpoint>("/fmu/in/trajectory_setpoint", 10);

        offboard_control_mode_timer_ = this->create_wall_timer(milliseconds(10), [this]() {offboard_mode_timer_callback();});
        control_timer_ = this->create_wall_timer(milliseconds(100), [this]() {publish_control_timer_callback();});

    }

OffboardNode::~OffboardNode()
    {
        cout << "Destroying Node" << endl;
    }


void OffboardNode::offboard_mode_timer_callback()
    {
        if (offboard_setpoint_counter_ == 10){
            // Change to Offboard mode after 10 setpoints
            this->publish_vehicle_command(VehicleCommand::VEHICLE_CMD_DO_SET_MODE, 1, 6);

            // Arm the vehicle
            this->arm();
        }
        if (offboard_setpoint_counter_ < 11){
            offboard_setpoint_counter_++;
        }

        // Implement state machine for different heartbeat signals at different times
        publish_offboard_control_heartbeat_signal_position();
        
    }


void OffboardNode::arm()
{
	publish_vehicle_command(VehicleCommand::VEHICLE_CMD_COMPONENT_ARM_DISARM, 1.0);

	RCLCPP_INFO(this->get_logger(), "Arm command send");
}

void OffboardNode::publish_control_timer_callback()
    {
        
        publish_position_setpoint(1.0,2.0,3.0,0.0);


    }

void OffboardNode::publish_position_setpoint(float x, float y, float z, float yaw)
    {  
        RCLCPP_INFO(this->get_logger(), "Publishing position setpoint");


        TrajectorySetpoint msg{};
        msg.position = {x, y, z};
        msg.yaw = yaw; // [-PI:PI]
        msg.timestamp = this->get_clock()->now().nanoseconds() / 1000;
        trajectory_setpoint_publisher_->publish(msg);

    }



void OffboardNode::publish_offboard_control_heartbeat_signal_position()
{
	OffboardControlMode msg{};
	msg.position = true;
	msg.velocity = false;
	msg.acceleration = false;
	msg.attitude = false;
	msg.body_rate = false;
	msg.timestamp = this->get_clock()->now().nanoseconds() / 1000;
	this->offboard_control_mode_publisher_->publish(msg);
}

void OffboardNode::publish_offboard_control_heartbeat_signal_bodyrate()
{
	OffboardControlMode msg{};
	msg.position = false;
	msg.velocity = false;
	msg.acceleration = false;
	msg.attitude = false;
	msg.body_rate = true;
	msg.timestamp = this->get_clock()->now().nanoseconds() / 1000;
	this->offboard_control_mode_publisher_->publish(msg);
}

void OffboardNode::publish_vehicle_command(uint16_t command, float param1, float param2)
{
	VehicleCommand msg{};
	msg.param1 = param1;
	msg.param2 = param2;
	msg.command = command;
	msg.target_system = 1;
	msg.target_component = 1;
	msg.source_system = 1;
	msg.source_component = 1;
	msg.from_external = true;
	msg.timestamp = this->get_clock()->now().nanoseconds() / 1000;
	vehicle_command_publisher_->publish(msg);
}