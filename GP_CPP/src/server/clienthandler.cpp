#include "clienthandler.h"    // 包含头文件
#include <QJsonDocument>      // JSON文档处理
#include <QJsonObject>        // JSON对象处理
#include <QJsonArray>         // JSON数组处理
#include <QDebug>            // 调试输出
#include <QFile>             // 文件操作
#include <QDir>              // 目录操作
#include <QStandardPaths>    // 标准路径

// 构造函数：初始化客户端处理器
ClientHandler::ClientHandler(qintptr socketDescriptor, QObject *parent)
    : QObject(parent)  // 初始化基类
    , socket(new QTcpSocket(this))  // 创建新的TCP套接字
{
    // 尝试设置套接字描述符
    if (!socket->setSocketDescriptor(socketDescriptor)) {
        // 如果设置失败，发出错误信号
        emit error(socket->errorString());
        return;
    }

    // 连接信号和槽
    // 当有新数据可读时，调用handleReadyRead函数
    connect(socket, &QTcpSocket::readyRead,
            this, &ClientHandler::handleReadyRead);
    // 当客户端断开连接时，调用handleDisconnected函数
    connect(socket, &QTcpSocket::disconnected,
            this, &ClientHandler::handleDisconnected);
    
    qDebug() << "客户端已连接:" << peerAddress();
}

// 析构函数：清理资源
ClientHandler::~ClientHandler()
{
    socket->disconnectFromHost();  // 断开与客户端的连接
    qDebug() << "客户端已断开:" << peerAddress();
}

// 获取客户端的IP地址
QString ClientHandler::peerAddress() const
{
    return socket->peerAddress().toString();  // 返回客户端IP地址的字符串形式
}

// 获取服务器数据存储路径
QString ClientHandler::getDataPath() const
{
    QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    path += "/server_data";
    
    // 确保目录存在
    QDir dir;
    if (!dir.exists(path)) {
        dir.mkpath(path);
    }
    
    return path;
}

// 处理新数据到达的事件
void ClientHandler::handleReadyRead()
{
    // 读取所有可用的数据
    QByteArray data = socket->readAll();
    
    // 尝试将收到的数据解析为JSON
    QJsonDocument doc = QJsonDocument::fromJson(data);
    
    // 检查是否是有效的JSON对象
    if (!doc.isObject()) {
        // 如果不是有效的JSON对象，发出错误信号
        qDebug() << "收到无效的JSON数据";
        emit error("收到无效的JSON数据");
        return;
    }

    // 获取JSON对象
    QJsonObject json = doc.object();
    
    // 检查消息类型
    QString messageType = json["type"].toString();
    qDebug() << "收到消息类型:" << messageType;
    
    if (messageType == "save_user_data") {
        handleSaveUserData(json);
    }
    else if (messageType == "load_user_data") {
        handleLoadUserData(json);
    }
    else if (messageType == "save_train_data") {
        handleSaveTrainData(json);
    }
    else if (messageType == "load_train_data") {
        handleLoadTrainData(json);
    }
    else if (messageType == "calculate_gpa") {
        processUserData(json);
    }
    else {
        qDebug() << "未知的消息类型:" << messageType;
        emit error("未知的消息类型");
    }
}

// 处理客户端断开连接的事件
void ClientHandler::handleDisconnected()
{
    emit disconnected();  // 发出断开连接信号
}

// 处理保存用户数据请求
void ClientHandler::handleSaveUserData(const QJsonObject &json)
{
    QString userId = json["userId"].toString();
    QJsonObject userData = json["userData"].toObject();
    
    qDebug() << "保存用户数据 - 用户ID:" << userId;
    
    // 构建文件路径
    QString filePath = getDataPath() + "/user_" + userId + ".json";
    
    // 保存到文件
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly)) {
        QJsonDocument doc(userData);
        file.write(doc.toJson());
        file.close();
        
        QJsonObject response;
        response["type"] = "save_user_data_response";
        response["status"] = "success";
        response["message"] = "用户数据已保存";
        sendResponse(response);
        
        qDebug() << "用户数据已保存到:" << filePath;
    } else {
        QJsonObject response;
        response["type"] = "save_user_data_response";
        response["status"] = "error";
        response["message"] = "无法保存用户数据";
        sendResponse(response);
        
        qDebug() << "无法保存用户数据到:" << filePath;
    }
}

// 处理加载用户数据请求
void ClientHandler::handleLoadUserData(const QJsonObject &json)
{
    QString userId = json["userId"].toString();
    
    qDebug() << "加载用户数据 - 用户ID:" << userId;
    
    // 构建文件路径
    QString filePath = getDataPath() + "/user_" + userId + ".json";
    
    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly)) {
        QByteArray data = file.readAll();
        file.close();
        
        QJsonDocument doc = QJsonDocument::fromJson(data);
        QJsonObject userData = doc.object();
        
        QJsonObject response;
        response["type"] = "load_user_data_response";
        response["status"] = "success";
        response["userData"] = userData;
        sendResponse(response);
        
        qDebug() << "用户数据已加载从:" << filePath;
    } else {
        QJsonObject response;
        response["type"] = "load_user_data_response";
        response["status"] = "error";
        response["message"] = "用户数据不存在";
        sendResponse(response);
        
        qDebug() << "用户数据文件不存在:" << filePath;
    }
}

// 处理保存训练数据请求
void ClientHandler::handleSaveTrainData(const QJsonObject &json)
{
    QString userId = json["userId"].toString();
    QJsonObject trainData = json["trainData"].toObject();
    
    qDebug() << "保存训练数据 - 用户ID:" << userId;
    
    // 构建文件路径
    QString filePath = getDataPath() + "/train_" + userId + ".json";
    
    // 保存到文件
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly)) {
        QJsonDocument doc(trainData);
        file.write(doc.toJson());
        file.close();
        
        QJsonObject response;
        response["type"] = "save_train_data_response";
        response["status"] = "success";
        response["message"] = "训练数据已保存";
        sendResponse(response);
        
        qDebug() << "训练数据已保存到:" << filePath;
    } else {
        QJsonObject response;
        response["type"] = "save_train_data_response";
        response["status"] = "error";
        response["message"] = "无法保存训练数据";
        sendResponse(response);
        
        qDebug() << "无法保存训练数据到:" << filePath;
    }
}

// 处理加载训练数据请求
void ClientHandler::handleLoadTrainData(const QJsonObject &json)
{
    QString userId = json["userId"].toString();
    
    qDebug() << "加载训练数据 - 用户ID:" << userId;
    
    // 构建文件路径
    QString filePath = getDataPath() + "/train_" + userId + ".json";
    
    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly)) {
        QByteArray data = file.readAll();
        file.close();
        
        QJsonDocument doc = QJsonDocument::fromJson(data);
        QJsonObject trainData = doc.object();
        
        QJsonObject response;
        response["type"] = "load_train_data_response";
        response["status"] = "success";
        response["trainData"] = trainData;
        sendResponse(response);
        
        qDebug() << "训练数据已加载从:" << filePath;
    } else {
        QJsonObject response;
        response["type"] = "load_train_data_response";
        response["status"] = "error";
        response["message"] = "训练数据不存在";
        sendResponse(response);
        
        qDebug() << "训练数据文件不存在:" << filePath;
    }
}

// 处理收到的用户数据 (用于计算GPA)
void ClientHandler::processUserData(const QJsonObject &json)
{
    // TODO: 从JSON数据创建UserData对象并计算GPA
    // 暂时返回一个测试用的固定GPA值
    qDebug() << "处理用户GPA计算请求";
    sendEvaluation(3.5);
}

// 发送评分结果给客户端
void ClientHandler::sendEvaluation(double gpa)
{
    // 创建响应JSON对象
    QJsonObject response;
    response["type"] = "evaluation";  // 设置消息类型
    response["gpa"] = gpa;  // 设置GPA值
    
    sendResponse(response);
    qDebug() << "发送GPA评估结果:" << gpa;
}

// 发送响应消息
void ClientHandler::sendResponse(const QJsonObject &response)
{
    // 将JSON对象转换为文档
    QJsonDocument doc(response);
    // 发送JSON数据给客户端
    socket->write(doc.toJson());
    socket->flush();
}