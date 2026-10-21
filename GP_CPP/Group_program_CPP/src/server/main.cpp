// =====================================================
// 文件名: server/main.cpp
// 功能: 数据存储服务器主程序
// 说明: 启动TCP服务器用于处理用户数据和训练数据的存储
// =====================================================

#include <QCoreApplication>  // Qt核心应用程序类（控制台应用）
#include "server.h"          // 服务器类
#include <QDebug>            // Qt调试输出

// =====================================================
// 主函数：服务器程序入口
// =====================================================
// @param argc: 命令行参数数量
// @param argv: 命令行参数数组
// @return: 程序退出码
int main(int argc, char *argv[])
{
    // 创建Qt核心应用程序对象
    // QCoreApplication用于控制台应用（无GUI）
    QCoreApplication app(argc, argv);

    // 创建服务器对象
    Server server;
    
    // ===== 连接服务器信号到Lambda槽函数 =====
    
    // 当有客户端连接时，打印日志
    QObject::connect(&server, &Server::clientConnected, 
        [](const QString &address) { 
            qDebug() << "客户端已连接:" << address; 
        });
    
    // 当客户端断开连接时，打印日志
    QObject::connect(&server, &Server::clientDisconnected,
        [](const QString &address) { 
            qDebug() << "客户端已断开:" << address; 
        });
    
    // 当服务器发生错误时，打印错误信息
    QObject::connect(&server, &Server::error,
        [](const QString &error) { 
            qDebug() << "服务器错误:" << error; 
        });

    // ===== 启动服务器 =====
    
    // 尝试在端口12345上启动服务器
    if (!server.start(12345)) {
        qDebug() << "服务器启动失败";
        return 1;  // 返回错误码1
    }

    qDebug() << "服务器已启动，按Ctrl+C退出";
    
    // 进入事件循环，保持程序运行
    return app.exec();
}