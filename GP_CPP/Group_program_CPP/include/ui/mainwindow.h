#ifndef MAINWINDOW_H  // 防止头文件重复包含
#define MAINWINDOW_H

// Qt框架相关头文件
#include <QMainWindow>  // Qt主窗口类
#include "ui/userdata.h"  // 用户数据管理类
#include "client/clientconnection.h"  // 客户端网络连接类
#include "analyseuser.h"  // 用户子类定义

// Qt命名空间开始
// 这是Qt的标准做法，用于避免名称冲突
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }  // 前向声明MainWindow UI类
QT_END_NAMESPACE

// 前向声明训练窗口类
class TrainWindow;
class ChatWindow;

// MainWindow类：应用程序的主窗口
// 继承自QMainWindow，提供菜单栏、工具栏、状态栏等标准窗口功能
class MainWindow : public QMainWindow
{
    Q_OBJECT  // 启用Qt的信号槽机制

public:
    // 构造函数：创建主窗口
    // @param parent: 父窗口指针，用于Qt的对象树管理
    explicit MainWindow(QWidget *parent = nullptr);
    
    // 析构函数：清理资源
    ~MainWindow();

// private slots：这些是Qt的槽函数，用于响应UI事件
private slots:
    // 响应"计算"按钮点击事件
    void on_buttonCalculate_clicked();

    // 响应体脂率复选框状态改变
    // @param checked: true表示选中，false表示取消选中
    void on_checkBodyFat_toggled(bool checked);

    // 响应深蹲复选框状态改变
    void on_checkSquat_toggled(bool checked);

    // 响应卧推复选框状态改变
    void on_checkBenchPress_toggled(bool checked);

    // 响应硬拉复选框状态改变
    void on_checkDeadlift_toggled(bool checked);

    // 响应用户类型下拉框选择改变
    // @param index: 选中项的索引
    void on_comboType_currentIndexChanged(int index);
    
    // 打开训练计划管理窗口
    void on_btnOpenTrainWindow_clicked();
    
    // 打开聊天室窗口
    void on_btnOpenChatWindow_clicked();
    
    // 处理服务器连接状态
    void onServerConnected();
    void onServerDisconnected();
    void onServerError(const QString &errorString);
    
    // 处理用户数据保存/加载响应
    void onUserDataSaved(bool success, const QString &message);
    void onUserDataLoaded(bool success, users* user);

// 私有成员
private:
    Ui::MainWindow *ui;      // UI对象指针，由Qt Designer生成
    UserData userData;       // 用户数据管理对象
    users* currentUser;      // 当前活动用户对象指针
    TrainWindow *trainWindow;  // 训练计划管理窗口指针
    ChatWindow *chatWindow;    // 聊天室窗口指针
    ClientConnection *serverConnection;  // 服务器连接对象
    
    // 初始化服务器连接
    void initServerConnection();
    
    // 保存当前用户数据到服务器
    void saveCurrentUserToServer();
    
    // 从服务器加载用户数据
    void loadCurrentUserFromServer();
};

#endif // MAINWINDOW_H