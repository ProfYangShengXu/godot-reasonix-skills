#include "server.h"    // 包含服务器类的头文件
#include <QDebug>      // Qt调试输出工具

// 构造函数：初始化TCP服务器
// @param parent: 父对象指针，用于Qt对象树管理
Server::Server(QObject *parent)
    : QTcpServer(parent)  // 初始化基类QTcpServer
{
    // 构造函数体为空，因为所有初始化都在初始化列表中完成
    // 这是C++的最佳实践，优先使用初始化列表而不是在构造函数体中赋值
}

// 析构函数：清理资源
Server::~Server()
{
    // qDeleteAll是Qt提供的便利函数，用于删除容器中的所有对象
    // 它会遍历容器，对每个元素调用delete
    qDeleteAll(clients);  // 删除所有客户端处理器对象
}

// 启动服务器
// @param port: 要监听的端口号
// @return: 服务器是否成功启动
bool Server::start(quint16 port)
{
    // QHostAddress::Any表示监听所有网络接口
    // 这样服务器可以接受来自任何IP地址的连接
    if (!listen(QHostAddress::Any, port)) {
        // 如果监听失败，发出错误信号
        // arg()是Qt字符串的格式化方法，类似于printf
        emit error(QString("无法启动服务器: %1").arg(errorString()));
        return false;
    }

    // 使用Qt的调试输出记录服务器启动信息
    qDebug() << "服务器已启动，正在监听端口" << port;
    return true;
}

// 处理新的客户端连接
// 这是QTcpServer的虚函数，当有新客户端连接时会被自动调用
// @param socketDescriptor: 新连接的套接字描述符
void Server::incomingConnection(qintptr socketDescriptor)
{
    // 创建新的客户端处理器对象
    // this作为父对象传递，确保正确的内存管理
    ClientHandler *client = new ClientHandler(socketDescriptor, this);
    
    // 将新客户端添加到列表中
    clients.append(client);

    // 使用Qt的新式信号槽语法连接断开连接的信号
    // lambda函数用作槽，处理客户端断开连接的情况
    connect(client, &ClientHandler::disconnected, [this, client]() {
        clients.removeOne(client);  // 从列表中移除客户端
        emit clientDisconnected(client->peerAddress());  // 发出客户端断开连接的信号
        client->deleteLater();  // 安排在事件循环的下一次迭代中删除客户端对象
    });

    // 发出客户端连接成功的信号
    emit clientConnected(client->peerAddress());
}