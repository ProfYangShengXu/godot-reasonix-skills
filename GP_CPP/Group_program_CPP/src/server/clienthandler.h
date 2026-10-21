#ifndef CLIENTHANDLER_H  // 防止头文件重复包含
#define CLIENTHANDLER_H

// 引入需要的Qt库
#include <QObject>      // Qt基础对象类
#include <QTcpSocket>   // TCP套接字，用于网络通信
#include <QJsonObject>  // JSON对象处理
#include <QJsonArray>   // JSON数组处理
#include "ui/userdata.h"    // 用户数据结构
#include "train.h"      // 训练计划结构
#include "analyseuser.h" // 用户分析类

// 客户端处理类：负责处理每个连接到服务器的客户端
class ClientHandler : public QObject
{
    Q_OBJECT  // Qt的元对象宏，支持信号槽机制

public:
    // 构造函数：创建新的客户端处理器
    // socketDescriptor: 系统分配的套接字描述符
    explicit ClientHandler(qintptr socketDescriptor, QObject *parent = nullptr);
    
    // 析构函数：清理资源
    ~ClientHandler();

    // 获取客户端的IP地址
    QString peerAddress() const;

signals:  // 信号声明部分
    void disconnected();  // 客户端断开连接时发出的信号
    void error(const QString &error);  // 发生错误时发出的信号

private slots:  // 私有槽函数
    void handleReadyRead();     // 处理收到数据的事件
    void handleDisconnected();  // 处理客户端断开连接的事件

private:
    QTcpSocket *socket;  // 处理与客户端通信的套接字

    // 处理接收到的用户体测数据
    void processUserData(const QJsonObject &json);
    
    // 处理接收到的训练计划数据
    void processTrainData(const QJsonObject &json);
    
    // 处理保存用户数据请求
    void handleSaveUserData(const QJsonObject &json);
    
    // 处理加载用户数据请求
    void handleLoadUserData(const QJsonObject &json);
    
    // 处理保存训练数据请求
    void handleSaveTrainData(const QJsonObject &json);
    
    // 处理加载训练数据请求
    void handleLoadTrainData(const QJsonObject &json);
    
    // 发送评分结果给客户端
    void sendEvaluation(double gpa);
    
    // 发送响应消息
    void sendResponse(const QJsonObject &response);
    
    // 从文件加载用户数据
    users* loadUserFromFile(const QString &userId);
    
    // 保存用户数据到文件
    void saveUserToFile(const QString &userId, users* user);
    
    // 从文件加载训练计划
    train* loadTrainFromFile(const QString &userId);
    
    // 保存训练计划到文件
    void saveTrainToFile(const QString &userId, const train &trainPlan);
    
    // 获取服务器数据存储路径
    QString getDataPath() const;
};

#endif // CLIENTHANDLER_H