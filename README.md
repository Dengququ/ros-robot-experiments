# ROS 机器人实验仓库

本仓库包含两个 ROS 工作空间：

## 工作空间说明

### djf_ros_class_ws（第二周完整实验）
包含主题消息、服务、动作、传感器实验的完整代码。

```bash
cd djf_ros_class_ws
catkin_make
source devel/setup.bash
```

**节点列表及启动指令详见** `djf_ros_class_ws/README.md`

---

### ros_class_ws_dqqself（主题消息基础实验）
包含标准消息发布/订阅的基础代码。

```bash
cd ros_class_ws_dqqself
catkin_make
source devel/setup.bash
```

#### 启动发布者
```bash
rosrun my_class_pkg ros_publisher_node      # C++
rosrun my_class_pkg ros_publisher_node.py   # Python
```

#### 启动订阅者
```bash
rosrun my_class_pkg ros_subscriber_node      # C++
rosrun my_class_pkg ros_subscriber_node.py   # Python
```

---

## 克隆仓库

```bash
git clone https://github.com/Dengququ/ros-robot-experiments.git
```
