# 项目文件结构说明

## 目录组织

本项目包含多个模块，文件分布在不同目录中：

### 1. 主窗口文件（MainWindow）

**实际使用的文件：**
- 📄 `include/ui/mainwindow.h` - 主窗口头文件（类声明）
- 📄 `src/ui/mainwindow.cpp` - 主窗口实现文件（类实现）
- 📄 `src/forms/mainwindow.ui` - 主窗口UI设计文件（Qt Designer）

**用途：** 客户端程序的主窗口，包含用户信息输入和GPA计算功能

---

### 2. 训练计划窗口文件（TrainWindow）

**文件位置：**
- 📄 `include/ui/trainwindow.h` - 训练窗口头文件
- 📄 `src/ui/trainwindow.cpp` - 训练窗口实现文件
- 📄 `src/forms/trainwindow.ui` - 训练窗口UI设计文件

**用途：** 训练计划管理界面，包含日历、训练计划列表、动作和组数管理

---

### 3. 客户端网络模块（ClientConnection）

**文件位置：**
- 📄 `include/client/clientconnection.h` - 客户端连接头文件
- 📄 `src/client/clientconnection.cpp` - 客户端连接实现文件
- 📄 `src/client/main.cpp` - 客户端程序入口

**用途：** 处理与服务器的TCP/IP网络通信

---

### 4. 服务器模块（Server）

**文件位置：**
- 📄 `include/server/server.h` - 服务器主类头文件
- 📄 `src/server/server.cpp` - 服务器主类实现
- 📄 `include/server/clienthandler.h` - 客户端处理器头文件
- 📄 `src/server/clienthandler.cpp` - 客户端处理器实现
- 📄 `src/server/main.cpp` - 服务器程序入口

**用途：** 接收客户端数据，计算GPA并返回结果

---

### 5. 核心业务逻辑（Common Library）

**用户分析模块：**
- 📄 `include/analyseuser.h` - 用户类型定义（基础用户、举重用户等）
- 📄 `src/analyse.cpp` - 用户GPA计算实现

**用户数据模块：**
- 📄 `include/ui/userdata.h` - 用户数据管理类
- 📄 `src/ui/userdata.cpp` - 用户数据处理实现

**训练计划模块：**
- 📄 `include/train.h` - 训练计划数据结构（set, actions, train, date）
- 📄 `src/train.cpp` - 训练计划功能实现

---

## CMake编译目标

### sport_evaluation_client（客户端程序）
使用文件：
- `src/ui/mainwindow.cpp`
- `include/ui/mainwindow.h`
- `src/forms/mainwindow.ui`
- `src/ui/trainwindow.cpp`
- `include/ui/trainwindow.h`
- `src/forms/trainwindow.ui`
- `src/client/main.cpp`
- `src/client/clientconnection.cpp`

### sport_evaluation_server（服务器程序）
使用文件：
- `src/server/main.cpp`
- `src/server/server.cpp`
- `src/server/clienthandler.cpp`

### sport_evaluation_common（共享库）
使用文件：
- `src/analyse.cpp`
- `src/ui/userdata.cpp`
- `src/train.cpp`

---

## 重要提示

⚠️ **注意事项：**

1. **主窗口文件只有一组**：
   - ✅ 使用：`src/ui/mainwindow.cpp` + `include/ui/mainwindow.h` + `src/forms/mainwindow.ui`
   - ❌ 已删除：`src/client/mainwindow.*`（这些是错误创建的重复文件）

2. **UI文件位置**：
   - 所有`.ui`文件都在`src/forms/`目录下
   - UI头文件在`include/ui/`目录下
   - UI实现文件在`src/ui/`目录下

3. **编辑文件时请注意**：
   - 修改主窗口功能 → 编辑`src/ui/mainwindow.cpp`
   - 修改UI布局 → 编辑`src/forms/mainwindow.ui`
   - 修改类声明 → 编辑`include/ui/mainwindow.h`

---

## 文件编辑指南

### 当你需要修改主窗口时：

1. **修改UI界面** → 打开 `src/forms/mainwindow.ui`（使用Qt Designer）
2. **添加槽函数** → 在 `include/ui/mainwindow.h` 中声明，在 `src/ui/mainwindow.cpp` 中实现
3. **修改业务逻辑** → 编辑 `src/ui/mainwindow.cpp`

### 当你需要修改训练计划时：

1. **修改UI界面** → 打开 `src/forms/trainwindow.ui`
2. **修改功能** → 编辑 `src/ui/trainwindow.cpp`
3. **修改数据结构** → 编辑 `include/train.h` 和 `src/train.cpp`

---

**最后更新：** 2025年11月12日
