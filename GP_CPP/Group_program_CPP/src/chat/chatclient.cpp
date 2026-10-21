// =====================================================
// 文件名: chatclient.cpp
// 功能: 聊天客户端的实现
// 说明: 负责连接到聊天服务器、发送和接收消息
// =====================================================

#include "chat/chatclient.h"  // 聊天客户端头文件
#include <QDebug>             // Qt调试输出

// =====================================================
// 构造函数：初始化聊天客户端
// =====================================================
// @param parent: 父对象指针
ChatClient::ChatClient(QObject *parent)
    : QObject(parent)                  // 初始化基类
    , socket(new QTcpSocket(this))     // 创建TCP套接字对象
{
    // ===== 连接套接字的信号到槽函数 =====
    
    // 当连接建立成功时触发
    connect(socket, &QTcpSocket::connected, this, &ChatClient::onConnected);
    
    // 当连接断开时触发
    connect(socket, &QTcpSocket::disconnected, this, &ChatClient::onDisconnected);
    
    // 当有新数据可读时触发
    connect(socket, &QTcpSocket::readyRead, this, &ChatClient::onReadyRead);
    
    // 当发生错误时触发
    connect(socket, &QTcpSocket::errorOccurred, this, &ChatClient::onError);
}

// =====================================================
// 析构函数：清理资源
// =====================================================
ChatClient::~ChatClient()
{
    disconnectFromServer();  // 断开连接
}

// =====================================================
// 连接到聊天服务器
// =====================================================
// 功能：尝试连接到指定的聊天服务器
// @param host: 服务器IP地址或域名
// @param port: 服务器端口号
// @return: true表示连接成功，false表示失败
bool ChatClient::connectToServer(const QString &host, quint16 port)
{
    // 检查是否已经连接
    if (socket->state() == QAbstractSocket::ConnectedState) {
        return true;  // 已经连接，直接返回成功
    }
    
    // 记录连接尝试
    qDebug() << "正在连接聊天服务器:" << host << ":" << port;
    
    // 发起连接请求
    socket->connectToHost(host, port);
    
    // 等待连接完成（最多等待3秒）
    // waitForConnected会阻塞直到连接成功或超时
    return socket->waitForConnected(3000);
}

// =====================================================
// 断开与服务器的连接
// =====================================================
// 功能：主动断开与聊天服务器的连接
void ChatClient::disconnectFromServer()
{
    // 检查是否处于连接状态
    if (socket->state() == QAbstractSocket::ConnectedState) {
        socket->disconnectFromHost();  // 断开连接
    }
}

// =====================================================
// 发送消息到服务器
// =====================================================
// 功能：向聊天服务器发送文本消息
// @param message: 要发送的消息内容
void ChatClient::sendMessage(const QString &message)
{
    // 检查连接状态
    if (socket->state() != QAbstractSocket::ConnectedState) {
        // 未连接，发出错误信号
        emit error("未连接到服务器");
        return;
    }
    
    // 将消息转换为UTF-8字节数组，并添加换行符作为消息分隔符
    QByteArray data = message.toUtf8() + "\n";
    
    // 写入数据到套接字
    socket->write(data);
    
    // 立即刷新发送缓冲区，确保数据被发送
    socket->flush();
}

// =====================================================
// 检查是否已连接
// =====================================================
// 功能：查询当前是否已连接到服务器
// @return: true表示已连接，false表示未连接
bool ChatClient::isConnected() const
{
    return socket->state() == QAbstractSocket::ConnectedState;
}

// =====================================================
// 连接成功的槽函数
// =====================================================
// 功能：处理套接字连接成功的信号
void ChatClient::onConnected()
{
    qDebug() << "已连接到聊天服务器";
    emit connected();  // 发出连接成功的信号
}

// =====================================================
// 断开连接的槽函数
// =====================================================
// 功能：处理套接字断开连接的信号
void ChatClient::onDisconnected()
{
    qDebug() << "与聊天服务器断开连接";
    emit disconnected();  // 发出断开连接的信号
}

// =====================================================
// 接收数据的槽函数
// =====================================================
// 功能：处理从服务器接收到的数据
void ChatClient::onReadyRead()
{
    // 循环读取所有可用的行数据
    // canReadLine()检查缓冲区是否包含完整的一行（以\n结尾）
    while (socket->canReadLine()) {
        // 读取一行数据
        QByteArray data = socket->readLine();
        
        // 将字节数组转换为QString并去除首尾空白字符
        QString message = QString::fromUtf8(data).trimmed();
        
        // 如果消息不为空，发出信号
        if (!message.isEmpty()) {
            emit messageReceived(message);
        }
    }
}

// =====================================================
// 错误处理的槽函数
// =====================================================
// 功能：处理套接字发生的错误
// @param socketError: 错误类型枚举值
void ChatClient::onError(QAbstractSocket::SocketError socketError)
{
    Q_UNUSED(socketError);  // 标记参数未使用（避免编译警告）
    
    // 获取错误描述字符串
    QString errorString = socket->errorString();
    
    // 记录错误信息
    qDebug() << "聊天客户端错误:" << errorString;
    
    // 发出错误信号
    emit error(errorString);
}
