#include "databasehelper.h"

DataBaseHelper * DataBaseHelper::dbHelp_ = NULL;

#define SQL_USERINFO_TABLE     "create table userinfo(username varchar(20) primary key, password varchar(20), level int)"

#define SQL_GET_USERINFO       "select id,username,password,groupname from userinfo"

#define SQL_INSERT_USERINFO    "insert into userinfo(username,password,groupname) value('%1','%2','%3')"
#define SQL_UPDATE_USERINFO    "update userinfo set password = '%1' where username = '%2'"
#define SQL_DELETE_USERINFO    "delete from userinfo where username = '%1'"

#define SQL_GET_GROUPINFO      "select id,groupname from groupinfo"
#define SQL_INSERT_GROUPINFO   "insert into groupinfo(id,groupname) value(%1,'%2')"
#define SQL_DELETE_GROUPINFO   "delete from groupinfo where groupname = '%1'"

#define SQL_GET_RIGHTINFO      "select id,rightname,groupflag from rightinfo"
#define SQL_UPDATE_RIGHTINFO   "update rightinfo set groupflag = %1 where id = %2"

#define SQL_GET_IAALARMDATA    "select id,level,time,filename,text,confirmed from iaalarmdata order by time desc limit %1"           //获取智能告警数据
#define SQL_GET_OAALARMDATA    "select id,level,lasTtime,device,msg,confirmed from dtxy_alarm_%1 order by lastTime desc limit %2"     //获取原始告警数据

#define SQL_GET_HISIAALARMDATA    "select id,level,time,filename,text,confirmed from iaalarmdata where time between '%1' and '%2'"                      //获取智能告警历史数据
#define SQL_GET_HISOAALARMDATA    "select id,level,lasTtime,device,msg,confirmed from dtxy_alarm_%1 where lasTtime between '%2' and '%3'"     //获取原始告警历史数据

#define SQL_MODIFY_IACONFIRM   "update iaalarmdata set confirmed = 1 where id = %1"                                                 //修改智能告警状态
#define SQL_MODIFY_OACONFIRM   "update dtxy_alarm_%1 set confirmed = '1' where id = %2"                                                 //修改原始告警状态


#define SQL_GET_IAALARMDATACNT "select COUNT(0) from iaalarmdata where time between '%1' and '%2' and level = %3"
#define SQL_GET_OAALARMDATACNT "select COUNT(0) from dtxy_alarm_%1 where lasTtime between '%2' and '%3' and level = %4"

DataBaseHelper::DataBaseHelper(QObject *parent) : QObject(parent)
{
    qRegisterMetaType<stAlarmData>("stAlarmData");
}

bool DataBaseHelper::open(QString ip, int port, QString dbName, QString user, QString passwd)
{
    sqlDatabase = QSqlDatabase::addDatabase("QMYSQL");
    sqlDatabase.setHostName(ip);
    sqlDatabase.setPort(port);
    sqlDatabase.setDatabaseName(dbName);
    sqlDatabase.setUserName(user);
    sqlDatabase.setPassword(passwd);
    ip_ = ip;
    port_ = port;
    dbName_ = dbName;
    user_ = user;
    passwd_ = passwd;
    return sqlDatabase.open();
}

bool DataBaseHelper::isOpen()
{
    bool result = sqlDatabase.isOpen();

    if(result == false)
    {
        result = open(ip_, port_, dbName_, user_, passwd_);
        if(result == true)
        {
            qDebug()<<QString::fromUtf8("数据库重新连接成功");
        }
    }

    return result;
}

void DataBaseHelper::initUserinfoTable()
{
    if(!sqlDatabase.tables().contains("userinfo"))
    {
        QSqlQuery query(SQL_USERINFO_TABLE);
        if(!query.lastError().isValid())
        {
            qDebug()<<"新建 userinfo 数据库表";
            insertUserInfoToDB("admin","admin",0);
        }
    }
}


void DataBaseHelper::readIaAlarmDataFromDB()
{
    QSqlQuery query(QString(SQL_GET_IAALARMDATA).arg(QString::number(SingletonConfig->getMaxSize())));
    if(query.lastError().isValid())
    {
        qDebug()<<query.lastError().text();
        return;
    }
    int size = query.size();
    while(size--)
    {
        query.seek(size);
        stAlarmData alarmData;
        alarmData.id_  = query.value(0).toInt();
        alarmData.level_  = query.value(1).toInt();
        alarmData.time_ =  query.value(2).toDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
        alarmData.filename_  = query.value(3).toString();
        //<0> 2018-09-10 16:39:17 茂名电网.广场变/10kV.#1站用变 保护测控装置异常告警 "T_0013 20180910_163917263_1S.xml 主变"
        alarmData.text_ =  query.value(4).toString().split(" ").at(3) + " " + query.value(4).toString().split(" ").at(4);
        alarmData.confirmed_ =   query.value(5).toInt();

        SingletonAlarmData->insertSmartAlarmData(alarmData);
    }
}


void DataBaseHelper::readOaAlarmDataFromDB()
{
    QSqlQuery query(QString(SQL_GET_OAALARMDATA).arg(QDateTime::currentDateTime().toString("yyyy_MM_dd")).arg(QString::number(SingletonConfig->getMaxSize())));
    if(query.lastError().isValid())
    {
        qDebug()<<query.lastError().text()<<query.lastQuery();
        return;
    }
    int size = query.size();

    while(size--)
    {
        query.seek(size);
        stAlarmData alarmData;
        alarmData.id_  = query.value(0).toInt();
        alarmData.level_  = query.value(1).toInt();
        alarmData.time_ =  query.value(2).toDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
        alarmData.text_ =  query.value(3).toString() + "  " + query.value(4).toString() ;
        alarmData.confirmed_ =   query.value(5).toInt();

        SingletonAlarmData->insertOriginalAlarmData(alarmData);
    }
}

/*************************************
Description: 读取数据库中的用户信息
Input:
Output:
Return:
Other:
 **************************************/
void DataBaseHelper::readUserInfoFromDB()
{
    QSqlQuery query(QString(SQL_GET_USERINFO));
    if(query.lastError().isValid())
    {
        qDebug()<<query.lastError().text();
        return;
    }
    while(query.next())
    {
        stUserInfo *userInfo = new stUserInfo();
        userInfo->userID =  query.value(0).toInt();
        userInfo->userName =  query.value(1).toString();
        userInfo->userPassWd =  query.value(2).toString();
        userInfo->groupName = query.value(3).toString();

        SingletonConfig->setUserInfo(userInfo->userName, userInfo);
    }
}

void DataBaseHelper::readUserGroupFromDB()
{
    QSqlQuery query(QString(SQL_GET_GROUPINFO));
    if(query.lastError().isValid())
    {
        qDebug()<<query.lastError().text();
        return;
    }
    while(query.next())
    {
        int id =  query.value(0).toInt();
        QString groupName =  query.value(1).toString();

        SingletonConfig->setUserGroup(groupName, id);
    }
}

bool DataBaseHelper::addUserGroupToDB(QString groupname, int groupid)
{
    bool result = true;

    QSqlQuery query(QString(SQL_INSERT_GROUPINFO).arg(groupid).arg(groupname));
    if(query.lastError().isValid())
    {
        qDebug()<<query.lastError().text()<<query.lastQuery();
        result = false;
    }
    else
    {
       qDebug()<<SingletonConfig->getCurUser().append(QString::fromUtf8(" 新建用户组:")).append(groupname);
    }

    return result;
}

bool DataBaseHelper::deleteUserGroupToDB(QString groupname)
{
    bool result = true;

    QSqlQuery query(QString(SQL_DELETE_GROUPINFO).arg(groupname));
    if(query.lastError().isValid())
    {
        qDebug()<<query.lastError().text();
        result = false;
    }
    else
    {
       qDebug()<<SingletonConfig->getCurUser().append(QString::fromUtf8(" 删除用户组:")).append(groupname);
    }

    return result;

}

/*************************************
Description: 读取数据库中的权限信息
Input:
Output:
Return:
Other:
 **************************************/
void DataBaseHelper::readRightFromDB()
{
    QSqlQuery query(QString(SQL_GET_RIGHTINFO));
    if(query.lastError().isValid())
    {
        qDebug()<<query.lastError().text();
        return;
    }
    while(query.next())
    {
        int id = query.value(0).toInt();
        QString rightName =  query.value(1).toString();
        int flag =  query.value(2).toInt();

        stRightInfo *rightInfo = new stRightInfo();
        rightInfo->rightID = id;
        rightInfo->rightName = rightName;
        rightInfo->groupFlag = flag;

        SingletonConfig->setRightInfo(rightName, rightInfo);
    }
}

bool DataBaseHelper::modifyRightToBD(int rightID, int groupflag)
{
    bool result = true;

    QSqlQuery query(QString(SQL_UPDATE_RIGHTINFO).arg(groupflag).arg(rightID));
    if(query.lastError().isValid())
    {
        qDebug()<<query.lastError().text();
        result = false;
    }
    else
    {
       qDebug()<<SingletonConfig->getCurUser().append(QString::fromUtf8(" 修改用户权限:%1").arg(rightID));
    }

    return result;
}


void DataBaseHelper::queryHistoryData(QString beginTime, QString endTime, int alarmLevel, int alarmType)
{
    if(alarmType == 0)          // 0:智能告警
    {
        QSqlQuery query(QString(SQL_GET_HISIAALARMDATA).arg(beginTime).arg(endTime));
        if(query.lastError().isValid())
        {
            qDebug()<<query.lastError().text()<<query.lastQuery();
            return;
        }
        int size = query.size();
        while(size--)
        {
            query.seek(size);
            stAlarmData alarmData;
            alarmData.id_  = query.value(0).toInt();
            alarmData.level_  = query.value(1).toInt();
            alarmData.time_ =  query.value(2).toDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
            alarmData.filename_  = query.value(3).toString();
            //<0> 2018-09-10 16:39:17 茂名电网.广场变/10kV.#1站用变 保护测控装置异常告警 "T_0013 20180910_163917263_1S.xml 主变"
            alarmData.text_ =  query.value(4).toString().split(" ").at(3) + " " + query.value(4).toString().split(" ").at(4);
            alarmData.confirmed_ =   query.value(5).toInt();

            if(alarmData.level_ == alarmLevel || LEVEL_REALTIME == alarmLevel)
            {
                emit sendAlarmData(alarmData.id_, alarmData);
            }
        }
    }else if(alarmType == 1)   //  1:原始告警
    {
        QDate beginDate = QDateTime::fromString(beginTime, "yyyy-MM-dd hh:mm:ss").date();
        QDate endDate = QDateTime::fromString(endTime, "yyyy-MM-dd hh:mm:ss").date();

        while(beginDate <= endDate)
        {
            QSqlQuery query(QString(SQL_GET_HISOAALARMDATA).arg(beginDate.toString("yyyy_MM_dd")).arg(beginTime).arg(endTime));
            beginDate = beginDate.addDays(1);
            if(query.lastError().isValid())
            {
                qDebug()<<query.lastError().text()<<query.lastQuery();
                continue;
            }
            int size = query.size();

            while(size--)
            {
                query.seek(size);
                stAlarmData alarmData;
                alarmData.id_  = query.value(0).toInt();
                alarmData.level_  = query.value(1).toInt();
                alarmData.time_ =  query.value(2).toDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
                alarmData.text_ =  query.value(3).toString() + "  " + query.value(4).toString() ;
                alarmData.confirmed_ =   query.value(5).toInt();

                if(alarmData.level_ == alarmLevel || LEVEL_REALTIME == alarmLevel)
                {
                    emit sendAlarmData(alarmData.id_, alarmData);
                }
            }
        }
    }
}

int DataBaseHelper::querySmartDataCnt(QString beginTime, QString endTime, int alarmLevel)
{
    int Cnt = 0;
    QSqlQuery query(QString(SQL_GET_IAALARMDATACNT).arg(beginTime).arg(endTime).arg(alarmLevel));

    if(query.lastError().isValid())
        qDebug()<<query.lastError().text()<<query.lastQuery();

    while(query.next())
    {
        Cnt = query.value(0).toInt();
    }
    return Cnt;
}

int DataBaseHelper::queryOriginalDataCnt(QString beginTime, QString endTime, int alarmLevel)
{
    int Cnt = 0;
    QDate beginDate = QDateTime::fromString(beginTime, "yyyy-MM-dd hh:mm:ss").date();
    QDate endDate = QDateTime::fromString(endTime, "yyyy-MM-dd hh:mm:ss").date();

    while(beginDate <= endDate)
    {
        if(!sqlDatabase.tables().contains(QString("dtxy_alarm_%1").arg(beginDate.toString("yyyy_MM_dd"))))
        {
            beginDate = beginDate.addDays(1);
            continue;
        }
        QSqlQuery query(QString(SQL_GET_OAALARMDATACNT).arg(beginDate.toString("yyyy_MM_dd")).arg(beginTime).arg(endTime).arg(alarmLevel));
        beginDate = beginDate.addDays(1);
        if(query.lastError().isValid())
        {
            qDebug()<<query.lastError().text()<<query.lastQuery();
            continue ;
        }
        while(query.next())
        {
            Cnt += query.value(0).toInt();
        }
    }

    return  Cnt;
}



bool DataBaseHelper::insertUserInfoToDB(QString username, QString password, QString groupname)
{
    bool result = true;

    QSqlQuery query(QString(SQL_INSERT_USERINFO).arg(username).arg(password).arg(groupname));
    if(query.lastError().isValid())
    {
        qDebug()<<query.lastError().text();
        result = false;
    }
    else
    {
       qDebug()<<SingletonConfig->getCurUser().append(QString::fromUtf8(" 新建用户:")).append(username);
    }

    return result;
}

bool DataBaseHelper::modifyUserInfoToDB(QString username, QString password)
{
    bool result = true;

    QSqlQuery query(QString(SQL_UPDATE_USERINFO).arg(password).arg(username));
    if(query.lastError().isValid())
    {
        qDebug()<<query.lastError().text();
        result = false;
    }
    else
    {
       qDebug()<<SingletonConfig->getCurUser().append(QString::fromUtf8(" 修改用户 ")).append(username);
    }

    return result;
}

bool DataBaseHelper::deleteUserInfoToDB(QString username)
{
    bool result = true;

    QSqlQuery query(QString(SQL_DELETE_USERINFO).arg(username));
    if(query.lastError().isValid())
    {
        qDebug()<<query.lastError().text();
        result = false;
    }
    else
    {
       qDebug()<<SingletonConfig->getCurUser().append(QString::fromUtf8(" 删除用户 ")).append(username);
    }

    return result;
}

QString DataBaseHelper::getError()
{
    return sqlDatabase.lastError().text();
}


bool DataBaseHelper::modifyIaAlarmStatus(int id)
{
    bool status = false;

    QSqlQuery query(QString(SQL_MODIFY_IACONFIRM).arg(id));
    if(query.lastError().isValid())
    {
        qDebug()<<query.lastError().text();
    }
    else
    {
        status = true;
    }

    return status;
}

bool DataBaseHelper::modifyOaAlarmStatus(QString alarmTime, int id)
{
    bool status = false;
    QDate alarmDate = QDateTime::fromString(alarmTime, "yyyy-MM-dd hh:mm:ss.zzz").date();
    QSqlQuery query(QString(SQL_MODIFY_OACONFIRM).arg(alarmDate.toString("yyyy_MM_dd")).arg(id));
    if(query.lastError().isValid())
    {
       qDebug()<<query.lastError().text();
       qDebug()<<query.lastQuery();
    }
    else
    {
         status = true;
    }

    return status;
}


DataBaseHelper *DataBaseHelper::GetInstance()
{
    if(dbHelp_ == NULL)
    {
        dbHelp_ = new DataBaseHelper();
    }
    return dbHelp_;
}
