# ROS 系统实践 — 实验报告

> ROS Noetic | 智行-W2A 机器人平台 | Ubuntu 20.04

## 实验内容

本仓库为 ROS 系统实践课程第一次实验代码，涵盖 ROS 三大核心通信机制：

- **话题（Topic）**：发布/订阅模式，异步通信
- **服务（Service）**：请求/响应模式，同步通信
- **动作（Action）**：带反馈的异步任务执行

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

## 环境准备

```bash
cd ~/d_ros_class_ws
catkin_make
source devel/setup.bash
```

## 实验一：话题通信（Topic）

### C++ 标准消息

```bash
roscore
rosrun my_class_pkg ros_publisher_node     # 终端2 — 发布者
rosrun my_class_pkg ros_subscriber_node    # 终端3 — 订阅者
rostopic echo /my_topic                    # 查看话题内容
```

### Python 标准消息

```bash
rosrun my_class_pkg ros_publisher_node.py
rosrun my_class_pkg ros_subscriber_node.py
```

### C++ 自定义消息（MyMessage）

```bash
rosrun my_class_pkg msg_publisher_node
rosrun my_class_pkg msg_subscriber_node
rostopic echo /my_msg_topic
```

### Launch 一键启动

```bash
roslaunch my_class_pkg bringup_topic.launch
```

## 实验二：服务通信（Service）

### C++

```bash
roscore
rosrun my_class_pkg ros_server_node        # 服务端
rosrun my_class_pkg ros_client_node        # 客户端（发送请求 input=42）
```

### Python

```bash
rosrun my_class_pkg ros_server.py
rosrun my_class_pkg ros_client.py
```

## 实验三：动作通信（Action）

### C++

```bash
roscore
rosrun my_class_pkg ros_action_server      # 动作服务端（10步执行）
rosrun my_class_pkg ros_action_client      # 动作客户端（发送目标）
```

### Python

```bash
rosrun my_class_pkg ros_action_server.py
rosrun my_class_pkg ros_action_client.py
```

## 验证自定义消息/服务/动作

```bash
source ~/d_ros_class_ws/devel/setup.bash
rosmsg show my_class_pkg/MyMessage
rossrv show my_class_pkg/MyServiceMsg
rosmsg show my_class_pkg/MyActionGoal
```

## 技术栈

| 组件 | 版本 |
|------|------|
| ROS | Noetic |
| OS | Ubuntu 20.04 |
| 语言 | C++ / Python3 |
| 构建 | catkin_make |
