// =====================================================
// 文件名: userdata.cpp
// 功能: 用户数据管理类的实现
// 说明: 提供用户数据设置和GPA计算功能
// =====================================================

#include "ui/userdata.h"

// =====================================================
// 构造函数：初始化用户数据管理对象
// =====================================================
// @param parent: 父对象指针，用于Qt对象树管理
UserData::UserData(QObject *parent)
    : QObject(parent)  // 初始化基类
{
    // 构造函数体为空，所有初始化在初始化列表中完成
}

// =====================================================
// 析构函数：清理资源
// =====================================================
UserData::~UserData()
{
    // 析构函数体为空，Qt会自动管理子对象的内存
}

// 用户创建功能已移至mainwindow.cpp中直接使用构造函数

// =====================================================
// 设置用户基础数据
// =====================================================
// 功能：设置用户的基本体测数据（必填项）
// @param user: 用户对象指针
// @param lungCapacity: 肺活量（毫升）
// @param run50m: 50米跑成绩（秒）
// @param sitAndReach: 坐位体前屈（厘米）
// @param standingJump: 立定跳远（米）
// @param pullUps: 引体向上次数（男生）
// @param longRunMin: 长跑成绩-分钟部分（男1000米/女800米）
// @param longRunSec: 长跑成绩-秒钟部分
// @param sitUps: 仰卧起坐次数（女生）
void UserData::setUserBasicData(users* user, double lungCapacity, double run50m, double sitAndReach,
                               double standingJump, double pullUps, double longRunMin, double longRunSec,
                               double sitUps)
{
    // 检查用户对象是否有效
    if (user) {
        // 调用用户对象的setBasicData方法设置基础数据
        user->setBasicData(lungCapacity, run50m, sitAndReach, standingJump, pullUps, longRunMin, longRunSec, sitUps);
        // 发出数据已改变的信号，通知UI更新
        emit dataChanged();
    } else {
        // 如果用户对象无效，发出错误信号
        emit error("Invalid user object");
    }
}

// =====================================================
// 设置用户可选数据
// =====================================================
// 功能：设置用户的可选体测数据（选填项）
// @param user: 用户对象指针
// @param bodyFat: 体脂率（%）
// @param squatRatio: 深蹲重量（kg）
// @param benchPressRatio: 卧推重量（kg）
// @param deadliftRatio: 硬拉重量（kg）
void UserData::setUserOptionalData(users* user, double bodyFat, double squatRatio,
                                  double benchPressRatio, double deadliftRatio)
{
    // 检查用户对象是否有效
    if (user) {
        // 调用用户对象的setOptionalData方法设置可选数据
        user->setOptionalData(bodyFat, squatRatio, benchPressRatio, deadliftRatio);
        // 发出数据已改变的信号
        emit dataChanged();
    } else {
        // 如果用户对象无效，发出错误信号
        emit error("Invalid user object");
    }
}

// =====================================================
// 设置用户可选数据标志
// =====================================================
// 功能：标记哪些可选数据项被填写了
// @param user: 用户对象指针
// @param hasBodyFat: 是否填写了体脂率
// @param hasSquat: 是否填写了深蹲
// @param hasBenchPress: 是否填写了卧推
// @param hasDeadlift: 是否填写了硬拉
void UserData::setUserOptionalFlags(users* user, bool hasBodyFat, bool hasSquat,
                                   bool hasBenchPress, bool hasDeadlift)
{
    // 检查用户对象是否有效
    if (user) {
        // 调用用户对象的ifhasOptionalData方法设置标志
        user->ifhasOptionalData(hasBodyFat, hasSquat, hasBenchPress, hasDeadlift);
        // 发出数据已改变的信号
        emit dataChanged();
    } else {
        // 如果用户对象无效，发出错误信号
        emit error("Invalid user object");
    }
}

// =====================================================
// 计算用户运动GPA
// =====================================================
// 功能：根据用户的所有数据计算运动GPA评分
// @param user: 用户对象指针
// @return: 计算得到的运动GPA值（0.0-4.0）
double UserData::calculateUserGPA(users* user)
{
    // 检查用户对象是否有效
    if (user) {
        // 调用用户对象的calculateSportGPA方法进行计算
        user->calculateSportGPA();
        // 获取计算结果
        double gpa = user->getSportGPA();
        // 发出计算完成信号，传递GPA值
        emit calculationComplete(gpa);
        // 返回GPA值
        return gpa;
    } else {
        // 如果用户对象无效，发出错误信号并返回0
        emit error("Invalid user object");
        return 0.0;
    }
}