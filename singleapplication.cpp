#include "singleapplication.h"
#include <QtNetwork/QLocalSocket>
#include <QFileInfo>

#define TIME_OUT                (500)    // 500ms

SingleApplication::SingleApplication(int &argc, char **argv)
    : QApplication(argc, argv)
    , w(NULL)
    , isRunning_(false)
    , localServer_(NULL) {

    // 取应用程序名作为LocalServer的名字
    serverName_ = QFileInfo(QCoreApplication::applicationFilePath()).fileName();

    initLocalConnection();
}


////////////////////////////////////////////////////////////////////////////////
// 说明：
// 检查是否已經有一个实例在运行, true - 有实例运行， false - 没有实例运行
////////////////////////////////////////////////////////////////////////////////
bool SingleApplication::isRunning() {
    return isRunning_;
}

////////////////////////////////////////////////////////////////////////////////
// 说明：
// 通过socket通讯实现程序单实例运行，监听到新的连接时触发该函数
////////////////////////////////////////////////////////////////////////////////
void SingleApplication::newLocalConnection() {
    QLocalSocket *socket = localServer_->nextPendingConnection();
    if(socket) {
        socket->waitForReadyRead(2*TIME_OUT);
        delete socket;

        // 其他处理，如：读取启动参数

        activateWindow();
    }
}

////////////////////////////////////////////////////////////////////////////////
// 说明：
// 通过socket通讯实现程序单实例运行，
// 初始化本地连接，如果连接不上server，则创建，否则退出
////////////////////////////////////////////////////////////////////////////////
void SingleApplication::initLocalConnection() {
    isRunning_ = false;

    QLocalSocket socket;
    socket.connectToServer(serverName_);
    if(socket.waitForConnected(TIME_OUT)) {
        fprintf(stderr, "%s already running.\n", serverName_.toLocal8Bit().constData());
        isRunning_ = true;
        // 其他处理，如：将启动参数发送到服务端
        return;
    }

    //连接不上服务器，就创建一个
    newLocalServer();
}

////////////////////////////////////////////////////////////////////////////////
// 说明：
// 创建LocalServer
////////////////////////////////////////////////////////////////////////////////
void SingleApplication::newLocalServer() {
    localServer_ = new QLocalServer(this);
    connect(localServer_, SIGNAL(newConnection()), this, SLOT(newLocalConnection()));
    if(!localServer_->listen(serverName_)) {
        // 此时监听失败，可能是程序崩溃时,残留进程服务导致的,移除之
        if(localServer_->serverError() == QAbstractSocket::AddressInUseError) {
            QLocalServer::removeServer(serverName_); // <-- 重点
            localServer_->listen(serverName_); // 再次监听
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
// 说明：
// 激活主窗口
////////////////////////////////////////////////////////////////////////////////
void SingleApplication::activateWindow() {
    if(w) {
        w->show();
        w->raise();
        w->activateWindow(); // 激活窗口
    }
}
