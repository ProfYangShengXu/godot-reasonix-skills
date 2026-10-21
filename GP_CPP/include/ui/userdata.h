#ifndef USERDATA_H  // 头文件保护宏开始
#define USERDATA_H

// 包含必要的Qt头文件和自定义头文件
#include <QObject>    // Qt基础对象类
#include <QString>    // Qt字符串类，提供Unicode字符串支持
#include "../analyseuser.h"  // 用户分析相关的头文件

// UserData类：管理用户的体育测试数据
// 继承自QObject以支持信号槽机制
class UserData : public QObject
{
    Q_OBJECT  // 启用Qt的信号槽机制

public:
    // 构造函数：创建用户数据对象
    // explicit防止隐式转换
    explicit UserData(QObject *parent = nullptr);
    
    // 析构函数：清理资源
    ~UserData();

    // 设置用户基本体测数据
    // @param user: 用户对象指针
    // @param lungCapacity: 肺活量(ml)
    // @param run50m: 50米跑成绩(秒)
    // @param sitAndReach: 坐位体前屈(cm)
    // @param standingJump: 立定跳远(cm)
    // @param pullUps: 引体向上(次)
    // @param longRunMin: 长跑分钟数
    // @param longRunSec: 长跑秒数
    // @param sitUps: 仰卧起坐(次)
    void setUserBasicData(users* user, double lungCapacity, double run50m, double sitAndReach,
                         double standingJump, double pullUps, double longRunMin, double longRunSec,
                         double sitUps);
    
    // 设置用户可选的力量测试数据
    // @param user: 用户对象指针
    // @param bodyFat: 体脂率(%)
    // @param squatRatio: 深蹲体重比
    // @param benchPressRatio: 卧推体重比
    // @param deadliftRatio: 硬拉体重比
    void setUserOptionalData(users* user, double bodyFat, double squatRatio,
                            double benchPressRatio, double deadliftRatio);
    
    // 设置用户可选测试项目的完成标志
    // @param user: 用户对象指针
    // @param hasBodyFat: 是否测试体脂率
    // @param hasSquat: 是否测试深蹲
    // @param hasBenchPress: 是否测试卧推
    // @param hasDeadlift: 是否测试硬拉
    void setUserOptionalFlags(users* user, bool hasBodyFat, bool hasSquat,
                            bool hasBenchPress, bool hasDeadlift);

    // 计算用户的体测总评分
    // @param user: 用户对象指针
    // @return: 返回计算得出的GPA(0.0-4.0)
    double calculateUserGPA(users* user);

// 信号声明
signals:
    void dataChanged();  // 数据变更信号
    void calculationComplete(double gpa);  // 计算完成信号，携带GPA结果
    void error(const QString& message);    // 错误信号，携带错误信息
};

#endif // USERDATA_H