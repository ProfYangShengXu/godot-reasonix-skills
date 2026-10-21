// =====================================================
// 文件名: chatwindow.cpp
// 功能: 聊天室窗口的实现
// 说明: 提供图形化的聊天界面，支持连接服务器和收发消息
// =====================================================

#include "ui/chatwindow.h"    // 聊天窗口头文件
#include "ui_chatwindow.h"    // UI自动生成的头文件
#include <QMessageBox>        // 消息框
#include <QDateTime>          // 日期时间

// =====================================================
// 构造函数：初始化聊天窗口
// =====================================================
// @param parent: 父窗口指针
ChatWindow::ChatWindow(QWidget *parent)
    : QMainWindow(parent)                  // 初始化基类主窗口
    , ui(new Ui::ChatWindow)               // 创建UI对象
    , chatClient(new ChatClient(this))     // 创建聊天客户端对象
{
    // 设置UI界面
    ui->setupUi(this);
    
    // 设置窗口标题
    setWindowTitle("运动评估系统 - 聊天室");
    
    // ===== 连接聊天客户端的信号到窗口的槽函数 =====
    
    // 当连接成功时调用onConnected
    connect(chatClient, &ChatClient::connected, this, &ChatWindow::onConnected);
    
    // 当断开连接时调用onDisconnected
    connect(chatClient, &ChatClient::disconnected, this, &ChatWindow::onDisconnected);
    
    // 当收到新消息时调用onMessageReceived
    connect(chatClient, &ChatClient::messageReceived, this, &ChatWindow::onMessageReceived);
    
    // 当发生错误时调用onError
    connect(chatClient, &ChatClient::error, this, &ChatWindow::onError);
    
    // 设置初始状态为未连接
    setConnectedState(false);
    
    // ===== 设置默认值 =====
    
    // 默认服务器地址为本机
    ui->lineHost->setText("127.0.0.1");
    
    // 默认聊天服务器端口为12346
    ui->spinPort->setValue(12346);
    
    // 生成随机用户名（使用当前时间戳的后三位）
    ui->lineUsername->setText("用户" + QString::number(QDateTime::currentMSecsSinceEpoch() % 1000));
}

// =====================================================
// 析构函数：清理资源
// =====================================================
ChatWindow::~ChatWindow()
{
    delete ui;  // 删除UI对象
}

// =====================================================
// 连接/断开按钮点击事件
// =====================================================
// 功能：根据当前连接状态决定是连接还是断开服务器
void ChatWindow::on_btnConnect_clicked()
{
    // 检查当前是否已连接
    if (chatClient->isConnected()) {
        // ===== 已连接，执行断开操作 =====
        
        // 断开与服务器的连接
        chatClient->disconnectFromServer();
        
        // 更新UI状态为未连接
        setConnectedState(false);
        
        // 在聊天框显示系统消息
        appendMessage("系统: 已断开连接");
    } else {
        // ===== 未连接，执行连接操作 =====
        
        // 获取并验证用户名
        username = ui->lineUsername->text().trimmed();  // 去除首尾空格
        if (username.isEmpty()) {
            // 用户名为空，显示警告
            QMessageBox::warning(this, "警告", "请输入用户名");
            return;
        }
        
        // 获取服务器地址和端口
        QString host = ui->lineHost->text();
        quint16 port = ui->spinPort->value();
        
        // 尝试连接到服务器
        if (chatClient->connectToServer(host, port)) {
            // 连接成功，更新UI状态
            setConnectedState(true);
            
            // 显示连接成功消息
            appendMessage("系统: 已连接到服务器 " + host + ":" + QString::number(port));
            
            // 向聊天室发送进入消息
            chatClient->sendMessage(username + " 进入了聊天室");
        } else {
            // 连接失败，显示错误消息
            QMessageBox::warning(this, "错误", "无法连接到服务器");
        }
    }
}

// =====================================================
// 发送按钮点击事件
// =====================================================
// 功能：发送消息到聊天服务器
void ChatWindow::on_btnSend_clicked()
{
    // 获取输入框中的消息并去除首尾空格
    QString message = ui->lineMessage->text().trimmed();
    
    // 检查消息是否为空
    if (message.isEmpty()) {
        return;  // 空消息不发送
    }
    
    // 检查是否已连接到服务器
    if (!chatClient->isConnected()) {
        QMessageBox::warning(this, "警告", "未连接到服务器");
        return;
    }
    
    // ===== 发送消息 =====
    
    // 构造完整消息：用户名 + 消息内容
    QString fullMessage = username + ": " + message;
    
    // 通过客户端发送消息
    chatClient->sendMessage(fullMessage);
    
    // ===== 在自己的聊天框显示发送的消息 =====
    
    // 获取当前时间戳
    QString timestamp = QDateTime::currentDateTime().toString("[HH:mm:ss]");
    
    // 在聊天框显示（显示为"我"而不是用户名）
    appendMessage(timestamp + " 我: " + message);
    
    // 清空输入框，准备输入下一条消息
    ui->lineMessage->clear();
}

// =====================================================
// 消息输入框回车事件
// =====================================================
// 功能：按回车键等同于点击发送按钮
void ChatWindow::on_lineMessage_returnPressed()
{
    on_btnSend_clicked();  // 调用发送按钮的处理函数
}

// =====================================================
// 连接成功的槽函数
// =====================================================
// 功能：处理客户端连接成功的信号
void ChatWindow::onConnected()
{
    setConnectedState(true);  // 更新UI为已连接状态
}

// =====================================================
// 断开连接的槽函数
// =====================================================
// 功能：处理客户端断开连接的信号
void ChatWindow::onDisconnected()
{
    setConnectedState(false);                  // 更新UI为未连接状态
    appendMessage("系统: 连接已断开");           // 显示系统消息
}

// =====================================================
// 接收消息的槽函数
// =====================================================
// 功能：处理从服务器接收到的消息
// @param message: 服务器转发的消息内容
void ChatWindow::onMessageReceived(const QString &message)
{
    appendMessage(message);  // 将消息追加到聊天框
}

// =====================================================
// 错误处理的槽函数
// =====================================================
// 功能：处理客户端发生的错误
// @param errorString: 错误描述信息
void ChatWindow::onError(const QString &errorString)
{
    appendMessage("错误: " + errorString);  // 在聊天框显示错误信息
}

// =====================================================
// 追加消息到聊天框
// =====================================================
// 功能：在聊天文本框中显示消息
// @param message: 要显示的消息内容
void ChatWindow::appendMessage(const QString &message)
{
    ui->textChat->append(message);  // 在文本框末尾追加消息
}

// =====================================================
// 设置连接状态
// =====================================================
// 功能：根据连接状态启用/禁用相应的UI控件
// @param connected: true表示已连接，false表示未连接
void ChatWindow::setConnectedState(bool connected)
{
    // ===== 根据连接状态设置控件的启用状态 =====
    
    // 未连接时可以修改服务器设置
    ui->lineHost->setEnabled(!connected);
    ui->spinPort->setEnabled(!connected);
    ui->lineUsername->setEnabled(!connected);
    
    // 已连接时才能发送消息
    ui->lineMessage->setEnabled(connected);
    ui->btnSend->setEnabled(connected);
    
    // 更改按钮文本
    if (connected) {
        ui->btnConnect->setText("断开连接");  // 已连接时显示"断开连接"
    } else {
        ui->btnConnect->setText("连接");      // 未连接时显示"连接"
    }
}
