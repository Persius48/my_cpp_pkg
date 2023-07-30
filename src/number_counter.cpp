#include "rclcpp/rclcpp.hpp"
#include "example_interfaces/msg/int64.hpp"
#include "example_interfaces/srv/set_bool.hpp"

using std::placeholders::_1;
using std::placeholders::_2;

class NumberCounterNode : public rclcpp::Node // MODIFY NAME
{
public:
    NumberCounterNode() : Node("number_counter") // MODIFY NAME
    {   
        num_publisher_ = this->create_publisher<example_interfaces::msg::Int64>("num_count", 10);
        num_subscriber_ = this->create_subscription<example_interfaces::msg::Int64>("num_channel", 10,
                                                    std::bind(&NumberCounterNode::callbackNumberChannel,
                                                    this, std::placeholders::_1));
        server_ = this->create_service<example_interfaces::srv::SetBool>(
            "reset_counter",
            std::bind(&NumberCounterNode::callbackCounterReset, this, _1, _2));
        RCLCPP_INFO(this->get_logger(), "Service server has been started.");

        RCLCPP_INFO(this->get_logger(),"NumberCounter has been started. ");
    }

private:
    void callbackNumberChannel(const example_interfaces::msg::Int64::SharedPtr msg)
    {
        //RCLCPP_INFO(this->get_logger(), "%ld", msg->data);
        counter_ += msg->data ;
        auto new_msg = example_interfaces::msg::Int64();
        new_msg.data = counter_ ;
        num_publisher_ ->publish(new_msg);


    }
    void callbackCounterReset(const example_interfaces::srv::SetBool::Request::SharedPtr request,
                            const example_interfaces::srv::SetBool::Response::SharedPtr response)
    {
        if (request->data){
            counter_ = 0;
            response->success = true;
            response->message = "counter sucessfully reset" ;
            RCLCPP_INFO(this->get_logger(),"%s",response->message.c_str());

        }
        else{
            response->success = false;
            response->message = "counter reset failed" ;
        }
    }
    int counter_ {};
    rclcpp::Subscription<example_interfaces::msg::Int64>::SharedPtr num_subscriber_;
    rclcpp::Publisher<example_interfaces::msg::Int64>::SharedPtr num_publisher_;
    rclcpp::Service<example_interfaces::srv::SetBool>::SharedPtr server_;
    
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<NumberCounterNode>(); // MODIFY NAME
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}