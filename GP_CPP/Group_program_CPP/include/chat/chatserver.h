#ifndef CHATSERVER_H
#define CHATSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QList>
#include <QDateTime>

// 聊天服务器类
class ChatServer : public QObject
{
    Q_OBJECT

public:
    explicit ChatServer(QObject *parent = nullptr);
    ~ChatServer();
    
    // 启动服务器
    bool start(quint16 port = 12346);
    
    // 停止服务器
    void stop();
    
    // 获取在线客户端数量
    int getClientCount() const;

signals:
    void clientConnected(const QString &address);
    void clientDisconnected(const QString &address);
    void messageReceived(const QString &message);
    void serverError(const QString &error);

private slots:
    void onNewConnection();
    void onClientReadyRead();
    void onClientDisconnected();

private:
    QTcpServer *server;
    QList<QTcpSocket*> clients;
    
    // 广播消息给所有客户端
    void broadcastMessage(const QString &message, QTcpSocket *sender);
    
    // 获取格式化的时间戳
    QString getTimeStamp() const;
};

#endif // CHATSERVER_H
