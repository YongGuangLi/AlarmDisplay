#ifndef CONFIGINI_H
#define CONFIGINI_H

#include <QObject>
#include <QSettings>
#include <QApplication>
#include <QFile>
#include <QDebug>
#include <QColor>
#include <QColorDialog>
#include <QPushButton>
#include <QDomDocument>
#include <QMessageBox>



enum RightID
{
    ADDUSER,            //新增用户
    DELETEUSER,         //删除用户
    MODIFYUSER,         //修改用户
    CONFIRMALARM,       //确认告警
    DELETEALARM,        //删除告警
    ALAEMDETAIL,        //查看告警详情
    MODIFYUSERGROUP     //修改用户组
};


typedef struct
{
    int userID;
    QString userName;
    QString userPassWd;
    QString groupName;
    int currentUserFlag;
}stUserInfo;

typedef struct
{
    int rightID;
    QString rightName;
    int groupFlag;
}stRightInfo;


#define SingletonConfig ConfigIni::GetInstance()

class ConfigIni : public QObject
{
    Q_OBJECT
public:
    static ConfigIni *GetInstance();

    //读取Config.ini
    bool initConfigIni(QString);

    //读取ActionClassifier.xml
    bool initActionClassifier(QString);
    void parseActions(QDomElement);
    void parseItem(QDomElement);

    float getActionClassifier(QString);

    //读取IABaySignal.xml
    bool initIABaySignal(QString);
    void parseStation(QDomElement);
    void parseVoltage(QDomElement);
    void parseVoltageItem(QDomElement);
    void parseDevice(QDomElement);
    void parseDots(QDomElement);


    //读取IAFRule.xml
    bool initIAFRule(QString);
    void parseIAFRuleItem(QDomElement);

    //读取IASRule.xml
    bool initIASRule(QString);
    void parseIASRuleItem(QDomElement);


    //mysql数据
    QString getIpMySql() const;
    int getPortMySql() const;
    QString getDbName() const;
    QString getUserMysql() const;
    QString getPasswdMysql() const;

    //redis数据
    QString getIpRedis() const;
    int getPortRedis() const;

    QString getStation() const;

    //告警报告路径
    QString getReportPath() const;

    //表格显示最大行数
    int getMaxSize() const;

    //告警等级颜色
    QColor getAlarmLevelColor(int level);
    void setAlarmLevelColor(int level, QColor color);
    void writeColorToFile();

    //间隔过滤
    void getBay(QString);
    QStringList getListBay();

    //告警等级弹窗
    int getAlarmLevelFlag(int level);
    void setAlarmLevelFlag(int level, int flag);

    //用户信息，
    void setUserInfo(QString, stUserInfo*);
    stUserInfo* getUserInfo(QString);
    QMap<QString, stUserInfo *> getUserInfo() const;

    bool IsUserExits(QString);
    bool CheckUserInfo(QString,QString);

    //所有用户组信息
    bool existUserGroup(QString);
    void setUserGroup(QString, int);
    QMap<QString, int> getUserGroup() const;
    int getUserGroupId(QString) const;
    int getMaxGroupId();

    //权限信息
    void setRightInfo(QString,stRightInfo*);
    stRightInfo* getRightInfo(QString);
    stRightInfo* getRightInfo(int);
    QMap<QString,stRightInfo*> getRightInfo() const;
    bool isCurrentUserhasRight(int rightID);              //当前用户是否有rightID权限

    //当前用户
    void setCurUser(const QString &Cur_User);
    QString getCurUser() const;

    //
    QString getLogicByCode(QString);

private:
    explicit ConfigIni(QObject *parent = 0);
    ~ConfigIni();

    static ConfigIni* configIni;

    QString ipMySql_;
    int portMySql_;
    QString userMysql_;
    QString passwdMysql_;
    QString dbName_;

    QString ipRedis_;
    int portRedis;

    QString station_;

    QStringList listBay_;

    QString Cur_User_;        //当前用户名

    QString ReportPath_;      //告警简报路径

    int iMaxSize_;            //显示最大告警个数

    QMap<int,QColor> mapAlarmLevelColor_;      //告警等级对应的颜色  key:告警等级   value:颜色
    QMap<int, int> mapAlarmLevelFlag_;         //告警框自动弹出       key:告警等级   value:0关  1开

    QMap<QString,stUserInfo*> mapUserInfo_;        //所有用户信息， key:用户名   value:用户信息
    QMap<QString,stRightInfo*> mapRightInfo_;      //所有权限信息   key:权限名
    QMap<QString, int> mapUserGroup_;              //所有用户组信息 key:组名  value:组ID

    QMap<QString, int> mapActionClassifier_;
    QSettings *settings_;

    QStringList listEventSignal_;

    QMap<QString, QString> mapCodeToLogic_;
signals:

public slots:
};

#endif // CONFIGINI_H
