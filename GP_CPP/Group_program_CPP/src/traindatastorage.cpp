// =====================================================
// 文件名: traindatastorage.cpp
// 功能: 训练数据存储管理实现
// =====================================================

#include "../include/traindatastorage.h"
#include <QDir>
#include <QStandardPaths>
#include <QDebug>

// 获取默认数据目录
QString TrainDataStorage::getDefaultDataDir()
{
    // 使用Qt标准路径获取应用数据目录
    QString dataDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    
    // 如果目录不存在，创建它
    QDir dir;
    if (!dir.exists(dataDir)) {
        dir.mkpath(dataDir);
    }
    
    return dataDir;
}

// 获取日历数据文件路径
QString TrainDataStorage::getCalendarFilePath()
{
    return getDefaultDataDir() + "/calendar.json";
}

// 获取训练计划数据文件路径
QString TrainDataStorage::getTrainPlansFilePath()
{
    return getDefaultDataDir() + "/trainplans.json";
}

// 保存日期列表到JSON文件
bool TrainDataStorage::saveCalendar(const std::vector<date>& calendar, const QString& filePath)
{
    QJsonArray calendarArray;
    
    // 将所有日期对象转换为JSON
    for (const auto& d : calendar) {
        calendarArray.append(d.toJson());
    }
    
    // 创建JSON文档
    QJsonDocument doc(calendarArray);
    
    // 写入文件
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "无法打开文件保存:" << filePath;
        return false;
    }
    
    file.write(doc.toJson());
    file.close();
    
    qDebug() << "日历数据已保存到:" << filePath;
    return true;
}

// 从JSON文件加载日期列表
std::vector<date> TrainDataStorage::loadCalendar(const QString& filePath)
{
    std::vector<date> calendar;
    
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "无法打开文件读取:" << filePath;
        return calendar;
    }
    
    // 读取JSON数据
    QByteArray data = file.readAll();
    file.close();
    
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isArray()) {
        qDebug() << "JSON格式错误：不是数组";
        return calendar;
    }
    
    // 解析每个日期对象
    QJsonArray calendarArray = doc.array();
    for (const auto& dateJson : calendarArray) {
        calendar.push_back(date::fromJson(dateJson.toObject()));
    }
    
    qDebug() << "从文件加载了" << calendar.size() << "个日期数据:" << filePath;
    return calendar;
}

// 保存训练计划列表到JSON文件
bool TrainDataStorage::saveTrainPlans(const std::vector<train>& trainPlans, const QString& filePath)
{
    QJsonArray trainPlansArray;
    
    // 将所有训练计划对象转换为JSON
    for (const auto& t : trainPlans) {
        trainPlansArray.append(t.toJson());
    }
    
    // 创建JSON文档
    QJsonDocument doc(trainPlansArray);
    
    // 写入文件
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "无法打开文件保存:" << filePath;
        return false;
    }
    
    file.write(doc.toJson());
    file.close();
    
    qDebug() << "训练计划数据已保存到:" << filePath;
    return true;
}

// 从JSON文件加载训练计划列表
std::vector<train> TrainDataStorage::loadTrainPlans(const QString& filePath)
{
    std::vector<train> trainPlans;
    
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "无法打开文件读取:" << filePath;
        return trainPlans;
    }
    
    // 读取JSON数据
    QByteArray data = file.readAll();
    file.close();
    
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isArray()) {
        qDebug() << "JSON格式错误：不是数组";
        return trainPlans;
    }
    
    // 解析每个训练计划对象
    QJsonArray trainPlansArray = doc.array();
    for (const auto& trainJson : trainPlansArray) {
        trainPlans.push_back(train::fromJson(trainJson.toObject()));
    }
    
    qDebug() << "从文件加载了" << trainPlans.size() << "个训练计划:" << filePath;
    return trainPlans;
}
