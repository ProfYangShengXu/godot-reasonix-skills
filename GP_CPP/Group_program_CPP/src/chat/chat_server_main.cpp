// =====================================================
// 文件名: chat/chat_server_main.cpp
// 功能: 聊天服务器主程序
// 说明: 启动独立的聊天服务器，管理聊天室连接和消息广播
// =====================================================

#include "chat/chatserver.h"   // 聊天服务器类
#include <QCoreApplication>    // Qt核心应用程序类
#include <QDebug>              // Qt调试输出

// =====================================================
// 主函数：聊天服务器程序入口
// =====================================================
// @param argc: 命令行参数数量
// @param argv: 命令行参数数组
// @return: 程序退出码
int main(int argc, char *argv[])
{
    // 创建Qt核心应用程序对象
    // 这是控制台应用程序，不需要GUI
    QCoreApplication app(argc, argv);
    
    // 创建聊天服务器对象
    ChatServer server;
    
    // ===== 连接服务器信号到Lambda槽函数 =====
    
    // 当新客户端连接时，打印连接信息
    QObject::connect(&server, &ChatServer::clientConnected, 
        [](const QString &address) {
            qDebug() << "客户端连接:" << address;
        });
    
    // 当客户端断开连接时，打印断开信息
    QObject::connect(&server, &ChatServer::clientDisconnected, 
        [](const QString &address) {
            qDebug() << "客户端断开:" << address;
        });
    
    // 当收到消息时，打印消息内容
    QObject::connect(&server, &ChatServer::messageReceived, 
        [](const QString &message) {
            qDebug() << message;
        });
    
    // ===== 启动聊天服务器 =====
    
    // 尝试在端口12346上启动服务器
    if (server.start(12346)) {
        qDebug() << "聊天服务器启动成功，端口: 12346";
        qDebug() << "按 Ctrl+C 停止服务器";
        
        // 进入事件循环，保持程序运行
        return app.exec();
    } else {
        // 启动失败
        qDebug() << "聊天服务器启动失败";
        return 1;  // 返回错误码1
    }
}
