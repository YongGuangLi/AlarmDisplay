#ifndef RIS_Log_H
#define RIS_Log_H

#define MAX_FILE_LINE   4000

#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QList>
#include <QString>

#define print_Log(x, y) RIS_Log::instance()->outputLog(x, y)
#define print_info(x) RIS_Log::instance()->print_log(QString(x))

#define pirnt_debug(x) RIS_Log::instance()->print_log(x)



class RIS_Log : public QThread
{
public:
    static RIS_Log* instance();
    static void close_singleton();
    void print_log(QString msg);
	void outputLog(bool isPut, QString msg);//是否输出日志
protected:
    RIS_Log(QObject *parent = 0, QThread::Priority priority = QThread::NormalPriority);
    ~RIS_Log();

    void run();
    QMutex mutex_;
    QWaitCondition bufferEmpty_;
    QList<QString> lstBuffer_;
    void workerThreadMain(QString &msg);
private:
    static RIS_Log* instance_;
    QThread::Priority priority_;
    int line_;
    QString fileDir_;
    QString fileName_;


};

#endif // RIS_Log_H
