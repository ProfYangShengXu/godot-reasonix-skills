#include "client/clientconnection.h"  // 包含头文件
#include <QDataStream>         // 用于数据流操作的Qt类
#include <QJsonArray>          // JSON数组处理
#include <QDebug>              // 调试输出

// 构造函数：初始化客户端连接
ClientConnection::ClientConnection(QObject *parent)
    : QObject(parent)  // 调用父类构造函数
    , socket(new QTcpSocket(this))  // 创建新的TCP套接字
{
    // 连接各种网络事件到对应的处理函数
    // 当连接建立时
    connect(socket, &QTcpSocket::connected, this, &ClientConnection::handleConnected);
    // 当连接断开时
    connect(socket, &QTcpSocket::disconnected, this, &ClientConnection::handleDisconnected);
    // 当发生错误时
    connect(socket, &QTcpSocket::errorOccurred, this, &ClientConnection::handleError);
    // 当有新数据可读时
    connect(socket, &QTcpSocket::readyRead, this, &ClientConnection::handleReadyRead);
}

// 析构函数：清理资源，断开连接
ClientConnection::~ClientConnection()
{
    socket->disconnectFromHost();  // 断开与服务器的连接
}

// 连接到服务器的函数
bool ClientConnection::connectToServer(const QString &host, quint16 port)
{
    qDebug() << "正在连接到服务器:" << host << ":" << port;
    socket->connectToHost(host, port);  // 尝试连接到指定主机和端口
    bool connected = socket->waitForConnected(5000);  // 等待连接完成，最多等5秒
    if (connected) {
        qDebug() << "成功连接到服务器";
    } else {
        qDebug() << "连接服务器失败:" << socket->errorString();
    }
    return connected;
}

// 断开与服务器的连接
void ClientConnection::disconnectFromServer()
{
    qDebug() << "断开与服务器的连接";
    socket->disconnectFromHost();  // 断开连接
}

// 发送JSON消息到服务器
void ClientConnection::sendJsonMessage(const QJsonObject &json)
{
    // 检查是否已连接到服务器
    if (socket->state() != QAbstractSocket::ConnectedState) {
        qDebug() << "未连接到服务器，无法发送消息";
        emit error("未连接到服务器");
        return;
    }
    
    QJsonDocument doc(json);
    QByteArray data = doc.toJson();
    
    qDebug() << "发送消息到服务器:" << json["type"].toString();
    socket->write(data);
    socket->flush();
}

// 发送用户数据到服务器
void ClientConnection::sendUserData(const UserData &userData)
{
    // 创建JSON对象来存储数据
    QJsonObject json;
    json["type"] = "calculate_gpa";
    // TODO: 实现数据序列化
    
    sendJsonMessage(json);
}

// 保存用户数据到服务器
void ClientConnection::saveUserData(const QString &userId, users* user)
{
    if (!user) {
        emit userDataSaved(false, "用户数据为空");
        return;
    }
    
    QJsonObject json;
    json["type"] = "save_user_data";
    json["userId"] = userId;
    
    // 序列化用户数据（直接访问公共成员变量）
    QJsonObject userData;
    userData["name"] = QString::fromStdString(user->name);
    userData["height"] = user->height;
    userData["weight"] = user->bodyweight;
    userData["currentGPA"] = user->getSportGPA();
    userData["guessGPA"] = user->getGuessGPA();
    userData["age"] = user->age;
    userData["gender"] = user->gender;
    userData["squatRatio"] = user->squatRatio;
    userData["benchPressRatio"] = user->benchPressRatio;
    userData["deadliftRatio"] = user->deadliftRatio;
    userData["bodyFat"] = user->bodyFat;
    userData["hasBodyFat"] = user->hasBodyFat;
    userData["hasSquat"] = user->hasSquat;
    userData["hasBenchPress"] = user->hasBenchPress;
    userData["hasDeadlift"] = user->hasDeadlift;
    
    json["userData"] = userData;
    
    sendJsonMessage(json);
}

// 从服务器加载用户数据
void ClientConnection::loadUserData(const QString &userId)
{
    QJsonObject json;
    json["type"] = "load_user_data";
    json["userId"] = userId;
    
    sendJsonMessage(json);
}

// 保存训练数据到服务器
void ClientConnection::saveTrainData(const QString &userId, const train &trainPlan)
{
    QJsonObject json;
    json["type"] = "save_train_data";
    json["userId"] = userId;
    json["trainData"] = trainPlan.toJson();
    
    sendJsonMessage(json);
}

// 从服务器加载训练数据
void ClientConnection::loadTrainData(const QString &userId)
{
    QJsonObject json;
    json["type"] = "load_train_data";
    json["userId"] = userId;
    
    sendJsonMessage(json);
}

// 处理连接成功的事件
void ClientConnection::handleConnected()
{
    qDebug() << "已连接到服务器";
    emit connected();  // 发出连接成功的信号
}

// 处理断开连接的事件
void ClientConnection::handleDisconnected()
{
    qDebug() << "与服务器断开连接";
    emit disconnected();  // 发出断开连接的信号
}

// 处理错误事件
void ClientConnection::handleError(QAbstractSocket::SocketError socketError)
{
    qDebug() << "Socket错误:" << socket->errorString();
    emit error(socket->errorString());  // 发出错误信号，包含错误信息
}

// 处理收到新数据的事件
void ClientConnection::handleReadyRead()
{
    // 读取所有可用数据
    QByteArray data = socket->readAll();
    
    qDebug() << "收到服务器响应:" << data;
    
    // 将数据解析为JSON文档
    QJsonDocument doc = QJsonDocument::fromJson(data);
    
    // 检查是否是有效的JSON对象
    if (doc.isObject()) {
        QJsonObject json = doc.object();
        handleResponse(json);
    }
}

// 处理服务器响应
void ClientConnection::handleResponse(const QJsonObject &json)
{
    QString type = json["type"].toString();
    qDebug() << "处理响应类型:" << type;
    
    if (type == "evaluation") {
        // 处理GPA评估结果
        if (json.contains("gpa")) {
            double gpa = json["gpa"].toDouble();
            emit evaluationReceived(gpa);
        }
    }
    else if (type == "save_user_data_response") {
        // 处理保存用户数据响应
        bool success = (json["status"].toString() == "success");
        QString message = json["message"].toString();
        emit userDataSaved(success, message);
    }
    else if (type == "load_user_data_response") {
        // 处理加载用户数据响应
        bool success = (json["status"].toString() == "success");
        
        if (success && json.contains("userData")) {
            QJsonObject userData = json["userData"].toObject();
            
            // 注意: users是抽象类，不能直接实例化
            // 这里需要根据实际用户类型创建对应的子类对象
            // 暂时返回失败，需要在上层处理
            qDebug() << "收到用户数据，但users是抽象类，无法直接实例化";
            emit userDataLoaded(false, nullptr);
        } else {
            emit userDataLoaded(false, nullptr);
        }
    }
    else if (type == "save_train_data_response") {
        // 处理保存训练数据响应
        bool success = (json["status"].toString() == "success");
        QString message = json["message"].toString();
        emit trainDataSaved(success, message);
    }
    else if (type == "load_train_data_response") {
        // 处理加载训练数据响应
        bool success = (json["status"].toString() == "success");
        
        if (success && json.contains("trainData")) {
            QJsonObject trainData = json["trainData"].toObject();
            train* trainPlan = new train();
            *trainPlan = train::fromJson(trainData);
            
            emit trainDataLoaded(true, trainPlan);
        } else {
            emit trainDataLoaded(false, nullptr);
        }
    }
}