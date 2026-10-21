#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QMainWindow>
#include "chat/chatclient.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ChatWindow; }
QT_END_NAMESPACE

class ChatWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ChatWindow(QWidget *parent = nullptr);
    ~ChatWindow();

private slots:
    void on_btnConnect_clicked();
    void on_btnSend_clicked();
    void on_lineMessage_returnPressed();
    
    void onConnected();
    void onDisconnected();
    void onMessageReceived(const QString &message);
    void onError(const QString &errorString);

private:
    Ui::ChatWindow *ui;
    ChatClient *chatClient;
    QString username;
    
    void appendMessage(const QString &message);
    void setConnectedState(bool connected);
};

#endif // CHATWINDOW_H
