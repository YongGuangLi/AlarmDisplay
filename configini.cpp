#include "configini.h"


ConfigIni* ConfigIni::configIni = NULL;

ConfigIni::ConfigIni(QObject *parent) : QObject(parent)
{
    Cur_User_ = "root";
}

ConfigIni::~ConfigIni()
{
    writeColorToFile();
}


bool ConfigIni::initConfigIni(QString filepath)
{
    if(!QFile::exists(filepath))
        qDebug()<<filepath + " not exist";

    settings_ = new QSettings(filepath,QSettings::IniFormat);

    settings_->beginGroup("MYSQL");
    ipMySql_ =  settings_->value("ip").toString();
    portMySql_ =  settings_->value("port").toInt();
    userMysql_ =  settings_->value("user").toString();
    passwdMysql_ =  settings_->value("passwd").toString();
    dbName_ = settings_->value("dbName").toString();
    settings_->endGroup();

    settings_->beginGroup("REDIS");
    ipRedis_ = settings_->value("ip").toString();
    portRedis = settings_->value("port").toInt();
    settings_->endGroup();

    settings_->beginGroup("PATH");
    ReportPath_ = QString::fromUtf8(settings_->value("IASReport").toString().toLatin1());
    settings_->endGroup();

    settings_->beginGroup("SIZE");
    iMaxSize_ = settings_->value("max").toInt();
    settings_->endGroup();

    settings_->beginGroup("COLOR");
    QStringList allAlarmLevelColor = settings_->allKeys();
    for(int i = 0; i < allAlarmLevelColor.size(); ++i)
    {
        QString colorName = settings_->value(allAlarmLevelColor.at(i)).toString();
        mapAlarmLevelColor_.insert(allAlarmLevelColor.at(i).toInt(), QColor(colorName));
    }
    settings_->endGroup();

    settings_->beginGroup("FLAG");
    QStringList allAlarmLevelFlag = settings_->allKeys();
    for(int i = 0; i < allAlarmLevelFlag.size(); ++i)
    {
        int flag = settings_->value(allAlarmLevelFlag.at(i)).toInt();
        mapAlarmLevelFlag_.insert(allAlarmLevelFlag.at(i).toInt(), flag);
    }
    settings_->endGroup();

    settings_->beginGroup("STATION");
    station_ = QString::fromUtf8(settings_->value("station").toString().toLatin1());
    settings_->endGroup();

    return true;
}

bool ConfigIni::initActionClassifier(QString filepath)
{
    QFile file(filepath);
    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug()<<file.fileName() << " open failed";
    }
    QDomDocument doc("myDocument");
    if (!doc.setContent(&file))
    {
        qDebug()<<file.fileName()<<" setContent failed";
        file.close();
        return false;
    }
    file.close();

    QDomElement docElem = doc.documentElement();

    QDomNode actions = docElem.firstChild();
    while(!actions.isNull())
    {
        QDomElement element = actions.toElement();
        if(!element.isNull())
        {
            if(element.tagName() == QString("actions"))
            {
                parseActions(element);
            }
        }
        actions = actions.nextSibling();
    }
    return true;
}


void ConfigIni::parseActions(QDomElement elem)
{
    QDomNode action = elem.firstChild();
    while(!action.isNull())
    {
        QDomElement element = action.toElement();
        if(!element.isNull())
        {
            if(element.tagName() == QString("action"))
            {
                parseItem(element);
            }
        }
        action = action.nextSibling();
    }
}

void ConfigIni::parseItem(QDomElement elem)
{
    QDomNode item = elem.firstChild();
    while(!item.isNull())
    {
        QDomElement element = item.toElement();
        if(!element.isNull())
        {
            if(element.tagName() == QString("item"))
            {
                mapActionClassifier_[element.attribute("desc")] = element.attribute("value").toInt();
            }
        }
        item = item.nextSibling();
    }
}

float ConfigIni::getActionClassifier(QString action)
{
    if(mapActionClassifier_.count(action))
    {
         return mapActionClassifier_[action];
    }
    else
    {
        qDebug()<<action;
        return action.toFloat();
    }
}

bool ConfigIni::initIABaySignal(QString filepath)
{
    QFile file(filepath);
    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug()<<file.fileName() << " open failed";
    }
    QDomDocument doc("myDocument");
    if (!doc.setContent(&file))
    {
        qDebug()<<file.fileName()<<" setContent failed";
        file.close();
        return false;
    }
    file.close();

    QDomElement docElem = doc.documentElement();

    QDomNode stations = docElem.firstChild();
    while(!stations.isNull())
    {
        QDomElement element = stations.toElement();
        if(!element.isNull())
        {
            if(element.tagName() == QString("station"))
            {
                parseStation(element);
            }
        }
        stations = stations.nextSibling();
    }
    return true;
}

void ConfigIni::parseStation(QDomElement elem)
{
    QDomNode voltage = elem.firstChild();
    while(!voltage.isNull())
    {
        QDomElement element = voltage.toElement();
        if(!element.isNull())
        {
            if(element.tagName() == QString("voltage"))
            {
                parseVoltage(element);
            }
        }
        voltage = voltage.nextSibling();
    }
}

void ConfigIni::parseVoltage(QDomElement elem)
{
    QDomNode item = elem.firstChild();
    while(!item.isNull())
    {
        QDomElement element = item.toElement();
        if(!element.isNull())
        {
            if(element.tagName() == QString("item"))
            {
                parseVoltageItem(element);

                if(element.hasAttribute("name"))
                {
                    QString bayName = element.attribute("name");
                    SingletonConfig->getBay(bayName);
                }
            }
        }
        item = item.nextSibling();
    }
}

void ConfigIni::parseVoltageItem(QDomElement elem)
{
    QDomNode device = elem.firstChild();
    while(!device.isNull())
    {
        QDomElement element = device.toElement();
        if(!element.isNull())
        {
            if(element.tagName() == QString("device"))
            {
                parseDevice(element);
            }
        }
        device = device.nextSibling();
    }
}

void ConfigIni::parseDevice(QDomElement elem)
{
    QDomNode dots = elem.firstChild();
    while(!dots.isNull())
    {
        QDomElement element = dots.toElement();
        if(!element.isNull())
        {
            if(element.tagName() == QString("dots"))
            {
                parseDots(element);
            }
        }
        dots = dots.nextSibling();
    }
}

void ConfigIni::parseDots(QDomElement elem)
{
    QDomNode dot = elem.firstChild();
    while(!dot.isNull())
    {
        QDomElement element = dot.toElement();
        if(!element.isNull())
        {
            if(element.tagName() == QString("dot"))
            {
                if(element.attribute("priority") == "0")
                    listEventSignal_.push_back(element.attribute("redisAddress"));
            }
        }
        dot = dot.nextSibling();
    }
}

bool ConfigIni::initIAFRule(QString filepath)
{
    QFile file(filepath);
    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug()<<file.fileName() << " open failed";
    }
    QDomDocument doc("myDocument");
    if (!doc.setContent(&file))
    {
        qDebug()<<file.fileName()<<" setContent failed";
        file.close();
        return false;
    }
    file.close();

    QDomElement docElem = doc.documentElement();

    QDomNode IAFRule = docElem.firstChild();
    while(!IAFRule.isNull())
    {
        QDomElement element = IAFRule.toElement();
        if(!element.isNull())
        {
            if(element.tagName() == QString("item"))
            {
                parseIAFRuleItem(element);
            }
        }
        IAFRule = IAFRule.nextSibling();
    }
    return true;
}

void ConfigIni::parseIAFRuleItem(QDomElement elem)
{
    QDomNode logic = elem.firstChild();
    while(!logic.isNull())
    {
        QDomElement element = logic.toElement();
        if(!element.isNull())
        {
            if(element.tagName() == QString("logic"))
            {
                if(elem.hasAttribute("code"))
                    mapCodeToLogic_[elem.attribute("code")] = element.text();
            }
        }
        logic = logic.nextSibling();
    }
}

bool ConfigIni::initIASRule(QString filepath)
{
    QFile file(filepath);
    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug()<<file.fileName() << " open failed";
    }
    QDomDocument doc("myDocument");
    if (!doc.setContent(&file))
    {
        qDebug()<<file.fileName()<<" setContent failed";
        file.close();
        return false;
    }
    file.close();

    QDomElement docElem = doc.documentElement();

    QDomNode IASRule = docElem.firstChild();
    while(!IASRule.isNull())
    {
        QDomElement element = IASRule.toElement();
        if(!element.isNull())
        {
            if(element.tagName() == QString("item"))
            {
                parseIASRuleItem(element);
            }
        }
        IASRule = IASRule.nextSibling();
    }
    return true;
}

void ConfigIni::parseIASRuleItem(QDomElement elem)
{
    QString logic;
    QString type = elem.attribute("type");
    if(type == QString::fromLocal8Bit("延时告警"))
    {
        logic = QString::fromLocal8Bit("类型:延时告警") + QString::fromLocal8Bit("   有效时间:%1秒").arg(elem.attribute("validTime"));
    }else if(type == QString::fromLocal8Bit("抖动监视"))
    {
        logic = QString::fromLocal8Bit("类型:抖动监视") + QString::fromLocal8Bit("   判定时间:%1秒").arg(elem.attribute("judgeTime")) +
                QString::fromLocal8Bit("   判定次数:%1").arg(elem.attribute("count"));
    }

    mapCodeToLogic_[elem.attribute("code")] = logic;

}




void ConfigIni::setRightInfo(QString rightName, stRightInfo* rightInfo)
{
    mapRightInfo_[rightName] = rightInfo;
}

stRightInfo *ConfigIni::getRightInfo(QString rightName)
{
    return  mapRightInfo_[rightName];
}

stRightInfo *ConfigIni::getRightInfo(int rightID)
{
    stRightInfo *pRightInfo = NULL;
    QList<stRightInfo*> listRightInfo = mapRightInfo_.values();
    for(int i = 0; i < listRightInfo.size(); ++i)
    {
        pRightInfo = listRightInfo.at(i);
        if(pRightInfo->rightID == rightID)
        {
            break;
        }
    }
    return pRightInfo;
}

QMap<QString, stRightInfo*> ConfigIni::getRightInfo() const
{
    return mapRightInfo_;
}


bool ConfigIni::isCurrentUserhasRight(int rightID)
{
    stUserInfo * pUserInfo = this->getUserInfo(Cur_User_);
    if(pUserInfo == NULL)
    {
        QMessageBox::warning( NULL, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("请登录用户"), QMessageBox::Yes);
        return false;
    }

    stRightInfo * pRightInfo = this->getRightInfo(rightID);
    int currentUserGroupId = this->getUserGroupId(pUserInfo->groupName);
    if(currentUserGroupId == -1)
    {
        QMessageBox::warning( NULL, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("用户组已删除"), QMessageBox::Yes);
        return false;
    }

    if(((pRightInfo->groupFlag >> currentUserGroupId) & 0x0001) == 1)
    {
        return true;
    }
    else
    {
        QMessageBox::warning(NULL, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("该用户无权限"), QMessageBox::Yes);
        return false;
    }
}


void ConfigIni::writeColorToFile()
{
    QMap<int,QColor>::iterator itColor = mapAlarmLevelColor_.begin();
    for( ; itColor != mapAlarmLevelColor_.end(); ++itColor)
    {
        settings_->setValue("COLOR/" + QString::number(itColor.key()), itColor.value().name());
    }

    QMap<int,int>::iterator itFlag = mapAlarmLevelFlag_.begin();
    for( ; itFlag != mapAlarmLevelFlag_.end(); ++itFlag)
    {
        settings_->setValue("FLAG/" + QString::number(itFlag.key()), QString::number(itFlag.value()));
    }

    settings_->sync();
}

void ConfigIni::getBay(QString bayName)
{
    if(listBay_.contains(bayName))
        return;

    listBay_.push_back(bayName);
}


QStringList ConfigIni::getListBay()
{
    return listBay_;
}


int ConfigIni::getMaxSize() const
{
    return iMaxSize_;
}

QColor ConfigIni::getAlarmLevelColor(int level)
{
    return mapAlarmLevelColor_[level];
}

void ConfigIni::setAlarmLevelColor(int level, QColor color)
{
    mapAlarmLevelColor_[level] = color;
}


void ConfigIni::setUserInfo(QString username, stUserInfo *userInfo)
{
    mapUserInfo_[username] = userInfo;
}

stUserInfo *ConfigIni::getUserInfo(QString username)
{
    return mapUserInfo_[username];
}


QMap<QString, stUserInfo*> ConfigIni::getUserInfo() const
{
    return mapUserInfo_;
}



int ConfigIni::getAlarmLevelFlag(int level)
{
    return mapAlarmLevelFlag_[level];
}

void ConfigIni::setAlarmLevelFlag(int level, int flag)
{
    mapAlarmLevelFlag_[level] = flag;
}


QString ConfigIni::getReportPath() const
{
    return ReportPath_;
}

QString ConfigIni::getDbName() const
{
    return dbName_;
}

void ConfigIni::setCurUser(const QString &Cur_User)
{
    Cur_User_ = Cur_User;
}

ConfigIni *ConfigIni::GetInstance()
{
    if(configIni == NULL)
    {
        configIni = new ConfigIni();
    }
    return configIni;
}

QString ConfigIni::getIpMySql() const
{
    return ipMySql_;
}

int ConfigIni::getPortMySql() const
{
    return portMySql_;
}

QString ConfigIni::getIpRedis() const
{
    return ipRedis_;
}

int ConfigIni::getPortRedis() const
{
    return portRedis;
}

QString ConfigIni::getStation() const
{
    return station_;
}

QString ConfigIni::getCurUser() const
{
    return Cur_User_;
}

QString ConfigIni::getLogicByCode(QString code)
{
    return mapCodeToLogic_.value(code, code + QString::fromLocal8Bit(" 不存在"));
}


bool ConfigIni::existUserGroup(QString groupName)
{
    return mapUserGroup_.keys().contains(groupName);
}


void ConfigIni::setUserGroup(QString groupName, int id)
{
    mapUserGroup_[groupName] = id;
}

QMap<QString, int> ConfigIni::getUserGroup() const
{
    return mapUserGroup_;
}

int ConfigIni::getUserGroupId(QString groupName) const
{
    return mapUserGroup_.value(groupName , -1);
}

int ConfigIni::getMaxGroupId()
{
    QList<int> listId = mapUserGroup_.values();

    qSort(listId.begin(), listId.end());

    return listId.at(listId.size() - 1) + 1;
}

QString ConfigIni::getUserMysql() const
{
    return userMysql_;
}

QString ConfigIni::getPasswdMysql() const
{
    return passwdMysql_;
}

bool ConfigIni::IsUserExits(QString userName)
{
    if(mapUserInfo_.count(userName) == 0)
    {
        return false;
    }
    return true;
}

bool ConfigIni::CheckUserInfo(QString userName, QString passWord)
{
    if(passWord == mapUserInfo_[userName]->userPassWd)
    {
        return true;
    }
    else
    {
        return false;
    }
}
