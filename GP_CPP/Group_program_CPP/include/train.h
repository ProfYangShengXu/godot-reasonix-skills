// =====================================================
/*
作者信息：
署名 陈霆钧 124090046
*/
// 功能: 训练计划管理系统的头文件
// 包含: 训练组(set)、动作(actions)、训练计划(train)、日期(date)
// =====================================================

#pragma once  // 防止头文件被重复包含
#include <vector>  // 引入动态数组容器，用于存储多个对象
#include <string>  // 引入字符串类，用于存储文字信息
#include <QJsonObject>  // Qt JSON对象，用于序列化
#include <QJsonArray>   // Qt JSON数组，用于序列化列表
#include "../include/analyseuser.h"  // 引入用户体测分析库，用于计算GPA

// ===== 前向声明 =====
// 提前告诉编译器这些类存在，解决类之间相互引用的问题
class set;      // 训练组类（单次训练的参数）
class actions;  // 动作类（包含多个训练组）
class train;    // 训练计划类（包含多个动作）
class date;     // 日期类（记录某天的训练计划）

// =====================================================
// 类名: set (训练组)
// 功能: 记录单次训练的详细参数
// 示例: 深蹲 100kg × 10次 × 4组，组间休息90秒
// =====================================================
class set
{
private:  // 私有成员变量，只能在类内部访问
    int weight;  // 训练重量（单位：公斤kg）
    int reps;    // 每组重复次数（repetitions的缩写）
    int group;   // 训练组数（做几组）
    int rest;    // 组间休息时间（单位：秒）
    int eval;    // 难度评价（0=轻松, 1=中等, 2=困难）
    
public: 
    std::string set_action_name; // 动作名称
 // 公有成员函数，外部可以调用
    // 构造函数：创建训练组对象时自动调用
    // 参数: w=重量, r=次数, g=组数, re=休息时间, e=难度评价
    set(int w, int r, int g, int re, int e);
    
    // 析构函数：对象销毁时自动调用（这里为空实现）
    ~set();
    
    // 获取器函数：返回训练重量
    int getWeight(){return weight;} 
    
    // 获取器函数：返回每组次数
    int getReps(){return reps;}
    
    // 获取器函数：返回组数
    int getGroup(){return group;}
    
    // 获取器函数：返回休息时间
    int getRest(){return rest;}
    
    // 获取器函数：返回难度评价
    int getEval(){return eval;}
    
    // 序列化：将训练组数据转换为JSON对象
    QJsonObject toJson() const;
    
    // 反序列化：从JSON对象创建训练组
    static set fromJson(const QJsonObject& json);
};

// =====================================================
// 类名: actions (训练动作)
// 功能: 管理某个具体动作的所有训练组
// 示例: "深蹲"动作包含3个训练组 + 教学视频链接
// =====================================================
class actions
{
private:  // 私有成员变量
    double difficulty_gpa;  // 该动作的难度GPA（0-4分制，根据用户水平计算）
    std::vector<set> sets;  // 存储多个训练组的数组（vector是动态数组）
   
    
public:  
     std::string action_name;  // 动作名称（如"深蹲"、"卧推"）
     std::vector<std::string> tutorials_links;  // 教学视频链接列表
// 公有成员函数

    // 添加一个训练组到这个动作中
    // 参数: s = 要添加的训练组对象
    void add_set(set s);
    
    // 根据用户的体测GPA计算该动作的难度GPA
    // 参数: user = 指向用户对象的指针
    // 说明: 难度 = 用户GPA ± 0.1（根据eval评价调整）
    void calculate_difficulty_gpa(users* user);
    
    // 删除指定位置的训练组
    // 参数: index = 要删除的训练组索引（从0开始）
    void delete_set(int index);
    
    // 修改指定位置的训练组
    // 参数: index = 要修改的位置, s = 新的训练组对象
    void modify_set(int index, set s);
    
    // 添加一个教学视频链接
    // 参数: link = 视频链接地址（如YouTube链接）
    void add_tutorial_link(std::string link);
    
    // 删除指定位置的教学视频链接
    // 参数: index = 要删除的链接索引
    void delete_tutorial_link(int index);
    
    // 获取动作名称
    std::string getActionName(){return action_name;}
    
    // 获取所有训练组的列表
    std::vector<set> getSets(){return sets;}
    
    // 获取所有教学视频链接的列表
    std::vector<std::string> getTutorialLinks(){return tutorials_links;}
    
    // 获取该动作的难度GPA
    double getDifficultyGPA(){return difficulty_gpa;}
    
    // 序列化：将动作数据转换为JSON对象
    QJsonObject toJson() const;
    
    // 反序列化：从JSON对象创建动作
    static actions fromJson(const QJsonObject& json);
    
    // 构造函数：创建动作对象
    // 参数: name = 动作名称
    actions(std::string name);
    
    // 默认构造函数（用于反序列化）
    actions() : action_name(""), difficulty_gpa(0) {}
    
    // 析构函数：对象销毁时自动调用
    ~actions();
};

// =====================================================
// 类名: train (训练计划)
// 功能: 组合多个动作，形成完整的训练计划
// 示例: "腿部训练日"包含深蹲、硬拉、腿举等多个动作
// =====================================================
class train
{
private:  // 私有成员变量
    double total_difficulty_gpa;  // 整个训练计划的总体难度GPA
    std::vector<actions> action_list;  // 存储多个动作的数组
    std::string train_name = "默认训练计划";  // 训练计划名称（默认值）
    
public:  // 公有成员函数
    // 获取训练计划的总体难度GPA
    double getTotalDifficultyGPA() const {return total_difficulty_gpa;}
    
    // 获取训练计划名称
    std::string getTrainName() const {return train_name;}
    
    // 设置训练计划名称
    // 参数: name = 新的训练计划名称
    void setTrainName(std::string name){train_name = name;}
    
    // 计算整个训练计划的总体难度GPA
    // 参数: user = 用户对象指针，用于计算每个动作的难度
    // 说明: 
    //   - 计算 total_difficulty_gpa = 所有动作难度的平均值
    //   - 计算并设置 user->guess_gpa = 预测完成训练后的运动GPA
    void calculate_total_difficulty_gpa(users* user);
    
    // 添加一个动作到训练计划中
    // 参数: a = 要添加的动作对象
    void add_action(actions a);
    
    // 删除指定位置的动作
    // 参数: index = 要删除的动作索引
    void delete_action(int index);
    
    // 修改指定位置的动作
    // 参数: index = 要修改的位置, a = 新的动作对象
    void modify_action(int index, actions a);
    
    // 获取所有动作的列表
    std::vector<actions> getActionList(){return action_list;}
    
    // 序列化：将训练计划数据转换为JSON对象
    QJsonObject toJson() const;
    
    // 反序列化：从JSON对象创建训练计划
    static train fromJson(const QJsonObject& json);
    
    // 构造函数：创建训练计划对象
    train();
    
    // 析构函数：对象销毁时自动调用
    ~train();
};

// =====================================================
// 类名: date (日期)
// 功能: 记录某一天的训练计划安排
// 示例: 2025年11月11日 → 腿部训练日
// =====================================================
class date
{
public:  // 所有成员都是公有的，可以直接访问
    int year;   // 年份（如：2025）
    int month;  // 月份（1-12）
    int day;    // 日期（1-31）
    
    // 构造函数：创建日期对象
    // 参数: y=年, m=月, d=日
    date(int y, int m, int d);
    
    // 析构函数：对象销毁时自动调用
    ~date();
    
    // 该日期安排的训练计划列表（一天可以安排多个训练计划）
    std::vector<train> train_list;
    
    // 为这一天添加一个训练计划
    // 参数: t = 要添加的训练计划对象
    void add_train(train t);
    
    // 删除指定位置的训练计划
    // 参数: index = 要删除的训练计划索引
    void delete_train(int index);
    
    // 修改指定位置的训练计划
    // 参数: index = 要修改的位置, t = 新的训练计划对象
    void modify_train(int index, train t);
    
    // 序列化：将日期数据转换为JSON对象
    QJsonObject toJson() const;
    
    // 反序列化：从JSON对象创建日期
    static date fromJson(const QJsonObject& json);
};
