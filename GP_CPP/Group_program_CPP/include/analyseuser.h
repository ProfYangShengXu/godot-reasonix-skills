#pragma once  // 头文件保护，防止重复包含（比#ifndef更现代的方式）

// 标准库头文件包含
#include <string>  // 字符串处理
#include <cmath>   // 数学函数

/*
作者信息：
署名 陈霆钧 124090046
*/

// ===== 工具函数声明 =====
// 将百分制分数转换为GPA(0-4分制)
double PersentageToGpa(double percentage);

// 计算深蹲的GL分数（国际级力量举重评分标准）
// @param sq: 深蹲重量
// @param bodyweight: 体重
// @param gender: 性别（true为男，false为女）
double Glpointsq(double sq, double bodyweight, bool gender);

// 计算卧推的GL分数
double Glpointbp(double bp, double bodyweight, bool gender);

// 计算硬拉的GL分数
double Glpointdp(double dp, double bodyweight, bool gender);

// ===== 用户基类 =====
// 定义基础用户数据结构和接口
struct users
{
   public:
    // ==== 基本信息 ====
    std::string name;      // 姓名
    int age;              // 年龄
    bool gender;          // 性别（true为男，false为女）
    int divisor=14;       // 总评成绩除数（可选项占双倍权重）
    double bodyweight;    // 体重
    double height;        // 身高
    
    // ==== 基本体测数据（所有数据均为百分制分数）====
    double lungCapacity;    // 肺活量(ml)
    double run50m;          // 50米跑(秒)
    double sitAndReach;     // 坐位体前屈(cm)
    double standingJump;    // 立定跳远(cm)
    double pullUps;         // 引体向上（男）
    double sitUps;          // 仰卧起坐（女）
    double longRun;         // 长跑（男1000米/女800米）
    
    // ==== 可选专项测试项目 ====
    double bodyFat;         // 体脂率(%)
    double squatRatio;      // 深蹲体重比
    double benchPressRatio; // 卧推体重比
    double deadliftRatio;   // 硬拉体重比
    
    // ==== 可选项目完成标志 ====
    bool hasBodyFat = false;    // 是否测试体脂
    bool hasSquat = false;      // 是否测试深蹲
    bool hasBenchPress = false; // 是否测试卧推
    bool hasDeadlift = false;   // 是否测试硬拉

    // ==== 评估结果 ====
    double sport_gpa;      // 运动总评成绩（0-4分制）
    double guess_gpa;  // 预测完成训练后的运动GPA
public:
    // ==== 成员函数 ====
    // 构造函数：初始化用户基本信息
    // @param n: 姓名
    // @param a: 年龄
    // @param g: 性别（true为男，false为女）
    // @param bw: 体重
    // @param h: 身高
    users(std::string n, int a, bool g, double bw, double h);

    // 设置用户基础体测数据
    // @param lc: 肺活量（毫升）
    // @param r50m: 50米跑（秒）
    // @param sar: 坐位体前屈（厘米）
    // @param sj: 立定跳远（米）
    // @param pu: 引体向上（个）
    // @param mlr: 1000米跑（分）
    // @param slr: 1000米跑（秒）
    // @param su: 仰卧起坐（个）
    void setBasicData(double lc, double r50m, double sar,
         double sj, double pu, double mlr, double slr, double su);

    // 设置可选项目数据
    // @param bf: 体脂率
    // @param sr: 深蹲体重比
    // @param bpr: 卧推体重比
    // @param dr: 硬拉体重比
    void setOptionalData(double bf, double sr, double bpr, double dr);

    // 设置可选项目完成标志
    // @param hb: 是否完成体脂测试
    // @param hs: 是否完成深蹲
    // @param hbp: 是否完成卧推
    // @param hd: 是否完成硬拉
    void ifhasOptionalData(bool hb, bool hs, bool hbp, bool hd);

    // 纯虚函数：计算运动总评成绩
    // 每个子类必须实现自己的计算方法
    virtual void calculateSportGPA() = 0;

    // 获取运动总评成绩
    // @return: 返回GPA（0-4分制）
    double getSportGPA() const;
    
    // 获取预测运动GPA
    // @return: 返回预测GPA（0-4分制）
    double getGuessGPA() const;
    
    // 设置预测运动GPA
    // @param gpa: 预测的GPA值
    void setGuessGPA(double gpa);
};

// ===== 特定用户类型 =====

// 普通用户类型
// 标准体测评分标准
struct Regular_users : public users
{
    using users::users;  // 继承基类构造函数
    void calculateSportGPA() override;  // 实现标准评分方法
};

// 力量举重用户类型
// 重点考察深蹲、卧推、硬拉三大项
struct Powerlifting_users : public users {
    using users::users;
    void calculateSportGPA() override;  // 实现力量举重特定评分
};

// 长跑运动员类型
// 重点考察耐力项目
struct Longruning_users : public users {
    using users::users;
    void calculateSportGPA() override;  // 实现长跑特定评分
};

// 短跑运动员类型
// 重点考察爆发力项目
struct Shortrunning_users : public users {
    using users::users;
    void calculateSportGPA() override;  // 实现短跑特定评分
};

// 健美运动员类型
// 重点考察体型体脂和力量表现
struct Bodybuilding_users : public users {
    using users::users;
    void calculateSportGPA() override;  // 实现健美特定评分
};

// 自定义评分类型
// 允许自定义各项目权重
struct Selfdefined_users : public users
{
    // ==== 自定义权重 ====
    int strenggthWeight;    // 力量项目权重
    int enduranceWeight;    // 耐力项目权重
    int flexibilityWeight;  // 柔韧项目权重
    int speedWeight;        // 速度项目权重

    // 构造函数：初始化用户信息和自定义权重
    Selfdefined_users(std::string n, int a, bool g, double bw, double h,
        int sw,  // 力量权重
        int ew,  // 耐力权重
        int fw,  // 柔韧权重
        int spw  // 速度权重
    );

    // 根据自定义权重计算总评成绩
    void calculateSportGPA() override;
};
// 功能函数声明
double PersentageToGpa(double percentage);
double Glpointsq(double sq, double bodyweight, bool gender);
double Glpointbp(double bp, double bodyweight, bool gender);
double Glpointdp(double dp, double bodyweight, bool gender);
/*使用例
#include "../include/analyseuser.h"
#include <iostream>
#include <iomanip>
#include <windows.h>

void printUserInfo(const users* user) {
    std::cout << "测试结果:" << std::endl;
    std::cout << "运动GPA: " << std::fixed << std::setprecision(2) << user->getSportGPA() << std::endl;
    std::cout << "----------------------------------------" << std::endl;
}

int main() {
    // 设置控制台输出为 UTF-8
    SetConsoleOutputCP(CP_UTF8);
    
    // 测试基础用户
    users baseUser("张三", 25, true, 70.0, 175.0);
    baseUser.setBasicData(
        4200,   // 肺活量(ml)
        6.8,    // 50米(秒)
        15.0,   // 体前屈(cm)
        2.5,    // 立定跳远(m)
        12,     // 引体向上(个)
        4,      // 1000米跑(分)
        0,      // 1000米跑(秒)
        0       // 仰卧起坐(次)
    );
    baseUser.ifhasOptionalData(true, true, true, true);
    baseUser.setOptionalData(15.0, 200.0, 150.0, 250.0);
    baseUser.calculateSportGPA();
    std::cout << "基础用户测试:" << std::endl;
    printUserInfo(&baseUser);
    // 测试举重运动员
    Powerlifting_users powerlifter("李四", 28, true, 85.0, 180.0);
    powerlifter.setBasicData(
        4000,   // 肺活量
        7.2,    // 50米
        12.0,   // 体前屈
        2.3,    // 立定跳远
        15,     // 引体向上
        4,      // 1000米跑(分)
        30,     // 1000米跑(秒)
        0       // 仰卧起坐
    );
    powerlifter.ifhasOptionalData(true, true, true, true);
    powerlifter.setOptionalData(12.0, 280.0, 200.0, 320.0);
    powerlifter.calculateSportGPA();
    std::cout << "举重运动员测试:" << std::endl;
    printUserInfo(&powerlifter);
    // 测试自定义用户 - 速度型
    Selfdefined_users speedUser("郑十一", 20, true, 67.0, 173.0, 1, 2, 2, 5);
    speedUser.setBasicData(
        4000,   // 肺活量
        6.5,    // 50米
        13.0,   // 体前屈
        2.6,    // 立定跳远
        12,     // 引体向上
        4,      // 1000米跑(分)
        30,     // 1000米跑(秒)
        0       // 仰卧起坐
    );
    speedUser.ifhasOptionalData(false, false, false, false);
    speedUser.setOptionalData(0, 0, 0, 0);
    speedUser.calculateSportGPA();
    std::cout << "自定义用户(速度型)测试:" << std::endl;
    printUserInfo(&speedUser);
    */
/*
日志
2025 10 30
代码出生
基本功能完善
要求用户手动输入体测分数
2025 11 03
添加了不同用户的专项分化
优化了体测项的算法,用户不再需要手动输入体测分数
*/
