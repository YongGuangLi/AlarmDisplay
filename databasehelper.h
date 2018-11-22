#ifndef DATABASEHELPER_H
#define DATABASEHELPER_H

#include "configini.h"
#include "alarmdata.h"
#include "RtdbMessage.pb.h"
#include <QObject>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QMessageBox>
#include <QDateTime>
#include <QString>

#define SingletonDBHelper DataBaseHelper::GetInstance()

class DataBaseHelper : public QObject
{
    Q_OBJECT
public:
    static DataBaseHelper *GetInstance();

    bool open(QString ip,int port, QString dbName, QString user, QString passwd);
    bool isOpen();

    //初始化用户表，默认超级用户
    void initUserinfoTable();

    //读取智能告警数据
    void readIaAlarmDataFromDB();
    //读取原始告警数据
    void readOaAlarmDataFromDB();

    //获取所有用户信息
    void readUserInfoFromDB();
    //新增用户数据
    bool insertUserInfoToDB(QString,QString,QString);
    //修改用户数据
    bool modifyUserInfoToDB(QString,QString);

    //获取所有用户组信息
    void readUserGroupFromDB();
    //新增用户组
    bool addUserGroupToDB(QString, int);
    //删除用户组
    bool deleteUserGroupToDB(QString);

    //获取所有权限信息
    void readRightFromDB();
    //修改权限
    bool modifyRightToBD(int rightID, int groupflag);
    //删除用户
    bool deleteUserInfoToDB(QString);

    //查询历史告警
    void queryHistoryData(QString beginTime, QString endTime, int alarmLevel, int alarmType);   //alarmType 0:智能告警   1:原始告警

    //查询统计信息
    int querySmartDataCnt(QString beginTime, QString endTime, int alarmLevel);
    int queryOriginalDataCnt(QString beginTime, QString endTime, int alarmLevel);

    QString getError();

    bool modifyIaAlarmStatus(int id);
    bool modifyOaAlarmStatus(QString alarmTime, int id);
signals:
    void sendAlarmData(int, stAlarmData);
private:
    explicit DataBaseHelper(QObject *parent = 0);
    static DataBaseHelper * dbHelp_;

    QSqlDatabase sqlDatabase;
    QString ip_;
    int port_;
    QString dbName_;
    QString user_;
    QString passwd_;

    QTimer *timer;
signals:

public slots:
};

#endif // DATABASEHELPER_H
