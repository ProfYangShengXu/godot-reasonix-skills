// =====================================================
// 文件名: traindatastorage.h
// 功能: 训练数据存储管理类
// 说明: 提供训练计划数据的保存和加载功能
// =====================================================

#pragma once
#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <vector>
#include "train.h"

// =====================================================
// 类名: TrainDataStorage
// 功能: 管理训练数据的持久化存储
// =====================================================
class TrainDataStorage
{
public:
    // 保存日期列表到JSON文件
    // 参数: calendar = 日期列表, filePath = 保存路径
    // 返回: 是否保存成功
    static bool saveCalendar(const std::vector<date>& calendar, const QString& filePath);
    
    // 从JSON文件加载日期列表
    // 参数: filePath = 文件路径
    // 返回: 日期列表
    static std::vector<date> loadCalendar(const QString& filePath);
    
    // 保存训练计划列表到JSON文件
    // 参数: trainPlans = 训练计划列表, filePath = 保存路径
    // 返回: 是否保存成功
    static bool saveTrainPlans(const std::vector<train>& trainPlans, const QString& filePath);
    
    // 从JSON文件加载训练计划列表
    // 参数: filePath = 文件路径
    // 返回: 训练计划列表
    static std::vector<train> loadTrainPlans(const QString& filePath);
    
    // 获取默认数据目录
    static QString getDefaultDataDir();
    
    // 获取日历数据文件路径
    static QString getCalendarFilePath();
    
    // 获取训练计划数据文件路径
    static QString getTrainPlansFilePath();
};
