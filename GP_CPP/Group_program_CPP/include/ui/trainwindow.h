// =====================================================
// 文件名: trainwindow.h
// 功能: 训练计划管理窗口的头文件
// 说明: 提供图形界面来创建和管理训练计划
// =====================================================

#ifndef TRAINWINDOW_H  // 防止头文件重复包含
#define TRAINWINDOW_H

// Qt框架核心组件
#include <QMainWindow>  // Qt主窗口类
#include <QListWidget>  // 列表控件，用于显示项目
#include <QDialog>      // 对话框类
#include <QDate>        // 日期类

// 自定义的训练计划系统
#include "../train.h"
#include "client/clientconnection.h"  // 客户端连接类

// Qt命名空间声明
QT_BEGIN_NAMESPACE
namespace Ui { class TrainWindow; }  // UI类的前向声明
QT_END_NAMESPACE

// =====================================================
// 类名: TrainWindow
// 功能: 训练计划管理的主窗口
// 继承: QMainWindow（Qt的主窗口类）
// =====================================================
class TrainWindow : public QMainWindow
{
    Q_OBJECT  // Qt元对象宏，启用信号槽机制

public:
    // 构造函数：创建训练计划窗口
    // @param parent: 父窗口指针
    // @param user: 当前用户对象指针（用于计算训练难度）
    explicit TrainWindow(QWidget *parent = nullptr, users* user = nullptr);
    
    // 析构函数：清理资源
    ~TrainWindow();

// 私有槽函数：响应UI事件
private slots:
    // === 日期管理 ===
    void on_calendarWidget_clicked(const QDate &date);  // 日历点击事件
    void on_btnAddTrainToDate_clicked();  // 添加训练计划到日期
    void on_btnDeleteTrainFromDate_clicked();  // 从日期删除训练计划
    
    // === 训练计划管理 ===
    void on_btnNewTrainPlan_clicked();  // 创建新训练计划
    void on_btnDeleteTrainPlan_clicked();  // 删除训练计划
    void on_btnSaveTrainPlan_clicked();  // 保存训练计划
    void on_listTrainPlans_itemClicked(QListWidgetItem *item);  // 选择训练计划
    
    // === 动作管理 ===
    void on_btnAddAction_clicked();  // 添加动作
    void on_btnDeleteAction_clicked();  // 删除动作
    void on_btnModifyAction_clicked();  // 修改动作
    void on_listActions_itemClicked(QListWidgetItem *item);  // 选择动作
    
    // === 训练组管理 ===
    void on_btnAddSet_clicked();  // 添加训练组
    void on_btnDeleteSet_clicked();  // 删除训练组
    void on_btnModifySet_clicked();  // 修改训练组
    void on_listSets_itemClicked(QListWidgetItem *item);  // 选择训练组
    
    // === 教学链接管理 ===
    void on_btnAddTutorial_clicked();  // 添加教学视频链接
    void on_btnDeleteTutorial_clicked();  // 删除教学视频链接
    void on_btnCopyTutorial_clicked();  // 复制教学视频链接
    
    // === 服务器数据同步 ===
    void onTrainDataSaved(bool success, const QString &message);
    void onTrainDataLoaded(bool success, train* trainPlan);

private:
    Ui::TrainWindow *ui;  // UI对象指针
    users* currentUser;   // 当前用户对象指针
    
    // === 数据存储 ===
    std::vector<date> calendar;  // 日历数据（存储所有日期的训练安排）
    std::vector<train> trainPlans;  // 所有训练计划库
    
    // === 当前选中的索引 ===
    int currentDateIndex;  // 当前选中的日期索引
    int currentTrainIndex;  // 当前选中的训练计划索引
    int currentActionIndex;  // 当前选中的动作索引
    int currentSetIndex;  // 当前选中的训练组索引
    
    // === 服务器连接 ===
    ClientConnection *serverConnection;  // 服务器连接对象
    
    // === 私有辅助函数 ===
    // 刷新训练计划列表显示
    void refreshTrainPlansList();
    
    // 刷新动作列表显示
    void refreshActionsList();
    
    // 刷新训练组列表显示
    void refreshSetsList();
    
    // 刷新教学链接列表显示
    void refreshTutorialsList();
    
    // 刷新当前日期的训练计划
    void refreshDateTrains();
    
    // 加载选中的训练计划详情
    void loadTrainPlanDetails();
    
    // 查找日期在日历中的索引
    // @param selectedDate: 要查找的日期
    // @return: 日期索引，如果不存在返回-1
    int findDateIndex(const QDate &selectedDate);
    
    // === 数据持久化 ===
    // 保存所有数据到服务器
    void saveAllDataToServer();
    
    // 从服务器加载所有数据
    void loadAllDataFromServer();
    
    // 从本地文件加载数据（后备方案）
    void loadAllDataLocal();
    
    // 初始化服务器连接
    void initServerConnection();
};

#endif // TRAINWINDOW_H
