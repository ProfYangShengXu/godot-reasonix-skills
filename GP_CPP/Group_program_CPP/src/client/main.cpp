// 包含必要的头文件
#include <QApplication>  // Qt应用程序类
#include <Windows.h>     // Windows API
#include "ui/mainwindow.h"  // 主窗口类（位于include/ui/mainwindow.h）

// Windows应用程序入口点
// 这是Windows GUI应用程序的标准入口函数，而不是控制台应用程序的main函数
// @param hInstance: 当前实例句柄
// @param hPrevInstance: 总是NULL，为了兼容16位Windows保留
// @param lpCmdLine: 命令行参数
// @param nCmdShow: 窗口显示方式
int WINAPI WinMain(HINSTANCE hInstance,     // 当前实例句柄
                   HINSTANCE hPrevInstance,  // 未使用，保持为NULL
                   LPSTR lpCmdLine,         // 命令行参数
                   int nCmdShow)            // 窗口显示方式
{
    // Qt应用程序需要的命令行参数
    // 由于是Windows GUI应用程序，这里不需要实际的参数
    int argc = 0;
    char **argv = nullptr;

    // 创建Qt应用程序对象
    // 这是Qt GUI应用程序必需的，管理应用程序的生命周期
    QApplication app(argc, argv);

    // 创建主窗口实例
    MainWindow window;
    
    // 显示主窗口
    // show()是非模态显示，不会阻塞程序继续运行
    window.show();

    // 启动应用程序的事件循环
    // exec()会一直运行，直到应用程序退出
    // 返回值通常用作程序的退出码
    return app.exec();
}