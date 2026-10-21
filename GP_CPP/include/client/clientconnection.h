#ifndef CLIENTCONNECTION_H  // 防止头文件重复包含的宏定义保护
#define CLIENTCONNECTION_H  // 如果没有定义这个宏，就定义它

// Qt框架的必要头文件引入
#include <QObject>       // Qt基础对象类，提供信号槽机制的基类
#include <QTcpSocket>    // Qt的TCP套接字类，用于网络通信
#include <QJsonDocument> // Qt的JSON文档处理类，用于解析JSON数据
#include <QJsonObject>   // Qt的JSON对象类，用于处理JSON数据
#include "ui/userdata.h" // 自定义的用户数据类
#include "analyseuser.h" // 用户分析类
#include "train.h"       // 训练计划类

// ClientConnection类：负责客户端与服务器之间的网络通信
// 继承自QObject以使用Qt的信号槽机制
class ClientConnection : public QObject
{
    // Q_OBJECT宏：Qt元对象系统的一部分，启用信号槽机制
    // 没有这个宏的类将无法使用信号槽！
    Q_OBJECT

public:
    // 构造函数：创建客户端连接对象
    // explicit关键字防止隐式转换
    // parent参数用于Qt的对象树管理（自动内存管理）
    explicit ClientConnection(QObject *parent = nullptr);
    
    // 析构函数：清理资源
    ~ClientConnection();

    // 公共方法：连接到服务器
    // host：服务器地址（如"127.0.0.1"）
    // port：服务器端口（如8080）
    // 返回值：连接是否成功
    bool connectToServer(const QString &host, quint16 port);
    
    // 公共方法：断开与服务器的连接
    void disconnectFromServer();
    
    // 公共方法：发送用户数据到服务器
    void sendUserData(const UserData &userData);
    
    // 保存用户数据到服务器
    void saveUserData(const QString &userId, users* user);
    
    // 从服务器加载用户数据
    void loadUserData(const QString &userId);
    
    // 保存训练数据到服务器
    void saveTrainData(const QString &userId, const train &trainPlan);
    
    // 从服务器加载训练数据
    void loadTrainData(const QString &userId);

// signals关键字：声明信号
// 信号是Qt特有的机制，用于在对象之间传递消息
// 信号没有函数体，只有声明
signals:
    void connected();    // 信号：连接成功时发出
    void disconnected(); // 信号：断开连接时发出
    void error(const QString &errorString);  // 信号：发生错误时发出
    void evaluationReceived(double gpa);     // 信号：收到成绩评估结果时发出
    
    // 新增信号
    void userDataSaved(bool success, const QString &message);
    void userDataLoaded(bool success, users* user);
    void trainDataSaved(bool success, const QString &message);
    void trainDataLoaded(bool success, train* trainPlan);

// private slots：私有槽函数
// 槽函数是用来响应信号的特殊函数
private slots:
    void handleConnected();    // 处理连接成功事件
    void handleDisconnected(); // 处理断开连接事件
    void handleError(QAbstractSocket::SocketError socketError); // 处理Socket错误
    void handleReadyRead();    // 处理收到新数据事件

private:
    QTcpSocket *socket; // TCP套接字对象，用于网络通信
    
    // 发送JSON消息到服务器
    void sendJsonMessage(const QJsonObject &json);
    
    // 处理服务器响应
    void handleResponse(const QJsonObject &json);
};

#endif // CLIENTCONNECTION_H