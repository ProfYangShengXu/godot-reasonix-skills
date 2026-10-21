// =====================================================
// 文件名: chatserver.cpp
// 功能: 聊天服务器的实现
// 说明: 管理多个客户端连接，实现消息广播功能
// =====================================================

#include "chat/chatserver.h"  // 聊天服务器头文件
#include <QDebug>             // Qt调试输出

// =====================================================
// 构造函数：初始化聊天服务器
// =====================================================
// @param parent: 父对象指针
ChatServer::ChatServer(QObject *parent)
    : QObject(parent)                  // 初始化基类
    , server(new QTcpServer(this))     // 创建TCP服务器对象
{
    // 连接新连接信号到槽函数
    // 当有新客户端连接时，会触发onNewConnection
    connect(server, &QTcpServer::newConnection, this, &ChatServer::onNewConnection);
}

// =====================================================
// 析构函数：清理资源
// =====================================================
ChatServer::~ChatServer()
{
    stop();  // 停止服务器并断开所有客户端
}

// =====================================================
// 启动聊天服务器
// =====================================================
// 功能：在指定端口启动服务器并开始监听连接
// @param port: 监听端口号
// @return: true表示启动成功，false表示失败
bool ChatServer::start(quint16 port)
{
    // 尝试在所有网络接口上监听指定端口
    // QHostAddress::Any 表示监听所有可用的IP地址
    if (server->listen(QHostAddress::Any, port)) {
        qDebug() << "聊天服务器已启动，监听端口:" << port;
        return true;
    } else {
        // 启动失败，发出错误信号
        emit serverError(server->errorString());
        qDebug() << "启动聊天服务器失败:" << server->errorString();
        return false;
    }
}

// =====================================================
// 停止聊天服务器
// =====================================================
// 功能：断开所有客户端连接并关闭服务器
void ChatServer::stop()
{
    // 遍历所有已连接的客户端
    for (QTcpSocket *client : clients) {
        client->disconnectFromHost();  // 断开连接
        client->deleteLater();          // 延迟删除对象
    }
    
    // 清空客户端列表
    clients.clear();
    
    // 关闭服务器监听
    server->close();
    
    qDebug() << "聊天服务器已停止";
}

// =====================================================
// 获取当前连接的客户端数量
// =====================================================
// @return: 客户端数量
int ChatServer::getClientCount() const
{
    return clients.size();
}

// =====================================================
// 处理新客户端连接
// =====================================================
// 功能：当有新客户端连接时被调用
void ChatServer::onNewConnection()
{
    // 获取等待处理的新连接
    QTcpSocket *clientSocket = server->nextPendingConnection();
    
    // ===== 连接客户端套接字的信号 =====
    
    // 当客户端有数据可读时触发
    connect(clientSocket, &QTcpSocket::readyRead, this, &ChatServer::onClientReadyRead);
    
    // 当客户端断开连接时触发
    connect(clientSocket, &QTcpSocket::disconnected, this, &ChatServer::onClientDisconnected);
    
    // 将新客户端添加到列表
    clients.append(clientSocket);
    
    // 获取客户端IP地址
    QString clientAddress = clientSocket->peerAddress().toString();
    qDebug() << "新客户端连接:" << clientAddress << "总客户端数:" << clients.size();
    
    // 发出客户端已连接的信号
    emit clientConnected(clientAddress);
}

// =====================================================
// 处理客户端发送的消息
// =====================================================
// 功能：读取客户端发送的消息并广播给其他客户端
void ChatServer::onClientReadyRead()
{
    // 获取发送信号的客户端套接字
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket*>(sender());
    if (!clientSocket) return;  // 类型转换失败，返回
    
    // 读取所有可用数据
    QByteArray data = clientSocket->readAll();
    
    // 将数据转换为QString并去除首尾空白
    QString message = QString::fromUtf8(data).trimmed();
    
    // 忽略空消息
    if (message.isEmpty()) return;
    
    // ===== 添加时间戳 =====
    QString timestampedMessage = getTimeStamp() + " " + message;
    
    // 记录消息到控制台
    qDebug() << timestampedMessage;
    
    // 发出消息已接收的信号
    emit messageReceived(timestampedMessage);
    
    // ===== 广播消息给其他客户端 =====
    // sender参数用于排除发送者本身（避免回显）
    broadcastMessage(timestampedMessage, clientSocket);
}

// =====================================================
// 处理客户端断开连接
// =====================================================
// 功能：清理断开连接的客户端
void ChatServer::onClientDisconnected()
{
    // 获取断开连接的客户端套接字
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket*>(sender());
    if (!clientSocket) return;
    
    // 获取客户端地址（用于日志）
    QString clientAddress = clientSocket->peerAddress().toString();
    
    // 从客户端列表中移除
    clients.removeOne(clientSocket);
    
    // 延迟删除客户端对象
    clientSocket->deleteLater();
    
    qDebug() << "客户端断开连接:" << clientAddress << "剩余客户端数:" << clients.size();
    
    // 发出客户端已断开的信号
    emit clientDisconnected(clientAddress);
}

// =====================================================
// 广播消息给所有客户端
// =====================================================
// 功能：将消息发送给除发送者外的所有客户端
// @param message: 要广播的消息内容
// @param sender: 发送消息的客户端（不会收到此消息）
void ChatServer::broadcastMessage(const QString &message, QTcpSocket *sender)
{
    // 将消息转换为UTF-8字节数组，并添加换行符
    QByteArray data = message.toUtf8() + "\n";
    
    // 遍历所有客户端
    for (QTcpSocket *client : clients) {
        // 跳过发送者本身，并检查连接是否有效
        if (client != sender && client->isValid()) {
            client->write(data);   // 发送数据
            client->flush();       // 立即刷新缓冲区
        }
    }
}

// =====================================================
// 获取当前时间戳
// =====================================================
// 功能：生成格式化的时间戳字符串
// @return: 格式为 [yyyy-MM-dd HH:mm:ss] 的时间戳
QString ChatServer::getTimeStamp() const
{
    return QDateTime::currentDateTime().toString("[yyyy-MM-dd HH:mm:ss]");
}
