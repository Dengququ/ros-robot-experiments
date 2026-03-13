#include <ros/ros.h>
#include <sensor_msgs/Imu.h>
#include <geometry_msgs/Twist.h>
#include <nav_msgs/Odometry.h>
#include <tf/transform_datatypes.h> // 四元数转欧拉角

// 全局变量（状态控制+Odom数据）
enum RobotState {MOVE_STRAIGHT, ROTATE, STOP}; // 运动状态：直行/旋转/停止
RobotState current_state = MOVE_STRAIGHT;     // 初始状态：直行

// Odom相关变量
double current_x = 0.0, current_y = 0.0;      // 当前位置
double current_yaw = 0.0;                     // 当前偏航角（弧度）
double start_x = 0.0, start_y = 0.0;          // 直行起始位置
double start_yaw = 0.0;                       // 旋转起始角度

// 配置参数（可根据机器人调整）
const double STRAIGHT_DISTANCE = 1.0;         // 直行目标距离：1米
const double ROTATE_ANGLE = M_PI / 2.0;       // 旋转目标角度：90度（π/2弧度）
const double LINEAR_SPEED = 0.2;              // 直行速度：0.2m/s
const double ANGULAR_SPEED = 0.3;             // 旋转速度：0.3rad/s
int square_side_count = 0;                    // 已完成的正方形边数（目标4条）

// Odom回调函数：更新位置和角度
void odom_callback(const nav_msgs::OdometryConstPtr &odom_msg)
{
    // 更新当前位置
    current_x = odom_msg->pose.pose.position.x;
    current_y = odom_msg->pose.pose.position.y;

    // 四元数转欧拉角（提取偏航角yaw）
    tf::Quaternion q(
        odom_msg->pose.pose.orientation.x,
        odom_msg->pose.pose.orientation.y,
        odom_msg->pose.pose.orientation.z,
        odom_msg->pose.pose.orientation.w
    );
    tf::Matrix3x3 m(q);
    double roll, pitch, yaw;
    m.getRPY(roll, pitch, yaw);
    current_yaw = yaw;

    // 打印调试信息（可选）
    // ROS_INFO("Current: x=%.2f, y=%.2f, yaw=%.2f°", current_x, current_y, current_yaw*180/M_PI);
}

// 计算两点间距离
double calculate_distance(double x1, double y1, double x2, double y2)
{
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

// 计算角度差（处理正负，确保在-π~π之间）
double calculate_yaw_diff(double target_yaw, double current_yaw)
{
    double diff = target_yaw - current_yaw;
    // 归一化到-π~π
    if (diff > M_PI) diff -= 2 * M_PI;
    if (diff < -M_PI) diff += 2 * M_PI;
    return fabs(diff);
}

int main(int argc, char **argv)
{
    // 初始化ROS节点
    ros::init(argc, argv, "square_move_node");
    ros::NodeHandle nh;

    // 订阅Odom话题（获取位置/角度反馈）
    ros::Subscriber odom_sub = nh.subscribe<nav_msgs::Odometry>("/odom", 10, odom_callback);
    // 发布速度指令（控制机器人运动）
    ros::Publisher cmd_pub = nh.advertise<geometry_msgs::Twist>("/cmd_vel", 10);

    ros::Rate loop_rate(20); // 控制频率：20Hz（50ms一次）
    geometry_msgs::Twist cmd_vel;

    // 等待Odom数据初始化（避免初始值为0）
    ros::Duration(1.0).sleep();
    start_x = current_x;
    start_y = current_y;
    start_yaw = current_yaw;
    ROS_INFO("Start moving square (1m side)...");

    while (ros::ok() && square_side_count < 4)
    {
        switch (current_state)
        {
            // 状态1：直行1米
            case MOVE_STRAIGHT:
            {
                // 计算已走距离
                double distance = calculate_distance(start_x, start_y, current_x, current_y);
                
                // 未达到目标距离：继续直行
                if (distance < STRAIGHT_DISTANCE)
                {
                    cmd_vel.linear.x = LINEAR_SPEED;
                    cmd_vel.angular.z = 0.0;
                }
                // 达到目标距离：停止直行，切换到旋转状态
                else
                {
                    cmd_vel.linear.x = 0.0;
                    cmd_vel.angular.z = 0.0;
                    cmd_pub.publish(cmd_vel); // 立即停止
                    ROS_INFO("Straight %d finished: distance=%.2fm", square_side_count+1, distance);
                    
                    // 更新旋转起始角度，切换状态
                    start_yaw = current_yaw;
                    current_state = ROTATE;
                    ros::Duration(0.5).sleep(); // 短暂停顿，避免惯性
                }
                break;
            }

            // 状态2：旋转90度
            case ROTATE:
            {
                // 计算目标角度（当前起始角度+90度）
                double target_yaw = start_yaw + ROTATE_ANGLE;
                // 计算已旋转角度差
                double yaw_diff = calculate_yaw_diff(target_yaw, current_yaw);
                
                // 未达到目标角度：继续旋转
                if (yaw_diff > 0.05) // 误差阈值：0.05弧度（≈2.86°）
                {
                    cmd_vel.linear.x = 0.0;
                    cmd_vel.angular.z = ANGULAR_SPEED;
                }
                // 达到目标角度：停止旋转，切换回直行状态
                else
                {
                    cmd_vel.linear.x = 0.0;
                    cmd_vel.angular.z = 0.0;
                    cmd_pub.publish(cmd_vel); // 立即停止
                    ROS_INFO("Rotate %d finished: angle=%.2f°", square_side_count+1, yaw_diff*180/M_PI);
                    
                    // 更新直行起始位置，切换状态，计数+1
                    start_x = current_x;
                    start_y = current_y;
                    current_state = MOVE_STRAIGHT;
                    square_side_count++;
                    ros::Duration(0.5).sleep(); // 短暂停顿
                }
                break;
            }

            // 状态3：停止（备用）
            case STOP:
            {
                cmd_vel.linear.x = 0.0;
                cmd_vel.angular.z = 0.0;
                break;
            }
        }

        // 发布速度指令，处理回调，按频率休眠
        cmd_pub.publish(cmd_vel);
        ros::spinOnce();
        loop_rate.sleep();
    }

    // 正方形完成：停止所有运动
    cmd_vel.linear.x = 0.0;
    cmd_vel.angular.z = 0.0;
    cmd_pub.publish(cmd_vel);
    ROS_INFO("Square movement finished! Total sides: %d", square_side_count);

    return 0;
}
