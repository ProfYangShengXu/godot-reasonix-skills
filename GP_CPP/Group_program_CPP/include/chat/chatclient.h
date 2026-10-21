#ifndef CHATCLIENT_H
#define CHATCLIENT_H

#include <QObject>
#include <QTcpSocket>

// 聊天客户端类
class ChatClient : public QObject
{
    Q_OBJECT

public:
    explicit ChatClient(QObject *parent = nullptr);
    ~ChatClient();
    
    // 连接到服务器
    bool connectToServer(const QString &host, quint16 port);
    
    // 断开连接
    void disconnectFromServer();
    
    // 发送消息
    void sendMessage(const QString &message);
    
    // 是否已连接
    bool isConnected() const;

signals:
    void connected();
    void disconnected();
    void messageReceived(const QString &message);
    void error(const QString &errorString);

private slots:
    void onConnected();
    void onDisconnected();
    void onReadyRead();
    void onError(QAbstractSocket::SocketError socketError);

private:
    QTcpSocket *socket;
};

#endif // CHATCLIENT_H
