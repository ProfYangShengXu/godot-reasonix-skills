// =====================================================
/*
作者信息：
署名 陈霆钧 124090046
*/
// 功能: 训练计划管理系统的实现文件
// 说明: 包含所有类的成员函数的具体实现代码
// =====================================================

#include "../include/train.h"        // 引入训练系统头文件
#include "../include/analyseuser.h"  // 引入用户分析系统头文件
#include <cmath>    // 数学函数库（本文件暂未使用）
#include <vector>   // 动态数组容器
#include <string>   // 字符串类
#include <QJsonObject>  // Qt JSON对象
#include <QJsonArray>   // Qt JSON数组

// =====================================================
// set类的实现（训练组）
// =====================================================

// 构造函数：创建训练组对象时初始化所有成员变量
// 参数说明:
//   w  = weight (重量，单位kg)
//   r  = reps (每组重复次数)
//   g  = group (训练组数)
//   re = rest (组间休息时间，单位秒)
//   e  = eval (难度评价: 0轻松/1中等/2困难)
set::set(int w, int r, int g, int re, int e)
{
    weight = w;   // 将参数w赋值给成员变量weight
    reps = r;     // 将参数r赋值给成员变量reps
    group = g;    // 将参数g赋值给成员变量group
    rest = re;    // 将参数re赋值给成员变量rest
    eval = e;     // 将参数e赋值给成员变量eval
}

// 析构函数：对象销毁时调用（这里没有需要清理的资源，所以为空）
set::~set()
{
}

// 序列化：将训练组数据转换为JSON对象
QJsonObject set::toJson() const
{
    QJsonObject json;
    json["weight"] = weight;
    json["reps"] = reps;
    json["group"] = group;
    json["rest"] = rest;
    json["eval"] = eval;
    json["action_name"] = QString::fromStdString(set_action_name);
    return json;
}

// 反序列化：从JSON对象创建训练组
set set::fromJson(const QJsonObject& json)
{
    set s(
        json["weight"].toInt(),
        json["reps"].toInt(),
        json["group"].toInt(),
        json["rest"].toInt(),
        json["eval"].toInt()
    );
    s.set_action_name = json["action_name"].toString().toStdString();
    return s;
}

// =====================================================
// actions类的实现（训练动作）
// =====================================================

// 构造函数：创建动作对象时设置动作名称
// 参数: name = 动作名称（如"深蹲"、"卧推"）
actions::actions(std::string name)
{
    action_name = name;  // 保存动作名称
}

// 析构函数：对象销毁时调用（无需特殊清理）
actions::~actions()
{   
}

// 功能：向该动作添加一个训练组
// 参数: x = 要添加的训练组对象
// 说明: 使用vector的push_back方法将新训练组添加到数组末尾
void actions::add_set(set x)
{
   if (sets.size()==0)
   {
         sets.reserve(4);  // 预留空间以存储多个训练组，提升性能
   }
   std::string name = this->action_name; // 获取当前动作名称
   x.set_action_name = name; // 设置训练组的动作名称为当前动作名称
   sets.emplace_back(x);  // 将训练组x添加到sets数组的末尾
}

// 功能：删除指定位置的训练组
// 参数: index = 要删除的训练组在数组中的位置（从0开始）
// 安全检查: 如果索引超出范围，则不执行任何操作
void actions::delete_set(int index)
{
    // 检查索引是否有效：index不能小于0，也不能大于等于数组大小
    if (index >= 0 && index < (int)sets.size()) {
        // 使用erase方法删除指定位置的元素
        // sets.begin() 返回数组第一个元素的位置
        // sets.begin() + index 移动到要删除的元素位置
        sets.erase(sets.begin() + index);
    }
}

// 功能：修改指定位置的训练组
// 参数: index = 要修改的位置, s = 新的训练组对象
// 安全检查: 如果索引超出范围，则不执行任何操作
void actions::modify_set(int index, set s)
{
    // 检查索引是否有效
    if (index < 0 || index >= sets.size()) return;
    
    // 将指定位置的训练组替换为新的训练组
    sets[index] = s;
}
// 功能：根据用户的体测GPA计算该动作的难度GPA
// 参数: user = 指向用户对象的指针（通过指针访问用户的GPA数据）
// 算法说明:
//   1. 遍历该动作的所有训练组
//   2. 根据每组的难度评价(eval)调整GPA:
//      - eval=0(轻松): GPA = 用户GPA - 0.1
//      - eval=1(中等): GPA = 用户GPA
//      - eval=2(困难): GPA = 用户GPA + 0.1
//   3. 计算所有训练组的平均难度GPA
void actions::calculate_difficulty_gpa(users* user)
{
    difficulty_gpa = 0;  // 初始化难度GPA为0
    
    // 遍历所有训练组（i从0到训练组数量-1）
    for (int i = 0; i < sets.size(); i++) 
    {
       double count = 0;  // 临时变量，存储当前训练组的难度GPA
       
       // 根据训练组的难度评价计算GPA
       if (sets[i].getEval() == 0)  // 如果是轻松难度
       {
          // 难度GPA = 用户基础GPA （因为太轻松了）
          count = user->getSportGPA();
       }
       else if (sets[i].getEval() == 1)  // 如果是中等难度
       {
          // 难度GPA = 用户基础GPA+0.1（刚好合适）
          count = user->getSportGPA()+0.1;
       }
       else if (sets[i].getEval() == 2)  // 如果是困难难度
       {
          // 难度GPA = 用户基础GPA + 0.15（有挑战性）
          count = user->getSportGPA() + 0.15;
       }
       
       // 累加每组的难度，最后除以总组数得到平均值
       // 注意：这里每次累加时就除以总数，最终效果等同于求和后除以总数
       difficulty_gpa += count / double(sets.size());
    }
}

// 功能：添加一个教学视频链接
// 参数: link = 视频链接地址（字符串格式）
void actions::add_tutorial_link(std::string link)
{
    if (tutorials_links.size()==0)
   {
         tutorials_links.reserve(4);  // 预留空间以存储多个训练组，提升性能
   }
    tutorials_links.emplace_back(link);  // 将链接添加到链接数组的末尾
}

// 功能：删除指定位置的教学视频链接
// 参数: index = 要删除的链接在数组中的位置
void actions::delete_tutorial_link(int index)
{
    // 检查索引是否有效
    if (index >= 0 && index < (int)tutorials_links.size()) {
        // 删除指定位置的链接
        tutorials_links.erase(tutorials_links.begin() + index);
    }
}

// 序列化：将动作数据转换为JSON对象
QJsonObject actions::toJson() const
{
    QJsonObject json;
    json["action_name"] = QString::fromStdString(action_name);
    json["difficulty_gpa"] = difficulty_gpa;
    
    // 序列化训练组列表
    QJsonArray setsArray;
    for (const auto& s : sets) {
        setsArray.append(s.toJson());
    }
    json["sets"] = setsArray;
    
    // 序列化教学链接列表
    QJsonArray linksArray;
    for (const auto& link : tutorials_links) {
        linksArray.append(QString::fromStdString(link));
    }
    json["tutorials_links"] = linksArray;
    
    return json;
}

// 反序列化：从JSON对象创建动作
actions actions::fromJson(const QJsonObject& json)
{
    actions a(json["action_name"].toString().toStdString());
    a.difficulty_gpa = json["difficulty_gpa"].toDouble();
    
    // 反序列化训练组列表
    QJsonArray setsArray = json["sets"].toArray();
    for (const auto& setJson : setsArray) {
        a.sets.push_back(set::fromJson(setJson.toObject()));
    }
    
    // 反序列化教学链接列表
    QJsonArray linksArray = json["tutorials_links"].toArray();
    for (const auto& linkJson : linksArray) {
        a.tutorials_links.push_back(linkJson.toString().toStdString());
    }
    
    return a;
}

// =====================================================
// train类的实现（训练计划）
// =====================================================

// 构造函数：创建训练计划对象（无需特殊初始化）
train::train()
{
}

// 析构函数：对象销毁时调用（无需特殊清理）
train::~train()
{
}

// 功能：向训练计划中添加一个动作
// 参数: a = 要添加的动作对象
void train::add_action(actions a)
{
     if (action_list.size()==0)
   {
         action_list.reserve(4);  // 预留空间以存储多个训练组，提升性能
   }
    action_list.emplace_back(a);  // 将动作添加到动作列表的末尾
}

// 功能：删除指定位置的动作
// 参数: index = 要删除的动作在列表中的位置
void train::delete_action(int index)
{
    // 检查索引是否有效
    if (index >= 0 && index < (int)action_list.size()){
        action_list.erase(action_list.begin() + index);  // 删除指定位置的动作
    }
}

// 功能：修改指定位置的动作
// 参数: index = 要修改的位置, a = 新的动作对象
void train::modify_action(int index, actions a)
{
    // 检查索引是否有效
    if (index >= 0 && index < action_list.size()){
        action_list[index] = a;  // 将指定位置的动作替换为新动作
    }
}  

// 功能：计算整个训练计划的总体难度GPA和预测GPA
// 参数: user = 用户对象指针，用于计算每个动作的难度
// 算法：
//   1. 首先为每个动作计算难度GPA（基于用户的运动GPA）
//   2. 然后遍历所有动作，计算它们的平均难度GPA
//   3. 基于训练难度预测完成后的运动GPA提升，并设置到user对象中
void train::calculate_total_difficulty_gpa(users* user)
{
    total_difficulty_gpa = 0;  // 初始化总难度为0
    
    // 如果没有用户对象，无法计算
    if (!user) return;
    
    // 获取用户当前的运动GPA（如果已有预测GPA，则基于预测GPA继续计算）
    double current_gpa;
    if (user->guess_gpa == 0) {
        current_gpa = user->getSportGPA();  // 使用实际运动GPA
    } else {
        current_gpa = user->guess_gpa;  // 使用已预测的GPA
    }
    
    // 遍历训练计划中的所有动作
    for (int i = 0; i < action_list.size(); i++)
    {
        // 先计算每个动作的难度GPA（需要用户对象）
        action_list[i].calculate_difficulty_gpa(user);
        
        // 累加每个动作的难度GPA，并除以动作总数得到平均值
        total_difficulty_gpa += action_list[i].getDifficultyGPA() / double(action_list.size());
    }
    
    // 计算预测GPA：基于训练难度和当前GPA
    // 算法：如果训练难度高于当前GPA，说明有提升空间
    // 预测提升幅度 = (训练难度 - 当前GPA) * 0.3（保守估计30%的提升）
    if (total_difficulty_gpa > current_gpa) {
        double improvement = (total_difficulty_gpa - current_gpa) * 0.3;
        user->guess_gpa = current_gpa + improvement;
        
        // 确保不超过4.0的上限
        if (user->guess_gpa > 4.0) {
            user->guess_gpa = 4.0;
        }
    } else {
        // 如果训练难度不高于当前水平，预测维持当前GPA
        user->guess_gpa = current_gpa;
    }
}

// 序列化：将训练计划数据转换为JSON对象
QJsonObject train::toJson() const
{
    QJsonObject json;
    json["train_name"] = QString::fromStdString(train_name);
    json["total_difficulty_gpa"] = total_difficulty_gpa;
    
    // 序列化动作列表
    QJsonArray actionsArray;
    for (const auto& action : action_list) {
        actionsArray.append(action.toJson());
    }
    json["action_list"] = actionsArray;
    
    return json;
}

// 反序列化：从JSON对象创建训练计划
train train::fromJson(const QJsonObject& json)
{
    train t;
    t.train_name = json["train_name"].toString().toStdString();
    t.total_difficulty_gpa = json["total_difficulty_gpa"].toDouble();
    
    // 反序列化动作列表
    QJsonArray actionsArray = json["action_list"].toArray();
    for (const auto& actionJson : actionsArray) {
        t.action_list.push_back(actions::fromJson(actionJson.toObject()));
    }
    
    return t;
}

// =====================================================
// date类的实现（日期）
// =====================================================

// 构造函数：创建日期对象并初始化年月日
// 参数: y = 年份, m = 月份, d = 日期
date::date(int y, int m, int d)
{
    year = y;   // 设置年份
    month = m;  // 设置月份
    day = d;    // 设置日期
}

// 析构函数：对象销毁时调用（无需特殊清理）
date::~date()
{
}

// 功能：为这一天添加一个训练计划
// 参数: t = 要添加的训练计划对象
void date::add_train(train t)
{
     if (train_list.size()==0)
   {
         train_list.reserve(2);  // 预留空间以存储多个训练组，提升性能
   }
    train_list.emplace_back(t);  // 将训练计划添加到该日期的训练列表末尾
}

// 功能：删除指定位置的训练计划
// 参数: index = 要删除的训练计划在列表中的位置
void date::delete_train(int index)
{
    // 检查索引是否有效
    if (index >= 0 && index < (int)train_list.size()){
        train_list.erase(train_list.begin() + index);  // 删除指定位置的训练计划
    }
}

// 功能：修改指定位置的训练计划
// 参数: index = 要修改的位置, t = 新的训练计划对象
void date::modify_train(int index, train t)
{
    // 检查索引是否有效
    if (index >= 0 && index < (int)train_list.size()){
        train_list[index] = t;  // 将指定位置的训练计划替换为新计划
    }
}

// 序列化：将日期数据转换为JSON对象
QJsonObject date::toJson() const
{
    QJsonObject json;
    json["year"] = year;
    json["month"] = month;
    json["day"] = day;
    
    // 序列化训练计划列表
    QJsonArray trainsArray;
    for (const auto& t : train_list) {
        trainsArray.append(t.toJson());
    }
    json["train_list"] = trainsArray;
    
    return json;
}

// 反序列化：从JSON对象创建日期
date date::fromJson(const QJsonObject& json)
{
    date d(
        json["year"].toInt(),
        json["month"].toInt(),
        json["day"].toInt()
    );
    
    // 反序列化训练计划列表
    QJsonArray trainsArray = json["train_list"].toArray();
    for (const auto& trainJson : trainsArray) {
        d.train_list.push_back(train::fromJson(trainJson.toObject()));
    }
    
    return d;
}