#include "ui/mainwindow.h"
#include "../build/sport_evaluation_client_autogen/include/ui_mainwindow.h"
#include "ui/trainwindow.h"
#include "ui/chatwindow.h"
#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , currentUser(nullptr)
    , trainWindow(nullptr)
    , chatWindow(nullptr)
    , serverConnection(nullptr)
{
    ui->setupUi(this);
    // 设置初始状态
    ui->spinBodyFat->setEnabled(false);
    ui->spinSquat->setEnabled(false);
    ui->spinBenchPress->setEnabled(false);
    ui->spinDeadlift->setEnabled(false);
    
    // 连接UserData信号
    connect(&userData, &UserData::error, this, [this](const QString& message) {
        QMessageBox::warning(this, "错误", message);
    });
    
    connect(&userData, &UserData::calculationComplete, this, [this](double gpa) {
        ui->lcdGPA->display(gpa);
    });
    
    // 初始化服务器连接
    initServerConnection();
}

MainWindow::~MainWindow()
{
    // 保存数据到服务器
    if (currentUser && serverConnection) {
        saveCurrentUserToServer();
    }
    
    delete ui;
    if (currentUser) {
        delete currentUser;
    }
}

// 初始化服务器连接
void MainWindow::initServerConnection()
{
    serverConnection = new ClientConnection(this);
    
    // 连接服务器信号
    connect(serverConnection, &ClientConnection::connected,
            this, &MainWindow::onServerConnected);
    connect(serverConnection, &ClientConnection::disconnected,
            this, &MainWindow::onServerDisconnected);
    connect(serverConnection, &ClientConnection::error,
            this, &MainWindow::onServerError);
    connect(serverConnection, &ClientConnection::userDataSaved,
            this, &MainWindow::onUserDataSaved);
    connect(serverConnection, &ClientConnection::userDataLoaded,
            this, &MainWindow::onUserDataLoaded);
    
    // 尝试连接到服务器
    if (!serverConnection->connectToServer("127.0.0.1", 12345)) {
        qDebug() << "无法连接到服务器，将以离线模式运行";
    }
}

// 保存当前用户数据到服务器
void MainWindow::saveCurrentUserToServer()
{
    if (!currentUser || !serverConnection) {
        return;
    }
    
    QString userId = QString::fromStdString(currentUser->name);
    serverConnection->saveUserData(userId, currentUser);
}

// 从服务器加载用户数据
void MainWindow::loadCurrentUserFromServer()
{
    if (!serverConnection) {
        return;
    }
    
    QString userId = ui->editName->text();
    if (!userId.isEmpty()) {
        serverConnection->loadUserData(userId);
    }
}

// 服务器连接成功
void MainWindow::onServerConnected()
{
    qDebug() << "已连接到服务器";
    // 可以尝试加载之前保存的用户数据
}

// 服务器断开连接
void MainWindow::onServerDisconnected()
{
    qDebug() << "与服务器断开连接";
}

// 服务器错误
void MainWindow::onServerError(const QString &errorString)
{
    qDebug() << "服务器错误:" << errorString;
}

// 用户数据保存响应
void MainWindow::onUserDataSaved(bool success, const QString &message)
{
    if (success) {
        qDebug() << "用户数据已保存到服务器";
    } else {
        qDebug() << "保存用户数据失败:" << message;
    }
}

// 用户数据加载响应
void MainWindow::onUserDataLoaded(bool success, users* user)
{
    if (success && user) {
        // 删除旧用户对象
        if (currentUser) {
            delete currentUser;
        }
        
        currentUser = user;
        
        // 更新UI显示
        ui->lcdGPA->display(currentUser->getSportGPA());
        qDebug() << "用户数据已从服务器加载";
    } else {
        qDebug() << "加载用户数据失败";
    }
}

void MainWindow::on_buttonCalculate_clicked()
{
    // 验证必填数据
    if (ui->editName->text().isEmpty()) {
        QMessageBox::warning(this, "警告", "请输入姓名");
        return;
    }

    // 删除旧的用户对象
    if (currentUser) {
        delete currentUser;
    }

    // 创建新的用户对象
    QString userType = ui->comboType->currentText();
    bool isMale = ui->comboGender->currentText() == "男";
    
    if (userType == "基础用户") {
        currentUser = new Regular_users(ui->editName->text().toStdString(),
                                    ui->spinAge->value(),
                                    isMale,
                                    ui->spinWeight->value(),
                                    ui->spinHeight->value());
    }
    else if (userType == "举重运动员") {
        currentUser = new Powerlifting_users(ui->editName->text().toStdString(),
                                         ui->spinAge->value(),
                                         isMale,
                                         ui->spinWeight->value(),
                                         ui->spinHeight->value());
    }
    else if (userType == "长跑运动员") {
        currentUser = new Longruning_users(ui->editName->text().toStdString(),
                                       ui->spinAge->value(),
                                       isMale,
                                       ui->spinWeight->value(),
                                       ui->spinHeight->value());
    }
    else if (userType == "短跑运动员") {
        currentUser = new Shortrunning_users(ui->editName->text().toStdString(),
                                         ui->spinAge->value(),
                                         isMale,
                                         ui->spinWeight->value(),
                                         ui->spinHeight->value());
    }
    else if (userType == "健美运动员") {
        currentUser = new Bodybuilding_users(ui->editName->text().toStdString(),
                                         ui->spinAge->value(),
                                         isMale,
                                         ui->spinWeight->value(),
                                         ui->spinHeight->value());
    }
    else if (userType == "自定义用户") {
        currentUser = new Selfdefined_users(ui->editName->text().toStdString(),
                                        ui->spinAge->value(),
                                        isMale,
                                        ui->spinWeight->value(),
                                        ui->spinHeight->value(),
                                        30, 30, 20, 20); // 默认权重
    }

    if (!currentUser) {
        QMessageBox::warning(this, "错误", "创建用户对象失败");
        return;
    }

    // 设置基本测试数据
    userData.setUserBasicData(currentUser,
                           ui->spinLungCapacity->value(),
                           ui->spinRun50m->value(),
                           ui->spinSitAndReach->value(),
                           ui->spinStandingJump->value(),
                           ui->spinPullUps->value(),
                           ui->spinLongRunMin->value(),
                           ui->spinLongRunSec->value(),
                           0.0); // sitUps 暂不使用

    // 设置可选项目标志
    userData.setUserOptionalFlags(currentUser,
                               ui->checkBodyFat->isChecked(),
                               ui->checkSquat->isChecked(),
                               ui->checkBenchPress->isChecked(),
                               ui->checkDeadlift->isChecked());

    // 设置可选项目数据
    userData.setUserOptionalData(currentUser,
                              ui->spinBodyFat->value(),
                              ui->spinSquat->value(),
                              ui->spinBenchPress->value(),
                              ui->spinDeadlift->value());

    // 计算评分
    userData.calculateUserGPA(currentUser);
    
    // 保存到服务器
    saveCurrentUserToServer();
}

void MainWindow::on_checkBodyFat_toggled(bool checked)
{
    ui->spinBodyFat->setEnabled(checked);
}

void MainWindow::on_checkSquat_toggled(bool checked)
{
    ui->spinSquat->setEnabled(checked);
}

void MainWindow::on_checkBenchPress_toggled(bool checked)
{
    ui->spinBenchPress->setEnabled(checked);
}

void MainWindow::on_checkDeadlift_toggled(bool checked)
{
    ui->spinDeadlift->setEnabled(checked);
}

void MainWindow::on_comboType_currentIndexChanged(int index)
{
    // 根据用户类型启用/禁用相关选项
    bool isWeightlifter = ui->comboType->currentText() == "举重运动员";
    bool isCustomUser = ui->comboType->currentText() == "自定义用户";
    
    ui->checkSquat->setVisible(isWeightlifter || isCustomUser);
    ui->spinSquat->setVisible(isWeightlifter || isCustomUser);
    ui->checkBenchPress->setVisible(isWeightlifter || isCustomUser);
    ui->spinBenchPress->setVisible(isWeightlifter || isCustomUser);
    ui->checkDeadlift->setVisible(isWeightlifter || isCustomUser);
    ui->spinDeadlift->setVisible(isWeightlifter || isCustomUser);
}
void MainWindow::on_btnOpenTrainWindow_clicked()
{
    // ����Ƿ��Ѵ����û�����
    if (!currentUser) {
        QMessageBox::warning(
            this,
            "��ʾ",
            "������д�û���Ϣ������GPA��Ȼ���ٴ�ѵ���ƻ�������"
        );
        return;
    }
    
    // ���ѵ�����ڻ�δ������������
    if (!trainWindow) {
        trainWindow = new TrainWindow(this, currentUser);
    }
    
    // ��ʾѵ���ƻ�����
    trainWindow->show();
    trainWindow->raise();  // ��������������ǰ��
    trainWindow->activateWindow();  // �����
}


void MainWindow::on_btnOpenChatWindow_clicked()
{
    // ������촰�ڻ�δ�������򴴽�
    if (!chatWindow) {
        chatWindow = new ChatWindow(this);
    }
    chatWindow->show();
    chatWindow->raise();
    chatWindow->activateWindow();
}
