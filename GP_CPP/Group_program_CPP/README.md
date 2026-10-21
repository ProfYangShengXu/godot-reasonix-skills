# 体育评估系统 (Sport Evaluation System)

一个基于Qt 6的客户端-服务器架构的体育评估系统，支持用户信息管理、GPA计算和训练计划管理。

## 📋 目录

- [系统要求](#系统要求)
- [安装步骤](#安装步骤)
- [构建项目](#构建项目)
- [运行程序](#运行程序)
- [项目结构](#项目结构)
- [常见问题](#常见问题)
- [开发日志](#开发日志)

---

## 🖥️ 系统要求

- **Qt版本**：6.10.0 或更高版本
- **编译器**：MinGW 13.1.0 64-bit
- **CMake**：3.16 或更高版本
- **操作系统**：Windows 10/11
- **IDE**：Qt Creator（可选）或 Visual Studio Code

---

## 📦 安装步骤

### 1. 安装Qt

1. 访问 [Qt官网](https://www.qt.io/download-qt-installer) 下载 **Qt Online Installer**
2. 运行安装程序，勾选以下组件：
   - ✅ Qt 6.10.0 → MinGW 64-bit
   - ✅ Qt Creator（推荐）
   - ✅ Developer and Designer Tools → MinGW 13.1.0 64-bit
   - ✅ Developer and Designer Tools → CMake 64-bit

3. 记住安装路径（默认路径）：
   - Qt库：`C:\Qt\6.10.0\mingw_64` 或 `D:\Qt\6.10.0\mingw_64`
   - MinGW编译器：`C:\Qt\Tools\mingw1310_64` 或 `D:\Qt\Tools\mingw1310_64`
   - CMake：`C:\Qt\Tools\CMake_64` 或 `D:\Qt\Tools\CMake_64`

### 2. 验证安装

打开 **PowerShell** 或 **命令提示符**，执行以下命令：

```powershell
# 检查CMake版本
cmake --version

# 检查MinGW编译器版本（替换为你的实际路径）
D:\Qt\Tools\mingw1310_64\bin\g++ --version
```

如果显示版本号，说明安装成功！

---

## 🔨 构建项目

### 方法1：使用Qt Creator（推荐新手）

1. **打开项目**
   - 启动 Qt Creator
   - 点击 **File** → **Open File or Project**
   - 选择项目根目录下的 `CMakeLists.txt`

2. **配置编译套件（Kit）**
   - 选择 **Desktop Qt 6.10.0 MinGW 64-bit**
   - 如果没有显示，点击 **Configure Project** → **Qt Versions** → 添加Qt路径

3. **构建项目**
   - 点击左下角"锤子"图标（🔨 Build）
   - 或按快捷键 `Ctrl + B`

4. **运行程序**
   - 点击左下角"播放"按钮（▶️ Run）
   - 或按快捷键 `Ctrl + R`

---

### 方法2：命令行构建（适合高级用户）

#### **步骤1：配置环境变量（可选）**

如果你的Qt和MinGW不在系统PATH中，需要先设置环境变量：

```powershell
# 设置Qt路径（替换为你的实际路径）
$env:Qt6_DIR = "D:\Qt\6.10.0\mingw_64"

# 添加MinGW到PATH
$env:Path = "D:\Qt\Tools\mingw1310_64\bin;$env:Path"

# 添加CMake到PATH
$env:Path = "D:\Qt\Tools\CMake_64\bin;$env:Path"

# 添加Ninja到PATH
$env:Path = "D:\Qt\Tools\Ninja;$env:Path"
```

#### **步骤2：配置和构建**

```powershell
# 1. 进入项目目录
cd C:\Users\你的用户名\Desktop\Group_program_CPP

# 2. 创建构建目录
New-Item -ItemType Directory -Path build -Force
cd build

# 3. 配置CMake（替换为你的Qt路径）
cmake -G "Ninja" `
  -DCMAKE_BUILD_TYPE=Debug `
  -DCMAKE_PREFIX_PATH=D:/Qt/6.10.0/mingw_64 `
  -DCMAKE_EXPORT_COMPILE_COMMANDS=ON `
  ..

# 4. 编译项目
cmake --build . --config Debug

# 5. 运行客户端
.\bin\sport_evaluation_client.exe
```

**注意**：服务器需要在另一个终端窗口中运行：

```powershell
# 新开一个PowerShell窗口
cd C:\Users\你的用户名\Desktop\Group_program_CPP\build
.\bin\sport_evaluation_server.exe
```

---

## ▶️ 运行程序

### 启动顺序

1. **先启动服务器**：
   - 运行 `sport_evaluation_server.exe`
   - 服务器默认监听端口：`12345`
   - 看到提示 `服务器已启动，监听端口 12345` 即表示成功

2. **再启动客户端**：
   - 运行 `sport_evaluation_client.exe`
   - 客户端会自动连接到本地服务器 `127.0.0.1:12345`

### 功能说明

#### **主窗口功能**
- 输入用户信息（姓名、年龄、身高、体重等）
- 计算运动GPA
- 打开训练计划管理窗口

#### **训练计划窗口功能**
- 📅 日历选择训练日期
- ➕ 添加/删除训练计划
- 🏋️ 管理训练动作（组数、重量、次数、休息时间）
- 💾 自动保存到服务器

---

## 📁 项目结构

### 目录说明

```
Group_program_CPP/
├── CMakeLists.txt          # CMake构建配置文件
├── README.md               # 项目说明文档
├── PROJECT_STRUCTURE.md    # 详细的文件组织说明
├── deploy.ps1              # 部署脚本
│
├── include/                # 头文件目录
│   ├── ui/                 # UI相关头文件
│   │   ├── mainwindow.h    # 主窗口类声明
│   │   ├── trainwindow.h   # 训练窗口类声明
│   │   └── userdata.h      # 用户数据类声明
│   ├── client/             # 客户端头文件
│   │   └── clientconnection.h
│   ├── server/             # 服务器头文件
│   │   ├── server.h
│   │   └── clienthandler.h
│   ├── analyseuser.h       # 用户分析类（GPA计算）
│   └── train.h             # 训练计划数据结构
│
├── src/                    # 源文件目录
│   ├── ui/                 # UI实现文件
│   │   ├── mainwindow.cpp
│   │   ├── trainwindow.cpp
│   │   └── userdata.cpp
│   ├── client/             # 客户端实现文件
│   │   ├── main.cpp        # 客户端程序入口
│   │   └── clientconnection.cpp
│   ├── server/             # 服务器实现文件
│   │   ├── main.cpp        # 服务器程序入口
│   │   ├── server.cpp
│   │   └── clienthandler.cpp
│   ├── forms/              # Qt Designer UI文件
│   │   ├── mainwindow.ui
│   │   └── trainwindow.ui
│   ├── analyse.cpp         # GPA计算实现
│   └── train.cpp           # 训练计划功能实现
│
├── resources/              # 资源文件
│   └── resources.qrc       # Qt资源文件
│
├── build/                  # 构建输出目录（自动生成）
│   └── bin/                # 可执行文件目录
│
└── bin/                    # 部署文件（DLL等）
    ├── platforms/
    ├── styles/
    └── translations/
```

### 编译目标说明

项目包含3个编译目标：

1. **sport_evaluation_client** - 客户端程序
   - 使用文件：`src/ui/*`, `src/client/*`, `src/forms/*`
   - 输出：`build/bin/sport_evaluation_client.exe`

2. **sport_evaluation_server** - 服务器程序
   - 使用文件：`src/server/*`
   - 输出：`build/bin/sport_evaluation_server.exe`

3. **sport_evaluation_common** - 共享库
   - 使用文件：`src/analyse.cpp`, `src/ui/userdata.cpp`, `src/train.cpp`
   - 被客户端和服务器链接使用

> 📘 **详细的文件组织说明**：请查看 [PROJECT_STRUCTURE.md](PROJECT_STRUCTURE.md)

---

## ❓ 常见问题

### 1. 找不到Qt库或编译器

**问题**：CMake报错 `Could not find Qt6` 或编译器未找到

**解决方法**：
- 检查Qt安装路径是否正确
- 确保 `CMAKE_PREFIX_PATH` 指向正确的Qt目录（如 `D:/Qt/6.10.0/mingw_64`）
- 检查MinGW是否在PATH中或使用绝对路径

### 2. 缺少DLL文件

**问题**：运行时提示缺少 `Qt6Core.dll` 等文件

**解决方法**：
- 方法1：使用Qt Creator运行（会自动处理DLL）
- 方法2：将Qt的bin目录添加到系统PATH：
  ```powershell
  $env:Path = "D:\Qt\6.10.0\mingw_64\bin;$env:Path"
  ```
- 方法3：运行部署脚本：
  ```powershell
  .\deploy.ps1
  ```

### 3. IntelliSense找不到Qt头文件（VSCode用户）

**问题**：VSCode中Qt头文件标红

**解决方法**：
1. 确保CMake已生成 `compile_commands.json`（已配置）
2. 检查 `.vscode/c_cpp_properties.json` 中的Qt路径是否正确
3. 重新加载VSCode窗口：`Ctrl+Shift+P` → `Reload Window`

### 4. 编译错误：C++标准不支持

**问题**：MSVC编译器报 `__cplusplus` 版本错误

**解决方法**：
- 已在 `CMakeLists.txt` 中添加 `/Zc:__cplusplus` 标志（MSVC）
- 如果使用MinGW，应该不会有此问题

### 5. 训练动作显示不出来

**问题**：训练计划窗口中动作列表为空

**可能原因**：
- 服务器未启动或未连接
- 数据未正确保存
- 用户对象为空（`currentUser` 未初始化）

**解决方法**：
- 确保服务器已启动
- 检查网络连接状态
- 确保在主窗口中先输入用户信息再打开训练窗口

### 6. 中文乱码问题

**问题**：界面或输出中中文显示为乱码

**解决方法**：
- 已在 `CMakeLists.txt` 中添加 `/utf-8` 编译标志（MSVC）
- 确保源文件保存为UTF-8编码
- 检查Qt界面是否正确设置中文

---

## 📝 开发日志

### 2025年11月14日
- 🐛 修复 `train.cpp` 第66行编译错误（变量名和成员访问错误）
- 📖 统一项目文档（README.md + PROJECT_STRUCTURE.md）

### 2025年11月12日
- 🔧 配置VSCode IntelliSense支持Qt头文件
- 🔧 添加 `CMAKE_EXPORT_COMPILE_COMMANDS` 用于生成 `compile_commands.json`
- 🗑️ 删除重复的 `src/client/mainwindow.*` 文件
- 📝 创建 `PROJECT_STRUCTURE.md` 说明文件组织

### 2025年11月11日
- ✨ 实现完整的训练计划管理系统
  - 新增 `train.h` 和 `train.cpp`（包含 set, actions, train, date 四个类）
  - 新增 `TrainWindow` UI界面（日历、训练计划列表、动作管理）
  - 集成服务器端训练数据处理逻辑
- 🐛 修复Qt命名冲突：`QWidget::actions` vs 自定义 `actions` 类
- 🐛 修复MSVC编译器C++17标准识别问题（添加 `/Zc:__cplusplus` 标志）
- 🐛 解决主窗口文件路径混淆（`src/client` vs `src/ui`）
- 🐛 修复训练计划窗口无法打开问题（`currentUser` 对象未初始化）

### 2025年11月10日
- 📖 为所有头文件添加详细的中文新手教学注释
- 📚 覆盖C++基础知识点（类、继承、指针、信号槽等）

### 2025年10月30日 - 2025年11月5日
- 🎉 项目初始化
- 🏗️ 搭建基本的客户端-服务器架构
- 💻 实现用户信息输入和GPA计算功能
- 🌐 实现TCP/IP网络通信模块

---
## 2025年11月14日
## 👥 贡献者

- **陈霆钧** - 项目开发者

---

## 📄 许可证

本项目仅用于学习和研究目的。

---

## 🆘 获取帮助

如果遇到问题：
1. 查看 [常见问题](#常见问题) 章节
2. 查看 [PROJECT_STRUCTURE.md](PROJECT_STRUCTURE.md) 了解文件组织
3. 检查开发日志中是否有相关修复记录
4. 查看Qt官方文档：https://doc.qt.io/qt-6/

---

**最后更新**：2025年11月14日
