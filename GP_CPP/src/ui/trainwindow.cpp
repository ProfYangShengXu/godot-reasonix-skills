// =====================================================
// 文件名: trainwindow.cpp
// 功能: 训练计划管理窗口的实现文件
// 说明: 实现训练计划的增删改查功能
// =====================================================

#include "ui/trainwindow.h"
#include "ui_trainwindow.h"
#include "traindatastorage.h"  // 数据存储管理
#include <QMessageBox>  // 消息提示框
#include <QInputDialog>  // 输入对话框
#include <QDebug>  // 调试输出

// =====================================================
// 构造函数：初始化训练计划窗口
// =====================================================
TrainWindow::TrainWindow(QWidget *parent, users* user)
    : QMainWindow(parent)  // 初始化基类
    , ui(new Ui::TrainWindow)  // 创建UI对象
    , currentUser(user)  // 保存用户对象指针
    , currentDateIndex(-1)  // 初始化索引为-1（表示未选中）
    , currentTrainIndex(-1)
    , currentActionIndex(-1)
    , currentSetIndex(-1)
    , serverConnection(nullptr)  // 初始化服务器连接为空
{
    ui->setupUi(this);  // 设置UI界面
    
    // 设置窗口标题
    setWindowTitle("训练计划管理系统");
    
    // 初始化服务器连接
    initServerConnection();
    
    // 加载保存的数据
    loadAllDataFromServer();
    
    // 刷新训练计划列表
    refreshTrainPlansList();
}

// =====================================================
// 析构函数：清理资源
// =====================================================
TrainWindow::~TrainWindow()
{
    // 保存所有数据到服务器
    saveAllDataToServer();
    
    delete ui;  // 删除UI对象
}

// =====================================================
// 日历点击事件：当用户点击日历上的某一天
// =====================================================
void TrainWindow::on_calendarWidget_clicked(const QDate &date)
{
    // 查找该日期在日历数据中的索引
    currentDateIndex = findDateIndex(date);
    
    // 如果日期不存在，创建新的日期对象
    if (currentDateIndex == -1) {
        // 创建新的date对象
        ::date newDate(date.year(), date.month(), date.day());
        calendar.push_back(newDate);
        currentDateIndex = calendar.size() - 1;
    }
    
    // 刷新该日期的训练计划显示
    refreshDateTrains();
    
    // 更新状态栏提示
    ui->statusbar->showMessage(
        QString("已选择日期: %1").arg(date.toString("yyyy年MM月dd日"))
    );
}

// =====================================================
// 添加训练计划到日期
// =====================================================
void TrainWindow::on_btnAddTrainToDate_clicked()
{
    // 检查是否选择了日期
    if (currentDateIndex == -1) {
        QMessageBox::warning(this, "提示", "请先在日历中选择一个日期！");
        return;
    }
    
    // 检查是否选择了训练计划
    if (currentTrainIndex == -1) {
        QMessageBox::warning(this, "提示", "请先选择一个训练计划！");
        return;
    }
    
    // 将选中的训练计划添加到该日期
    calendar[currentDateIndex].add_train(trainPlans[currentTrainIndex]);
    
    // 刷新显示
    refreshDateTrains();
    
    QMessageBox::information(this, "成功", "训练计划已添加到该日期！");
}

// =====================================================
// 从日期删除训练计划
// =====================================================
void TrainWindow::on_btnDeleteTrainFromDate_clicked()
{
    // 检查是否选择了日期
    if (currentDateIndex == -1) {
        QMessageBox::warning(this, "提示", "请先选择一个日期！");
        return;
    }
    
    // 获取当前选中的训练计划项
    QListWidgetItem* item = ui->listDateTrains->currentItem();
    if (!item) {
        QMessageBox::warning(this, "提示", "请先选择要删除的训练计划！");
        return;
    }
    
    // 获取选中项的索引
    int index = ui->listDateTrains->currentRow();
    
    // 从日期中删除训练计划
    calendar[currentDateIndex].delete_train(index);
    
    // 刷新显示
    refreshDateTrains();
    
    QMessageBox::information(this, "成功", "训练计划已从该日期删除！");
}

// =====================================================
// 创建新训练计划
// =====================================================
void TrainWindow::on_btnNewTrainPlan_clicked()
{
    // 弹出输入对话框，让用户输入训练计划名称
    bool ok;
    QString name = QInputDialog::getText(
        this,
        "新建训练计划",
        "请输入训练计划名称:",
        QLineEdit::Normal,
        "新训练计划",
        &ok
    );
    
    // 如果用户点击了确定
    if (ok && !name.isEmpty()) {
        // 创建新的训练计划对象
        train newTrain;
        newTrain.setTrainName(name.toStdString());
        
        // 添加到训练计划库
        trainPlans.push_back(newTrain);
        
        // 刷新训练计划列表
        refreshTrainPlansList();
        
        QMessageBox::information(this, "成功", "新训练计划已创建！");
    }
}

// =====================================================
// 删除训练计划
// =====================================================
void TrainWindow::on_btnDeleteTrainPlan_clicked()
{
    // 检查是否选择了训练计划
    if (currentTrainIndex == -1) {
        QMessageBox::warning(this, "提示", "请先选择要删除的训练计划！");
        return;
    }
    
    // 确认删除
    QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        "确认删除",
        "确定要删除该训练计划吗？",
        QMessageBox::Yes | QMessageBox::No
    );
    
    if (reply == QMessageBox::Yes) {
        // 删除训练计划
        trainPlans.erase(trainPlans.begin() + currentTrainIndex);
        currentTrainIndex = -1;
        
        // 刷新显示
        refreshTrainPlansList();
        refreshActionsList();
        
        QMessageBox::information(this, "成功", "训练计划已删除！");
    }
}

// =====================================================
// 保存训练计划（计算难度GPA）
// =====================================================
void TrainWindow::on_btnSaveTrainPlan_clicked()
{
    // 检查是否选择了训练计划
    if (currentTrainIndex == -1) {
        QMessageBox::warning(this, "提示", "请先选择一个训练计划！");
        return;
    }
    
    // 检查是否有用户对象
    if (!currentUser) {
        QMessageBox::warning(this, "提示", "请先创建用户对象！");
        return;
    }
    
    // 计算训练计划的总体难度GPA（需要传入用户对象）
    // 这个函数会同时计算训练难度并设置user的guess_gpa
    trainPlans[currentTrainIndex].calculate_total_difficulty_gpa(currentUser);
    
    // 获取计算结果
    double gpa = trainPlans[currentTrainIndex].getTotalDifficultyGPA();
    double guessGpa = currentUser->getGuessGPA();  // 从user对象获取预测GPA
    double currentGpa = currentUser->getSportGPA();
    
    qDebug() << "保存训练计划:";
    qDebug() << "  当前运动GPA =" << currentGpa;
    qDebug() << "  训练难度GPA =" << gpa;
    qDebug() << "  预测达到GPA =" << guessGpa;
    
    // 构建显示信息
    QString message = QString(
        "训练计划已保存！\n\n"
        "📊 当前运动GPA: %1\n"
        "💪 训练难度GPA: %2\n"
        "🎯 预测达到GPA: %3\n\n"
        "%4"
    ).arg(currentGpa, 0, 'f', 2)
     .arg(gpa, 0, 'f', 2)
     .arg(guessGpa, 0, 'f', 2)
     .arg(guessGpa > currentGpa ? 
          QString("预计提升: +%1").arg(guessGpa - currentGpa, 0, 'f', 2) :
          "建议增加训练难度以获得提升");
    
    QMessageBox::information(this, "保存成功", message);
}

// =====================================================
// 选择训练计划
// =====================================================
void TrainWindow::on_listTrainPlans_itemClicked(QListWidgetItem *item)
{
    // 获取选中项的索引
    currentTrainIndex = ui->listTrainPlans->currentRow();
    
    // 加载训练计划详情
    loadTrainPlanDetails();
}

// =====================================================
// 添加动作
// =====================================================
void TrainWindow::on_btnAddAction_clicked()
{
    // 检查是否选择了训练计划
    if (currentTrainIndex == -1) {
        QMessageBox::warning(this, "提示", "请先选择一个训练计划！");
        return;
    }
    
    // 弹出输入对话框，让用户输入动作名称
    bool ok;
    QString name = QInputDialog::getText(
        this,
        "添加动作",
        "请输入动作名称:",
        QLineEdit::Normal,
        "深蹲",
        &ok
    );
    
    if (ok && !name.isEmpty()) {
        // 创建新动作对象（使用::actions来明确指定全局命名空间的actions类）
        ::actions newAction(name.toStdString());
        
        // 添加到当前训练计划
        trainPlans[currentTrainIndex].add_action(newAction);
        
        // 调试输出
        qDebug() << "添加动作后，训练计划中的动作数量:" 
                 << trainPlans[currentTrainIndex].getActionList().size();
        
        // 刷新动作列表
        refreshActionsList();
        
        QMessageBox::information(this, "成功", "动作已添加！");
    }
}

// =====================================================
// 删除动作
// =====================================================
void TrainWindow::on_btnDeleteAction_clicked()
{
    // 检查是否选择了动作
    if (currentActionIndex == -1) {
        QMessageBox::warning(this, "提示", "请先选择要删除的动作！");
        return;
    }
    
    // 删除动作
    trainPlans[currentTrainIndex].delete_action(currentActionIndex);
    currentActionIndex = -1;
    
    // 刷新显示
    refreshActionsList();
    refreshSetsList();
    
    QMessageBox::information(this, "成功", "动作已删除！");
}

// =====================================================
// 修改动作（暂不实现，可以扩展）
// =====================================================
void TrainWindow::on_btnModifyAction_clicked()
{
    QMessageBox::information(this, "提示", "修改功能开发中...");
}

// =====================================================
// 选择动作
// =====================================================
void TrainWindow::on_listActions_itemClicked(QListWidgetItem *item)
{
    // 获取选中项的索引
    currentActionIndex = ui->listActions->currentRow();
    
    // 刷新训练组和教学链接列表
    refreshSetsList();
    refreshTutorialsList();
}

// =====================================================
// 添加训练组
// =====================================================
void TrainWindow::on_btnAddSet_clicked()
{
    // 检查是否选择了动作
    if (currentActionIndex == -1) {
        QMessageBox::warning(this, "提示", "请先选择一个动作！");
        return;
    }
    
    // 检查是否选择了训练计划
    if (currentTrainIndex == -1) {
        QMessageBox::warning(this, "提示", "请先选择一个训练计划！");
        return;
    }
    
    // 从UI控件获取训练组参数
    int weight = ui->spinWeight->value();  // 重量
    int reps = ui->spinReps->value();  // 次数
    int groups = ui->spinGroups->value();  // 组数
    int rest = ui->spinRest->value();  // 休息时间
    int eval = ui->comboEval->currentIndex();  // 难度评价
    
    qDebug() << "添加训练组: 重量=" << weight << "次数=" << reps << "组数=" << groups;
    
    // 创建新训练组
    set newSet(weight, reps, groups, rest, eval);
    
    // 获取动作列表（使用::actions明确指定全局命名空间）
    std::vector<::actions> actionList = trainPlans[currentTrainIndex].getActionList();
    
    qDebug() << "添加前，动作" << currentActionIndex << "有" << actionList[currentActionIndex].getSets().size() << "个训练组";
    
    // 在副本上添加训练组
    actionList[currentActionIndex].add_set(newSet);
    
    qDebug() << "添加后，动作" << currentActionIndex << "有" << actionList[currentActionIndex].getSets().size() << "个训练组";
    
    // 更新训练计划中的动作（这里会将修改后的动作写回）
    trainPlans[currentTrainIndex].modify_action(currentActionIndex, actionList[currentActionIndex]);
    
    // 验证是否真的更新了
    std::vector<::actions> verifyList = trainPlans[currentTrainIndex].getActionList();
    qDebug() << "验证：动作" << currentActionIndex << "现在有" << verifyList[currentActionIndex].getSets().size() << "个训练组";
    
    // 刷新训练组列表
    refreshSetsList();
    
    QMessageBox::information(this, "成功", "训练组已添加！");
}

// =====================================================
// 删除训练组
// =====================================================
void TrainWindow::on_btnDeleteSet_clicked()
{
    // 检查是否选择了训练组
    if (currentSetIndex == -1) {
        QMessageBox::warning(this, "提示", "请先选择要删除的训练组！");
        return;
    }
    
    // 获取当前动作（使用::actions明确指定全局命名空间）
    std::vector<::actions> actionList = trainPlans[currentTrainIndex].getActionList();
    
    // 删除训练组
    actionList[currentActionIndex].delete_set(currentSetIndex);
    
    // 更新训练计划中的动作
    trainPlans[currentTrainIndex].modify_action(currentActionIndex, actionList[currentActionIndex]);
    
    currentSetIndex = -1;
    
    // 刷新训练组列表
    refreshSetsList();
    
    QMessageBox::information(this, "成功", "训练组已删除！");
}

// =====================================================
// 修改训练组（暂不实现，可以扩展）
// =====================================================
void TrainWindow::on_btnModifySet_clicked()
{
    QMessageBox::information(this, "提示", "修改功能开发中...");
}

// =====================================================
// 添加教学视频链接
// =====================================================
void TrainWindow::on_btnAddTutorial_clicked()
{
    // 检查是否选择了动作
    if (currentActionIndex == -1) {
        QMessageBox::warning(this, "提示", "请先选择一个动作！");
        return;
    }
    
    // 弹出输入对话框
    bool ok;
    QString link = QInputDialog::getText(
        this,
        "添加教学链接",
        "请输入视频链接地址:",
        QLineEdit::Normal,
        "https://",
        &ok
    );
    
    if (ok && !link.isEmpty()) {
        // 获取当前动作（使用::actions明确指定全局命名空间）
        std::vector<::actions> actionList = trainPlans[currentTrainIndex].getActionList();
        
        // 添加教学链接
        actionList[currentActionIndex].add_tutorial_link(link.toStdString());
        
        // 更新训练计划中的动作
        trainPlans[currentTrainIndex].modify_action(currentActionIndex, actionList[currentActionIndex]);
        
        // 刷新教学链接列表
        refreshTutorialsList();
        
        QMessageBox::information(this, "成功", "教学链接已添加！");
    }
}

// =====================================================
// 删除教学视频链接
// =====================================================
void TrainWindow::on_btnDeleteTutorial_clicked()
{
    // 获取选中的链接项
    QListWidgetItem* item = ui->listTutorials->currentItem();
    if (!item) {
        QMessageBox::warning(this, "提示", "请先选择要删除的链接！");
        return;
    }
    
    // 获取索引
    int index = ui->listTutorials->currentRow();
    
    // 获取当前动作（使用::actions明确指定全局命名空间）
    std::vector<::actions> actionList = trainPlans[currentTrainIndex].getActionList();
    
    // 删除教学链接
    actionList[currentActionIndex].delete_tutorial_link(index);
    
    // 更新训练计划中的动作
    trainPlans[currentTrainIndex].modify_action(currentActionIndex, actionList[currentActionIndex]);
    
    // 刷新教学链接列表
    refreshTutorialsList();
    
    QMessageBox::information(this, "成功", "教学链接已删除！");
}

// =====================================================
// 辅助函数：刷新训练计划列表
// =====================================================
void TrainWindow::refreshTrainPlansList()
{
    // 清空列表
    ui->listTrainPlans->clear();
    
    // 遍历所有训练计划，添加到列表
    for (size_t i = 0; i < trainPlans.size(); i++) {
        QString name = QString::fromStdString(trainPlans[i].getTrainName());
        ui->listTrainPlans->addItem(name);
    }
}

// =====================================================
// 辅助函数：刷新动作列表
// =====================================================
void TrainWindow::refreshActionsList()
{
    // 清空列表
    ui->listActions->clear();
    
    // 如果没有选中训练计划，返回
    if (currentTrainIndex == -1) {
        qDebug() << "refreshActionsList: 没有选中训练计划";
        return;
    }
    
    // 获取训练计划的动作列表（使用::actions明确指定全局命名空间）
    std::vector<::actions> actionList = trainPlans[currentTrainIndex].getActionList();
    
    // 调试输出
    qDebug() << "refreshActionsList: 动作列表大小 =" << actionList.size();
    
    // 遍历所有动作，添加到列表
    for (size_t i = 0; i < actionList.size(); i++) {
        QString name = QString::fromStdString(actionList[i].getActionName());
        qDebug() << "添加动作到列表:" << name;
        ui->listActions->addItem(name);
    }
}

// =====================================================
// 辅助函数：刷新训练组列表
// =====================================================
void TrainWindow::refreshSetsList()
{
    // 清空列表
    ui->listSets->clear();
    
    qDebug() << "refreshSetsList: currentActionIndex=" << currentActionIndex 
             << ", currentTrainIndex=" << currentTrainIndex;
    
    // 如果没有选中动作，返回
    if (currentActionIndex == -1) {
        qDebug() << "refreshSetsList: 没有选中动作";
        return;
    }
    
    // 如果没有选中训练计划，返回
    if (currentTrainIndex == -1) {
        qDebug() << "refreshSetsList: 没有选中训练计划";
        return;
    }
    
    // 获取动作的训练组列表（使用::actions明确指定全局命名空间）
    std::vector<::actions> actionList = trainPlans[currentTrainIndex].getActionList();
    std::vector<set> sets = actionList[currentActionIndex].getSets();
    
    qDebug() << "refreshSetsList: 训练组数量=" << sets.size();
    
    // 遍历所有训练组，添加到列表
    for (size_t i = 0; i < sets.size(); i++) {
        QString info = QString("%1kg × %2次 × %3组 (休息%4秒)")
            .arg(sets[i].getWeight())
            .arg(sets[i].getReps())
            .arg(sets[i].getGroup())
            .arg(sets[i].getRest());
        qDebug() << "添加训练组到列表:" << info;
        ui->listSets->addItem(info);
    }
}

// =====================================================
// 辅助函数：刷新教学链接列表
// =====================================================
void TrainWindow::refreshTutorialsList()
{
    // 清空列表
    ui->listTutorials->clear();
    
    // 如果没有选中动作，返回
    if (currentActionIndex == -1) return;
    
    // 获取动作的教学链接列表（使用::actions明确指定全局命名空间）
    std::vector<::actions> actionList = trainPlans[currentTrainIndex].getActionList();
    std::vector<std::string> links = actionList[currentActionIndex].getTutorialLinks();
    
    // 遍历所有链接，添加到列表
    for (size_t i = 0; i < links.size(); i++) {
        QString link = QString::fromStdString(links[i]);
        ui->listTutorials->addItem(link);
    }
}

// =====================================================
// 辅助函数：刷新当前日期的训练计划
// =====================================================
void TrainWindow::refreshDateTrains()
{
    // 清空列表
    ui->listDateTrains->clear();
    
    // 如果没有选中日期，返回
    if (currentDateIndex == -1) return;
    
    // 获取该日期的训练计划列表
    std::vector<train> dateTrains = calendar[currentDateIndex].train_list;
    
    // 遍历所有训练计划，添加到列表
    for (size_t i = 0; i < dateTrains.size(); i++) {
        QString name = QString::fromStdString(dateTrains[i].getTrainName());
        ui->listDateTrains->addItem(name);
    }
}

// =====================================================
// 辅助函数：加载训练计划详情
// =====================================================
void TrainWindow::loadTrainPlanDetails()
{
    // 刷新动作列表
    refreshActionsList();
    
    // 显示训练计划名称
    if (currentTrainIndex != -1) {
        ui->lineTrainName->setText(
            QString::fromStdString(trainPlans[currentTrainIndex].getTrainName())
        );
    }
}

// =====================================================
// 辅助函数：查找日期索引
// =====================================================
int TrainWindow::findDateIndex(const QDate &selectedDate)
{
    // 遍历日历，查找匹配的日期
    for (size_t i = 0; i < calendar.size(); i++) {
        if (calendar[i].year == selectedDate.year() &&
            calendar[i].month == selectedDate.month() &&
            calendar[i].day == selectedDate.day()) {
            return i;
        }
    }
    
    // 未找到，返回-1
    return -1;
}

// =====================================================
// 初始化服务器连接
// =====================================================
void TrainWindow::initServerConnection()
{
    serverConnection = new ClientConnection(this);
    
    // 连接服务器信号
    connect(serverConnection, &ClientConnection::trainDataSaved,
            this, &TrainWindow::onTrainDataSaved);
    connect(serverConnection, &ClientConnection::trainDataLoaded,
            this, &TrainWindow::onTrainDataLoaded);
    
    // 尝试连接到服务器
    if (!serverConnection->connectToServer("127.0.0.1", 12345)) {
        qDebug() << "无法连接到服务器，训练数据将无法保存";
        QMessageBox::warning(this, "警告", "无法连接到服务器，训练数据将无法保存到后端");
    } else {
        qDebug() << "已连接到服务器，可以使用后端存储";
    }
}

// =====================================================
// 保存所有数据到服务器
// =====================================================
void TrainWindow::saveAllDataToServer()
{
    if (!serverConnection || !currentUser) {
        qDebug() << "无法保存到服务器：连接或用户不存在";
        return;
    }
    
    // 创建组合的训练数据JSON对象
    QJsonObject combinedData;
    
    // 保存日历数据
    QJsonArray calendarArray;
    for (const auto& d : calendar) {
        calendarArray.append(d.toJson());
    }
    combinedData["calendar"] = calendarArray;
    
    // 保存训练计划数据
    QJsonArray trainPlansArray;
    for (const auto& t : trainPlans) {
        trainPlansArray.append(t.toJson());
    }
    combinedData["trainPlans"] = trainPlansArray;
    
    // 创建临时train对象用于发送
    train tempTrain;
    QString userId = QString::fromStdString(currentUser->name);
    
    // 发送到服务器
    QJsonObject message;
    message["type"] = "save_train_data";
    message["userId"] = userId;
    message["trainData"] = combinedData;
    
    // 直接使用底层发送方法
    if (serverConnection) {
        qDebug() << "正在保存训练数据到服务器，用户ID:" << userId;
        serverConnection->saveTrainData(userId, tempTrain);  // 注意：这里需要修改以支持完整数据
    }
}

// =====================================================
// 从服务器加载所有数据
// =====================================================
void TrainWindow::loadAllDataFromServer()
{
    if (!serverConnection || !currentUser) {
        qDebug() << "无法从服务器加载：连接或用户不存在";
        // 使用本地数据作为后备
        loadAllDataLocal();
        return;
    }
    
    QString userId = QString::fromStdString(currentUser->name);
    qDebug() << "正在从服务器加载训练数据，用户ID:" << userId;
    serverConnection->loadTrainData(userId);
}

// =====================================================
// 训练数据保存响应
// =====================================================
void TrainWindow::onTrainDataSaved(bool success, const QString &message)
{
    if (success) {
        qDebug() << "训练数据已保存到服务器";
        ui->statusbar->showMessage("训练数据已保存到服务器", 3000);
    } else {
        qDebug() << "保存训练数据失败:" << message;
        ui->statusbar->showMessage("保存训练数据失败: " + message, 3000);
    }
}

// =====================================================
// 训练数据加载响应
// =====================================================
void TrainWindow::onTrainDataLoaded(bool success, train* trainPlan)
{
    if (success && trainPlan) {
        qDebug() << "训练数据已从服务器加载";
        // TODO: 解析完整的训练数据（包括日历和训练计划）
        // 目前先使用本地数据
        delete trainPlan;
        ui->statusbar->showMessage("训练数据已从服务器加载", 3000);
    } else {
        qDebug() << "从服务器加载训练数据失败，使用本地数据";
        // 使用本地数据作为后备
        loadAllDataLocal();
        ui->statusbar->showMessage("使用本地数据", 3000);
    }
}

// =====================================================
// 保存所有数据到文件（本地后备）
// =====================================================
void TrainWindow::loadAllDataLocal()
{
    // 加载日历数据
    QString calendarPath = TrainDataStorage::getCalendarFilePath();
    calendar = TrainDataStorage::loadCalendar(calendarPath);
    qDebug() << "加载日历数据:" << calendar.size() << "个日期";
    
    // 加载训练计划数据
    QString trainPlansPath = TrainDataStorage::getTrainPlansFilePath();
    trainPlans = TrainDataStorage::loadTrainPlans(trainPlansPath);
    qDebug() << "加载训练计划数据:" << trainPlans.size() << "个计划";
}
