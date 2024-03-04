#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <sensor_msgs/Joy.h>
#include <std_msgs/Int32.h>
#include <std_msgs/Int32MultiArray.h>

#include <std_msgs/UInt8MultiArray.h>

geometry_msgs::Twist cmd_vel;
std_msgs::Int32 uvg_button;
std_msgs::Int32MultiArray walk_button;

//std_msgs::UInt8MultiArray fcontact;


void joy_callback(const sensor_msgs::Joy& joy_msg)
{
  cmd_vel.linear.x = joy_msg.axes[1]/2;
  cmd_vel.linear.y = joy_msg.axes[0]/2;
  cmd_vel.angular.z =joy_msg.axes[3]*0.7;
}

void button_callback(const sensor_msgs::Joy& joy_msg)
{
  if(joy_msg.buttons.size()>0){
    uvg_button.data = joy_msg.buttons[1];
  }
}

void walk_callback(const sensor_msgs::Joy& joy_msg)
{
  if(joy_msg.buttons.size()>0){
    walk_button.data = joy_msg.buttons;
  }
}

/*void fcontact_callback(const controllertwist::ContactsStamped& contact_msg)
{
  //fcontact.data = contact_msg.contacts;
  q1.push(contact_msg);
 if(q1.size() > 40){
   fcontact.data =q1.front().contacts;
    q1.pop();
 }

}*/



int main(int argc, char** argv)
{
  ros::init(argc, argv, "basic_twist_publisher");
  ros::NodeHandle nh;
  ros::Publisher cmd_pub = nh.advertise<geometry_msgs::Twist>("/final_cmd_vel", 1000);
  ros::Publisher button_pub = nh.advertise<std_msgs::Int32>("uvg_button", 1000);
  ros::Publisher walk_pub = nh.advertise<std_msgs::Int32MultiArray>("walk_button", 1000);
  //ros::Publisher fcontact_pub = nh.advertise<std_msgs::UInt8MultiArray>("fcontact", 100);

  ros::Subscriber joy_sub = nh.subscribe("joy", 10, joy_callback);
  ros::Subscriber button_sub = nh.subscribe("joy", 10, button_callback);
  ros::Subscriber walk_sub = nh.subscribe("joy", 10, walk_callback);
  //ros::Subscriber fcontact_sub = nh.subscribe("foot_contacts", 10, fcontact_callback);
  walk_button.data.resize(30);

  ros::Rate loop_rate(10);
  while (ros::ok())
  {
    cmd_pub.publish(cmd_vel);
    button_pub.publish(uvg_button);
    walk_pub.publish(walk_button);
    //fcontact_pub.publish(fcontact);    
    ros::spinOnce();
    loop_rate.sleep();
  }
  return 0;
}
