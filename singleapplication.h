#ifndef SINGLEAPPLICATION_H
#define SINGLEAPPLICATION_H

#include <QObject>
#include <QApplication>
#include <QtNetwork/QLocalServer>
#include <QWidget>

 class SingleApplication : public QApplication {
         Q_OBJECT
public:
    SingleApplication(int &argc, char **argv);

    bool isRunning();                // 是否已經有实例在运行
    QWidget *w;                        // MainWindow指针

private slots:
    // 有新连接时触发
    void newLocalConnection();

private:
    // 初始化本地连接
    void initLocalConnection();
    // 创建服务端
    void newLocalServer();
    // 激活窗口
    void activateWindow();

    bool isRunning_;                // 是否已經有实例在运行
    QLocalServer *localServer_;     // 本地socket Server
    QString serverName_;            // 服务名称
};

#endif // SINGLEAPPLICATION_H
