#include <actionlib/client/simple_action_client.h>
#include <fiducial_msgs/FiducialTransformArray.h>
#include <geometry_msgs/Twist.h>           //for geometry_msgs::Twist
#include <move_base_msgs/MoveBaseAction.h> // Move Base header file
#include <tf2/LinearMath/Quaternion.h>
#include <tf2_ros/transform_broadcaster.h>
#include <tf2_ros/transform_listener.h>
#include <ros/ros.h>
#include <iostream>
#include <array>
#include <cmath>
#include <math.h>
typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

int fiducial_ID;
/**
 * @brief This function is used to handle messages received by the subscriber
 * These messages are then broadcasted onto a different topic called /tf
 * 
 * @param msg This parameter is the pointer which is the buffer for all the messages subscribed by the subscriber.
 */
void fiducial_callback(const fiducial_msgs::FiducialTransformArray::ConstPtr &msg)
{
  //check if aruco marker is detected
  if (!msg->transforms.empty())
  {
    static tf2_ros::TransformBroadcaster broadcaster;
    geometry_msgs::TransformStamped transformStamped;

    transformStamped.header.stamp = ros::Time::now();
    transformStamped.header.frame_id = "explorer_tf/camera_rgb_optical_frame";
    transformStamped.child_frame_id = "marker_frame";
    transformStamped.transform.translation.x = msg->transforms[0].transform.translation.x;
    transformStamped.transform.translation.y = msg->transforms[0].transform.translation.y;
    transformStamped.transform.translation.z = msg->transforms[0].transform.translation.z;

    transformStamped.transform.rotation.x = msg->transforms[0].transform.rotation.x;
    transformStamped.transform.rotation.y = msg->transforms[0].transform.rotation.y;
    transformStamped.transform.rotation.z = msg->transforms[0].transform.rotation.z;
    transformStamped.transform.rotation.w = msg->transforms[0].transform.rotation.w;

    fiducial_ID = msg->transforms[0].fiducial_id;

    //broadcast the marker frame pose to tf topic
    broadcaster.sendTransform(transformStamped);
  }
}
/**
 * @brief listen function transforms the marker frame to map frame
 * This function is used to listen to the broadcasted message in the fiducial callback function and feed the data for the follower to move around
 * 
 * @param tfBuffer This varible is an buffer if the tf2_ros
 * @param aruco_locations This 2D array stores the aruco locations according to the fiducial ID's
 * @param explorer_x This variable is used to store the current x- coordinate of the explorer when the listener is called
 * @param explorer_y This variable is used to store the current y- coordinate of the explorer when the listener is called
 */
void listen(tf2_ros::Buffer &tfBuffer, std::array<std::array<double, 2>, 4> &aruco_locations, double explorer_x, double explorer_y)
{
  geometry_msgs::TransformStamped transformStamped;
  std::array<std::array<double, 2>, 4> explorer_array;
  try
  {
    //transform the marker frame to map frame and store the pose of the marker
    transformStamped = tfBuffer.lookupTransform("map", "marker_frame", ros::Time(0));
    auto trans_x = transformStamped.transform.translation.x;
    auto trans_y = transformStamped.transform.translation.y;
    auto trans_z = transformStamped.transform.translation.z;
    auto rot_x = transformStamped.transform.rotation.x;
    auto rot_y = transformStamped.transform.rotation.y;
    auto rot_z = transformStamped.transform.rotation.z;
    auto rot_w = transformStamped.transform.rotation.w;

    //store the location of the marker as per fiducial ID
    if (fiducial_ID == 0)
    {
      explorer_array[0][0] = explorer_x;
      explorer_array[0][1] = explorer_y;
      aruco_locations[0][0] = (double(trans_x + explorer_array[0][0]) / double(2.0));
      aruco_locations[0][1] = (double(trans_y + explorer_array[0][1]) / double(2.0));
    }
    else if (fiducial_ID == 1)
    {
      explorer_array[1][0] = explorer_x;
      explorer_array[1][1] = explorer_y;

      aruco_locations[1][0] = (double(trans_x + explorer_array[1][0]) / double(2.0));
      aruco_locations[1][1] = (double(trans_y + explorer_array[1][1]) / double(2.0));
    }
    else if (fiducial_ID == 2)
    {
      explorer_array[2][0] = explorer_x;
      explorer_array[2][1] = explorer_y;

      aruco_locations[2][0] = (double(trans_x + explorer_array[2][0]) / double(2.0));
      aruco_locations[2][1] = (double(trans_y + explorer_array[2][1]) / double(2.0));
    }
    else if (fiducial_ID == 3)
    {
      explorer_array[3][0] = explorer_x;
      explorer_array[3][1] = explorer_y;

      aruco_locations[3][0] = (double(trans_x + explorer_array[3][0]) / double(2.0));
      aruco_locations[3][1] = (double(trans_y + explorer_array[3][1]) / double(2.0));
    }
    else
    {
      ROS_INFO_STREAM("No fiducial ID left or all fiducial IDs have been transformed");
    }
  }
  catch (tf2::TransformException &ex)
  {
    ROS_WARN("%s", ex.what());
    ros::Duration(1.0).sleep();
  }
}

int main(int argc, char **argv)
{
// These variables store the boolean values whether the goals are sent to the client or not. Where true is sent and false is not yet sent
  bool explorer_goal_sent[5] = {false, false, false, false, false};
  bool follower_goal_sent[5] = {false, false, false, false, false};
// This varible stores the aruco marker locations
  std::array<std::array<double, 2>, 4> aruco_locations{0.0};
// Here we are are initializing a node
  ros::init(argc, argv, "simple_navigation_goals");
  ros::NodeHandle nh;// Creation of a node object
// Defining a Subscriber to subscribe to the topic "/fiducial_transforms" with fiducial callback as the call back function and queue size as 1000.
  ros::Subscriber sub_fid = nh.subscribe("fiducial_transforms", 1000, &fiducial_callback);
//Defining a Publisher to advertise on the topic "/explorer/cmd_vel" with  a queue size of 2000
  ros::Publisher pub = nh.advertise<geometry_msgs::Twist>("explorer/cmd_vel", 2000);
  geometry_msgs::Twist msg;// Creating an object of geometry messages to publish messages.
  msg.linear.x = 0;
  msg.linear.y = 0;
  msg.angular.z = 0.3;

  XmlRpc::XmlRpcValue target;// This is a used to retrieve the parameters from parameter server.
  std::array<std::array<double, 2>, 4> arr;// This is used to convert the values from XMLRPC type to double and store for use.
  std::string str = "/aruco_lookup_locations/target_";
  std::string temp;
  for (int i = 0; i < 4; i++)
  {
    temp = str + std::to_string(i + 1);// Concatenation of the string to use in the getParam. 
    nh.getParam(temp, target);// This a function which is used to get Paramters from the paramter server by taking the source and target
    for (int j = 0; j < 2; j++)
    {
      arr[i][j] = static_cast<double>(target[j]);// Used for static casting the values into double type.
    }
  }

  MoveBaseClient explorer_client("/explorer/move_base", true);// Defining the explorer client 

  MoveBaseClient follower_client("/follower/move_base", true);// Defining the follower client 

  // The explorer client waits for the server to start.
  while (!explorer_client.waitForServer(ros::Duration(5.0)))
  {
    ROS_INFO("Waiting for the move_base action server to come up for explorer");
  }
// The follower client waits for the server to start.
  while (!follower_client.waitForServer(ros::Duration(5.0)))
  {
    ROS_INFO("Waiting for the move_base action server to come up for follower");
  }
  // Defining an array of movebase objects both for the follower and explorer.
  move_base_msgs::MoveBaseGoal explorer_goal[5];
  move_base_msgs::MoveBaseGoal follower_goal[5];

  for (int i = 0; i < 5; i++)
  {
    //Below are all the parameters set for each goal.
    explorer_goal[i].target_pose.header.frame_id = "map";
    explorer_goal[i].target_pose.header.stamp = ros::Time::now();
    explorer_goal[i].target_pose.pose.position.x = arr[i][0];
    explorer_goal[i].target_pose.pose.position.y = arr[i][1];
    explorer_goal[i].target_pose.pose.orientation.w = 1.0;
  }
  //This is the hardcoded goal coordinates the explorer has to go.
  explorer_goal[4].target_pose.header.frame_id = "map";
  explorer_goal[4].target_pose.header.stamp = ros::Time::now();
  explorer_goal[4].target_pose.pose.position.x = -4.000000;
  explorer_goal[4].target_pose.pose.position.y = 2.5000000;
  explorer_goal[4].target_pose.pose.orientation.w = 1.0;
//This is the hardcoded goal coordinates the follower has to go.
  follower_goal[4].target_pose.header.frame_id = "map";
  follower_goal[4].target_pose.header.stamp = ros::Time::now();
  follower_goal[4].target_pose.pose.position.x = -4.00000;
  follower_goal[4].target_pose.pose.position.y = 3.50000;
  follower_goal[4].target_pose.pose.orientation.w = 1.0;
//Creation of tf buffer objects for listener function.
  tf2_ros::Buffer tfBuffer;
  tf2_ros::TransformListener tfListener(tfBuffer);
  ros::Rate loop_rate(10);
//Counters for sending goals for explorer and follower
  int e = 0;
  int f = 0;
  while (ros::ok())
  {

    if (!explorer_goal_sent[e])
    {
      // ROS_INFO("Sending goal for explorer");
      explorer_client.sendGoal(explorer_goal[e]); //this should be sent only once
      explorer_goal_sent[e] = true;
    }
    //Checks whether the sent goal is reached or not
    if (explorer_client.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
    {
      int num = 0;
      while (num < 50 && (!explorer_goal_sent[4]))
      {
        //Publishing the messages onto the /explorer/cmd_vel
        pub.publish(msg);
        ros::spinOnce();// Start a thread to subscribe to the fiducial transforms
        listen(tfBuffer, aruco_locations, arr[e][0], arr[e][1]);// Calling the listener function to compute the transforms
        loop_rate.sleep();// Setting the looprate to sleep is to ensure that the custom cmd_vel commands are published.
        num++;
      }//To ensure no garbage values being added and no segmentation fault
      if (e < 4)
      {
        e += 1;
      }
    }
    //Check if the explorer reached the start position, only then start the follower
    if (explorer_goal_sent[4] && explorer_client.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
    {
      if (!follower_goal_sent[f])
      {
        if (f < 4)
        {
          //Assigning the goal positon to follower using the aruco_locations array
          follower_goal[f].target_pose.header.frame_id = "map";
          follower_goal[f].target_pose.header.stamp = ros::Time::now();
          follower_goal[f].target_pose.pose.position.x = (aruco_locations[f][0]);
          follower_goal[f].target_pose.pose.position.y = (aruco_locations[f][1]);
          follower_goal[f].target_pose.pose.orientation.w = 1.0;
        }
        //send goal to the follower client
        follower_client.sendGoal(follower_goal[f]);
        follower_goal_sent[f] = true;
      }
      //check if the follower reached the goal sent
      if (follower_client.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
      {
        //increment f to send the next goal to follower
        f += 1;
        loop_rate.sleep();
        ROS_INFO("Hooray, follower reached goal");
      }
    }
    //check if both explorer and follower reached the start position
    if (explorer_goal_sent[4] && follower_goal_sent[4] && follower_client.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
    {
      ROS_INFO_STREAM("Rescued everyone");
      ros::shutdown();
    }
  }
}