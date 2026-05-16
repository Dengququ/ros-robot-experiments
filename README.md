# ROS 系统实践 — 第一周实验

## 主题与消息实验

> ROS Noetic | 智行-W2A 机器人平台 | Ubuntu 20.04

### 实验目的
- 熟悉 ROS 机器人通信系统架构
- 熟悉 ROS 中不同通信方式的特点
- 熟悉基于实验机器人平台的 ROS 开发方法

### 实验内容
- 使用 C++/Python 实现 ROS 标准消息的发布与订阅
- 自定义 ROS 消息类型
- 体验机器人上的主题与消息
- 使用 Launch 文件启动节点
- ROS 服务（Service）请求/响应通信
- ROS 动作（Action）异步任务通信

---

## 仓库结构

```
d_ros_class_ws/
└── src/
    └── my_class_pkg/
        ├── msg/
        │   └── MyMessage.msg           # 自定义消息（int32 key + string value）
        ├── srv/
        │   └── MyServiceMsg.srv        # 自定义服务（int64 input → int64 output）
        ├── action/
        │   └── MyAction.action         # 自定义动作（string → bool + float32 progress）
        ├── src/                        # C++ 节点
        │   ├── ros_publisher.cpp       # 标准消息发布者
        │   ├── ros_subscriber.cpp      # 标准消息订阅者
        │   ├── msg_publisher.cpp       # 自定义消息发布者
        │   ├── msg_subscriber.cpp      # 自定义消息订阅者
        │   ├── ros_server.cpp          # 服务端
        │   ├── ros_client.cpp          # 客户端
        │   ├── ros_action_server.cpp   # 动作服务端
        │   └── ros_action_client.cpp   # 动作客户端
        ├── scripts/                    # Python 节点
        │   ├── ros_publisher_node.py
        │   ├── ros_subscriber_node.py
        │   ├── ros_server.py
        │   ├── ros_client.py
        │   ├── ros_action_server.py
        │   └── ros_action_client.py
        └── launch/
            └── bringup_topic.launch    # 一键启动发布者+订阅者
```

---

## 环境准备

```bash
cd ~/d_ros_class_ws
catkin_make
source devel/setup.bash
```

---

## 实验步骤

### 5.1 C++ 标准消息（Topic）

```bash
# 终端1 — 启动 ROS Master
roscore

# 终端2 — 发布者
rosrun my_class_pkg ros_publisher_node

# 终端3 — 订阅者
rosrun my_class_pkg ros_subscriber_node

# 查看话题内容
rostopic echo /my_topic
```

### 5.2 Python 标准消息（Topic）

```bash
roscore
rosrun my_class_pkg ros_publisher_node.py    # 终端2
rosrun my_class_pkg ros_subscriber_node.py   # 终端3
```

### 5.3 C++ 自定义消息（MyMessage）

```bash
roscore
rosrun my_class_pkg msg_publisher_node       # 终端2
rosrun my_class_pkg msg_subscriber_node      # 终端3
rostopic echo /my_msg_topic
```

### 5.4 体验机器人上的主题与消息

```bash
# 启动机器人硬件
roslaunch upros_bringup bringup_w2a.launch

# 查看所有话题
rostopic list

# 查看里程计数据
rostopic echo /odom

# 键盘控制机器人
rosrun teleop_twist_keyboard teleop_twist_keyboard.py
```

### 5.5 Launch 文件一键启动

```bash
roslaunch my_class_pkg bringup_topic.launch
```

> 无需单独启动 roscore，launch 文件会自动处理。

---

## 服务实验（Service）

### C++

```bash
roscore
rosrun my_class_pkg ros_server_node    # 终端2 — 服务端
rosrun my_class_pkg ros_client_node    # 终端3 — 客户端
```

### Python

```bash
roscore
rosrun my_class_pkg ros_server.py
rosrun my_class_pkg ros_client.py
```

---

## 动作实验（Action）

### C++

```bash
roscore
rosrun my_class_pkg ros_action_server    # 终端2 — 动作服务端
rosrun my_class_pkg ros_action_client    # 终端3 — 动作客户端
```

### Python

```bash
roscore
rosrun my_class_pkg ros_action_server.py
rosrun my_class_pkg ros_action_client.py
```

---

## 验证自定义消息/服务/动作

```bash
source ~/d_ros_class_ws/devel/setup.bash
rosmsg show my_class_pkg/MyMessage
rossrv show my_class_pkg/MyServiceMsg
rosmsg show my_class_pkg/MyActionGoal
```

---

## 技术栈

| 组件 | 版本 |
|------|------|
| ROS | Noetic |
| OS | Ubuntu 20.04 |
| 语言 | C++ / Python3 |
| 构建 | catkin_make |
