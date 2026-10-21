#ifndef SERVER_H  // 防止头文件重复包含的宏定义保护
#define SERVER_H

// 包含必要的Qt网络编程相关头文件
#include <QTcpServer>  // Qt的TCP服务器类，提供网络服务器功能
#include <QList>       // Qt的列表容器，用于存储连接的客户端
#include "clienthandler.h"  // 自定义的客户端处理类

// Server类：服务器核心类，负责监听和管理客户端连接
// 继承自QTcpServer，提供TCP服务器功能
class Server : public QTcpServer
{
    Q_OBJECT  // Qt元对象宏，启用信号槽机制

public:
    // 构造函数：创建服务器实例
    // @param parent: 父对象指针，用于Qt的对象树管理
    explicit Server(QObject *parent = nullptr);
    
    // 析构函数：清理资源，断开所有客户端连接
    ~Server();

    // 启动服务器
    // @param port: 服务器监听的端口号，默认为12345
    // @return: 是否成功启动
    bool start(quint16 port = 12345);

// 信号声明：用于通知外部对象服务器的各种事件
signals:
    // 当新客户端连接时发出此信号
    // @param address: 客户端的IP地址
    void clientConnected(const QString &address);
    
    // 当客户端断开连接时发出此信号
    void clientDisconnected(const QString &address);
    
    // 当发生错误时发出此信号
    void error(const QString &error);

protected:
    // 重写QTcpServer的虚函数，处理新的客户端连接
    // @param socketDescriptor: 新连接的套接字描述符
    void incomingConnection(qintptr socketDescriptor) override;

private:
    // 存储所有已连接的客户端处理器
    // QList是Qt提供的动态数组容器，类似于std::vector
    QList<ClientHandler*> clients;
};

#endif // SERVER_H