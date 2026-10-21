// =====================================================
// 文件名: analyse.cpp
// 功能: 用户分析系统的实现
// 作者: 陈霆钧 124090046
// 说明: 实现不同类型用户的运动GPA计算逻辑
// =====================================================

#include "../include/analyseuser.h"

// =====================================================
// users类：用户基类
// =====================================================

// 构造函数：初始化用户基本信息
// @param n: 姓名
// @param a: 年龄
// @param g: 性别（true=男，false=女）
// @param bw: 体重（kg）
// @param h: 身高（cm）
users::users(std::string n, int a, bool g, double bw, double h){ 
    name = n;          // 设置姓名
    age = a;           // 设置年龄
    gender = g;        // 设置性别
    bodyweight = bw;   // 设置体重
    height = h;        // 设置身高
    guess_gpa = 0;     // 初始化预测GPA为0
}

// =====================================================
// 设置基础数据
// =====================================================
// 功能：输入体测数据并转换为百分制分数
// @param lc: 肺活量（毫升）
// @param r50m: 50米跑成绩（秒）
// @param sar: 坐位体前屈（厘米）
// @param sj: 立定跳远（米）
// @param pu: 引体向上次数（男生）
// @param mlr: 长跑成绩-分钟部分
// @param slr: 长跑成绩-秒钟部分
// @param su: 仰卧起坐次数（女生）
void users::setBasicData(double lc, double r50m, double sar,
         double sj, double pu, double mlr, double slr, double su) { 
    // 将长跑成绩转换为总秒数
    int lr = mlr * 60 + slr;
    
    // 根据性别使用不同的评分标准
    if(gender) {  // 男生
        // 肺活量评分公式
        lungCapacity = 12.5 + lc * 7.0/200.0;
        
        // 50米跑评分公式（时间越短分数越高）
        run50m = 205 - 17.5 * r50m;
        
        // 坐位体前屈评分公式
        sitAndReach = 125/3 + sar * 7.0/3.0;
        
        // 立定跳远评分公式
        standingJump = -89 + sj * 70;
        
        // 引体向上评分公式
        pullUps = 160.0/3.0 + 7/3 * pu;
        
        // 男生不做仰卧起坐
        sitUps = 0;
        
        // 长跑（1000米）评分公式
        longRun = 685.0/3.0 - (7.0 / 12.0) * lr;
    } else {  // 女生
        // 肺活量评分公式
        lungCapacity = 55.0/3.0 + lc * 7.0/300.0;
        
        // 50米跑评分公式
        run50m = 445.0/2.0 - 17.5 * r50m;
        
        // 坐位体前屈评分公式
        sitAndReach = 125.0/3 + sar * 35/18;
        
        // 立定跳远评分公式
        standingJump = -75.0 + sj * 87.5;
        
        // 女生不做引体向上
        pullUps = 0;
        
        // 仰卧起坐评分公式
        sitUps = 30 + 7.0 / 5.0* su;
        
        // 长跑（800米）评分公式
        longRun = 650/3 - (7.0 / 12.0) * lr;
    }   
}

// =====================================================
// 设置可选数据
// =====================================================
// 功能：设置体脂率和力量训练相关数据
// @param bf: 体脂率（%）
// @param sr: 深蹲重量（kg）
// @param bpr: 卧推重量（kg）
// @param dr: 硬拉重量（kg）
void users::setOptionalData(double bf, double sr, double bpr, double dr) {
    // 体脂率评分
    if (hasBodyFat) { 
        if (gender) {  // 男生标准
            bodyFat = 135 - 3.5 * bf;
        } else {       // 女生标准
            bodyFat = 170 - 3.5 * bf;
        }
    } else {
        // 如果没有体脂率数据，使用BMI估算
        double bmi = bodyweight / ((height/100) * (height/100));
        bodyFat = 217.64 - 6.36 * bmi;
    }
    
    // 深蹲评分（使用GL公式）
    if (hasSquat) {
        squatRatio = 65 + 2.28765 * (Glpointsq(sr, bodyweight, gender) - 9.07);
    }
    
    // 硬拉评分（使用GL公式）
    if (hasDeadlift) {
        deadliftRatio = 65 + (Glpointdp(dr, bodyweight, gender) - 12.09) * 1.71654;
    }
    
    // 卧推评分（使用GL公式）
    if (hasBenchPress) {
        benchPressRatio = 65 + (Glpointbp(bpr, bodyweight, gender) - 6.05) * 3.42936;
    }
}

// =====================================================
// 设置可选数据标志
// =====================================================
// 功能：标记哪些可选项目用户填写了
// @param hb: 是否有体脂率数据
// @param hs: 是否有深蹲数据
// @param hbp: 是否有卧推数据
// @param hd: 是否有硬拉数据
void users::ifhasOptionalData(bool hb, bool hs, bool hbp, bool hd) {
    hasBodyFat = hb;
    hasSquat = hs;
    hasBenchPress = hbp;
    hasDeadlift = hd;
}

// =====================================================
// Regular_users类：普通用户
// =====================================================

// 计算运动GPA（普通用户标准）
// 功能：按标准权重计算各项目得分的GPA
void Regular_users::calculateSportGPA() {
    // 统计缺失的可选项数量
    int optionalCount = 0;
    if (hasBodyFat == 0) optionalCount++;
    if (hasSquat ==0 ) optionalCount++;
    if (hasBenchPress == 0) optionalCount++;
    if (hasDeadlift == 0) optionalCount++;
    
    // 计算除数（缺失项会减少总权重）
    divisor = 14 - 2 * optionalCount;
    
    // 将各项百分制分数转换为GPA分数
    double lc = PersentageToGpa(lungCapacity);      // 肺活量
    double r50m = PersentageToGpa(run50m);          // 50米跑
    double sar = PersentageToGpa(sitAndReach);      // 坐位体前屈
    double sj = PersentageToGpa(standingJump);      // 立定跳远
    double pu = PersentageToGpa(pullUps);           // 引体向上
    double su = PersentageToGpa(sitUps);            // 仰卧起坐
    double lr = PersentageToGpa(longRun);           // 长跑
    double bf = hasBodyFat ? PersentageToGpa(bodyFat) : 0;                 // 体脂率
    double sr = hasSquat ? PersentageToGpa(squatRatio) : 0;               // 深蹲
    double bpr = hasBenchPress ? PersentageToGpa(benchPressRatio) : 0;    // 卧推
    double dr = hasDeadlift ? PersentageToGpa(deadliftRatio) : 0;         // 硬拉
    
    // 计算加权平均GPA（可选项权重为2）
    sport_gpa = (lc + r50m + sar + sj + pu + su + lr + 2 * (bf + sr + bpr + dr)) / divisor;
}

// =====================================================
// 获取运动GPA
// =====================================================
// @return: 当前用户的运动GPA值
double users::getSportGPA() const {
    return sport_gpa;
}

// =====================================================
// 获取预测运动GPA
// =====================================================
// @return: 用户的预测GPA值
double users::getGuessGPA() const {
    return guess_gpa;
}

// =====================================================
// 设置预测运动GPA
// =====================================================
// @param gpa: 要设置的预测GPA值
void users::setGuessGPA(double gpa) {
    guess_gpa = gpa;
}

// =====================================================
// Powerlifting_users类：举重运动员
// =====================================================

// 计算运动GPA（举重运动员标准）
// 功能：加大力量训练项目的权重（5倍）
void Powerlifting_users::calculateSportGPA() {
    // 统计缺失的可选项数量
    int optionalCount = 0;
    if (hasBodyFat == 0) optionalCount++;
    if (hasSquat ==0 ) optionalCount++;
    if (hasBenchPress == 0) optionalCount++;
    if (hasDeadlift == 0) optionalCount++;
    
    // 计算除数
    divisor = 14 - 2 * optionalCount;
    
    // 将各项百分制分数转换为GPA分数
    double lc = PersentageToGpa(lungCapacity);
    double r50m = PersentageToGpa(run50m);
    double sar = PersentageToGpa(sitAndReach);
    double sj = PersentageToGpa(standingJump);
    double pu = PersentageToGpa(pullUps);
    double su = PersentageToGpa(sitUps);
    double lr = PersentageToGpa(longRun);
    double bf = hasBodyFat ? PersentageToGpa(bodyFat) : 0;
    double sr = hasSquat ? PersentageToGpa(squatRatio) : 0;
    double bpr = hasBenchPress ? PersentageToGpa(benchPressRatio) : 0;
    double dr = hasDeadlift ? PersentageToGpa(deadliftRatio) : 0;
    
    // 计算GPA：力量项目（深蹲、卧推、硬拉）权重为5倍
    sport_gpa = (lc + r50m + sar + sj + pu + su + lr + bf + 5*(sr + bpr + dr)) / (divisor + 8);
}

// =====================================================
// Longruning_users类：长跑运动员
// =====================================================

// 计算运动GPA（长跑运动员标准）
// 功能：加大肺活量和长跑项目的权重（5倍）
void Longruning_users::calculateSportGPA() {
    // 统计缺失的可选项数量
    int optionalCount = 0;
    if (hasBodyFat == 0) optionalCount++;
    if (hasSquat ==0 ) optionalCount++;
    if (hasBenchPress == 0) optionalCount++;
    if (hasDeadlift == 0) optionalCount++;
    
    // 计算除数
    divisor = 14 - 2 * optionalCount;
    
    // 将各项百分制分数转换为GPA分数
    double lc = PersentageToGpa(lungCapacity);
    double r50m = PersentageToGpa(run50m);
    double sar = PersentageToGpa(sitAndReach);
    double sj = PersentageToGpa(standingJump);
    double pu = PersentageToGpa(pullUps);
    double su = PersentageToGpa(sitUps);
    double lr = PersentageToGpa(longRun);
    double bf = hasBodyFat ? PersentageToGpa(bodyFat) : 0;
    double sr = hasSquat ? PersentageToGpa(squatRatio) : 0;
    double bpr = hasBenchPress ? PersentageToGpa(benchPressRatio) : 0;
    double dr = hasDeadlift ? PersentageToGpa(deadliftRatio) : 0;
    
    // 计算GPA：肺活量和长跑权重为5倍
    sport_gpa = (5*lc + r50m + sar + sj + pu + su + 5*lr + 2*(bf + sr + bpr + dr)) / (divisor + 8);
}

// =====================================================
// Shortrunning_users类：短跑运动员
// =====================================================

// 计算运动GPA（短跑运动员标准）
// 功能：加大50米跑和立定跳远项目的权重（5倍）
void Shortrunning_users::calculateSportGPA() {
    // 统计缺失的可选项数量
    int optionalCount = 0;
    if (hasBodyFat == 0) optionalCount++;
    if (hasSquat ==0 ) optionalCount++;
    if (hasBenchPress == 0) optionalCount++;
    if (hasDeadlift == 0) optionalCount++;
    
    // 计算除数
    divisor = 14 - 2 * optionalCount;
    
    // 将各项百分制分数转换为GPA分数
    double lc = PersentageToGpa(lungCapacity);
    double r50m = PersentageToGpa(run50m);
    double sar = PersentageToGpa(sitAndReach);
    double sj = PersentageToGpa(standingJump);
    double pu = PersentageToGpa(pullUps);
    double su = PersentageToGpa(sitUps);
    double lr = PersentageToGpa(longRun);
    double bf = hasBodyFat ? PersentageToGpa(bodyFat) : 0;
    double sr = hasSquat ? PersentageToGpa(squatRatio) : 0;
    double bpr = hasBenchPress ? PersentageToGpa(benchPressRatio) : 0;
    double dr = hasDeadlift ? PersentageToGpa(deadliftRatio) : 0;
    
    // 计算GPA：50米跑和立定跳远权重为5倍
    sport_gpa = (lc + 5 * r50m + sar + 5 * sj + pu + su + lr + 2*(bf + sr + bpr + dr)) / (divisor + 8);
}

// =====================================================
// Bodybuilding_users类：健美运动员
// =====================================================

// 计算运动GPA（健美运动员标准）
// 功能：加大可选项目（体脂、力量）的权重（4倍）
void Bodybuilding_users::calculateSportGPA() {
    // 统计缺失的可选项数量
    int optionalCount = 0;
    if (hasBodyFat == 0) optionalCount++;
    if (hasSquat ==0 ) optionalCount++;
    if (hasBenchPress == 0) optionalCount++;
    if (hasDeadlift == 0) optionalCount++;
    
    // 计算除数
    divisor = 14 - 2 * optionalCount;
    
    // 将各项百分制分数转换为GPA分数
    double lc = PersentageToGpa(lungCapacity);
    double r50m = PersentageToGpa(run50m);
    double sar = PersentageToGpa(sitAndReach);
    double sj = PersentageToGpa(standingJump);
    double pu = PersentageToGpa(pullUps);
    double su = PersentageToGpa(sitUps);
    double lr = PersentageToGpa(longRun);
    double bf = hasBodyFat ? PersentageToGpa(bodyFat) : 0;
    double sr = hasSquat ? PersentageToGpa(squatRatio) : 0;
    double bpr = hasBenchPress ? PersentageToGpa(benchPressRatio) : 0;
    double dr = hasDeadlift ? PersentageToGpa(deadliftRatio) : 0;
    
    // 计算GPA：体脂和力量项目权重为4倍
    sport_gpa = (lc + r50m + sar + sj + pu + su + lr + 4*(bf + sr + bpr + dr)) / (divisor + 8);
}

// =====================================================
// Selfdefined_users类：自定义用户
// =====================================================

// 构造函数：初始化自定义权重
// @param n: 姓名
// @param a: 年龄
// @param g: 性别
// @param bw: 体重
// @param h: 身高
// @param sw: 力量权重
// @param ew: 耐力权重
// @param fw: 柔韧性权重
// @param spw: 速度权重
Selfdefined_users::Selfdefined_users(std::string n, int a, bool g, double bw, double h,
    int sw, int ew, int fw,  int spw)
    : users(n, a, g, bw, h), strenggthWeight(sw), enduranceWeight(ew),
      flexibilityWeight(fw), speedWeight(spw) {}

// 计算运动GPA（自定义标准）
// 功能：根据用户设定的权重计算GPA
void Selfdefined_users::calculateSportGPA() {
    // 统计缺失的可选项数量
    int optionalCount = 0;
    if (hasBodyFat == 0) optionalCount++;
    if (hasSquat ==0 ) optionalCount++;
    if (hasBenchPress == 0) optionalCount++;
    if (hasDeadlift == 0) optionalCount++;
    
    // 计算除数（根据用户自定义权重）
    divisor = hasBodyFat ? strenggthWeight * 3 + enduranceWeight * 2 + flexibilityWeight
    + speedWeight * 2 + 4 - strenggthWeight * optionalCount  : strenggthWeight * 3 + enduranceWeight * 2 + flexibilityWeight
    + speedWeight * 2 + 4 - strenggthWeight * (optionalCount-1) - 2;
    
    // 将各项百分制分数转换为GPA分数
    double lc = PersentageToGpa(lungCapacity);
    double r50m = PersentageToGpa(run50m);
    double sar = PersentageToGpa(sitAndReach);
    double sj = PersentageToGpa(standingJump);
    double pu = PersentageToGpa(pullUps);
    double su = PersentageToGpa(sitUps);
    double lr = PersentageToGpa(longRun);
    double bf = hasBodyFat ? PersentageToGpa(bodyFat) : 0;
    double sr = hasSquat ? PersentageToGpa(squatRatio) : 0;
    double bpr = hasBenchPress ? PersentageToGpa(benchPressRatio) : 0;
    double dr = hasDeadlift ? PersentageToGpa(deadliftRatio) : 0;
    
    // 计算GPA：使用自定义权重
    // 力量 = 深蹲 + 卧推 + 硬拉
    // 耐力 = 肺活量 + 长跑
    // 柔韧性 = 坐位体前屈
    // 速度 = 50米跑 + 立定跳远
    sport_gpa = (strenggthWeight * (sr + bpr + dr) +
                 enduranceWeight * (lc + lr) +
                 flexibilityWeight * (sar) +
                 speedWeight * (r50m + sj) +
                 pu + su + bf * 2) / divisor;
}

// =====================================================
// 全局函数：百分制转GPA
// =====================================================
// 功能：将百分制分数转换为4.0制GPA
// @param percentage: 百分制分数（0-100）
// @return: GPA分数（0.0-4.0）
double PersentageToGpa(double percentage) {
    if (percentage >= 90.0)
        return 4.0;                                    // 优秀：4.0
    else if (percentage >= 65.0)
        return 3.0 + (percentage - 65.0) / 35.0;      // 良好：3.0-4.0
    else if (percentage >= 55.0)
        return 2.0 + (percentage - 55.0) / 10.0;      // 中等：2.0-3.0
    else if (percentage >= 45.0)
        return 1.0 + (percentage - 45.0) / 10.0;      // 及格：1.0-2.0
    else
        return 0.0;                                    // 不及格：0.0
}

// =====================================================
// 全局函数：深蹲GL Points计算
// =====================================================
// 功能：使用GL公式计算深蹲的相对力量分数
// @param sq: 深蹲重量（kg）
// @param bodyweight: 体重（kg）
// @param gender: 性别（true=男，false=女）
// @return: GL Points分数
double Glpointsq(double sq, double bodyweight, bool gender) {
    // GL公式参数（根据性别选择）
    double A = gender ? 1199.72839 : 610.32796;
    double B = gender ? 1025.18162 : 1045.59282;
    double C = gender ? 0.00921 : 0.03048;
    
    // GL公式：100 * 重量 / (A - B * e^(-C * 体重))
    return 100.0 * sq / (A - B * std::exp(-C * bodyweight));
}

// =====================================================
// 全局函数：卧推GL Points计算
// =====================================================
// 功能：使用GL公式计算卧推的相对力量分数
// @param bp: 卧推重量（kg）
// @param bodyweight: 体重（kg）
// @param gender: 性别（true=男，false=女）
// @return: GL Points分数
double Glpointbp(double bp, double bodyweight, bool gender) {
    // GL公式参数（根据性别选择）
    double A = gender ? 320.98041 : 142.40398;
    double B = gender ? 281.40258 : 442.52671;
    double C = gender ? 0.01008 : 0.04724;
    
    // GL公式
    return 100.0 * bp / (A - B * std::exp(-C * bodyweight));
}

// =====================================================
// 全局函数：硬拉GL Points计算
// =====================================================
// 功能：使用GL公式计算硬拉的相对力量分数
// @param dp: 硬拉重量（kg）
// @param bodyweight: 体重（kg）
// @param gender: 性别（true=男，false=女）
// @return: GL Points分数
double Glpointdp(double dp, double bodyweight, bool gender) {
    // GL公式参数（根据性别选择）
    double A = gender ? 1199.72839 : 610.32796;
    double B = gender ? 1025.18162 : 1045.59282;
    double C = gender ? 0.00921 : 0.03048;
    
    // GL公式
    return 100.0 * dp / (A - B * std::exp(-C * bodyweight));
}

