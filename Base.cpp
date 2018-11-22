#include "Base.h"

Base::Base(QWidget *parent)
    : QWidget(parent)
{
    m_AnalysisUi = NULL;
    m_AnalysisUi = new AnalysisUi();
    if (m_AnalysisUi != NULL)
    {
        m_AnalysisUi->SetUiFilePath(QApplication::applicationDirPath() + "/UiConfiguration/");
        m_AnalysisUi->GetFolderAllFile();
        
    }

    GetServerAddress();
    GetSubscribeKey();
#ifdef MESSAGEQUEUE
    m_pMsgDataSrc = new MessageQueueDataSource(this);
    connect(m_pMsgDataSrc,SIGNAL(SetControlData(QStringQString)),this,SLOT(ReceiveMessage(QString,QString)));
    connect(m_pMsgDataSrc,SIGNAL(SetControlResult(QString)),this,SLOT(ReceiveMessage(QString)));
    
#else
//    m_RedisDataSrc = RTDBReadThread::Instance();
//    connect(m_RedisDataSrc,SIGNAL(SetControlData(QString,QString)),this,SLOT(ReceiveMessage(QString,QString)));
//    connect(m_RedisDataSrc,SIGNAL(SetControlResult(QString)),this,SLOT(ReceiveMessage(QString)));
#endif

//    connect(m_AnalysisUi->pOptionDlg->dialog,SIGNAL(SendRemoteControlProcessInfo(QString,int)),this, SLOT( SetRemoteControlProcessInfo(QString, int)));

    ControlAndProperty();

    GetStructToControl();
    m_AnalysisUi->SetStructToControl(m_StructToControl);
}

Base::~Base()
{
    if (m_AnalysisUi != NULL)
    {
        delete m_AnalysisUi;
        m_AnalysisUi = NULL;
    }

#ifdef MESSAGEQUEUE
    if(m_pMsgDataSrc != NULL)
    {
        delete m_pMsgDataSrc;
        m_pMsgDataSrc = NULL;
    }
#else
   
   ////退出线程
   //if(m_RedisDataSrc != NULL)
   // {
   //     delete m_RedisDataSrc;
   //     m_RedisDataSrc = NULL;
   // }
#endif

}

void Base::SetRemoteControlInfo(QString strId, int iVal)
{
#ifdef MESSAGEQUEUE
    m_pMsgDataSrc->SendControlCmd(strId, iVal);
#else
    //m_RedisDataSrc->SendControlCmd(strId, iVal);
#endif
}

void Base::ReceiveMessage(QString strResult)
{
    emit MessageBox(strResult);
}

void Base::ReceiveMessage(QString strControlId, QString strControlVal)
{
    SetControlVal(strControlId.toStdString(), strControlVal.toStdString());
}

void Base::SetControlVal(string strControlId, string strControlVal)
{
    m_AnalysisUi->SetControlVal(strControlId,strControlVal);
}

QMap<QString, int> Base::GetFolderAllUiFile()
{
    return m_AnalysisUi->GetUIFileName();
}

QStackedWidget * Base::LoadUiFile(QWidget *pWidget)
{
    return m_AnalysisUi->LoadUiFile(pWidget);
}

int Base::LinkDataSource()
{
#ifdef MESSAGEQUEUE
    m_pMsgDataSrc->Init(m_SubscribeKey,m_strServerAddr,atoi(m_strServerPort.c_str()));
    m_pMsgDataSrc->start();
#else
    //m_RedisDataSrc->Init(m_SubscribeKey,m_strServerAddr,atoi(m_strServerPort.c_str()));
    //m_RedisDataSrc->start();
#endif
    return 0;
}

int Base::DisconnectDataSource()
{
#ifdef MESSAGEQUEUE
    m_pMsgDataSrc->ConnectRelease();
#else
    //m_RedisDataSrc->ConnectRelease();
#endif
    return 0;
}
bool Base::GetLinkState()
{
    bool bRet = false;
#ifdef MESSAGEQUEUE
    bRet =  m_pMsgDataSrc->GetConnentState();
#else
    //bRet =  m_RedisDataSrc->GetConnentState();
#endif
    return bRet;
}

void Base::GetServerAddress()
{
    QString ServerIP= "";
    QString ServerPort="";
    RWIniFile::readIni(QString("Config.ini"),QString("ServerInfo"),QString("ServerIp"),ServerIP);
    RWIniFile::readIni(QString("Config.ini"),QString("ServerInfo"),QString("ServerPort"),ServerPort); 
    m_strServerAddr = ServerIP.toStdString();
    m_strServerPort = ServerPort.toStdString();
}
int Base::ControlAndProperty()
{
    QString strControlNum="";
    strControlNum =  RWIniFile::readIni(QString("Config.ini"),QString("ControlInfo"),QString("Count"),strControlNum);
    //qDebug()<< strControlNum;
    if (strControlNum.toInt() <= 0)
    {
        return -1;
    }
    for (int i = 1; i <= strControlNum.toInt(); i++)
    {
        QString strKey = QString("Control") + QString::number(i,10);
        RWIniFile::readIni("Config.ini","ControlInfo",strKey,strControlNum);

        QStringList DataList = strControlNum.split(";");
        if (DataList.size() != 2)
        {
            return -2;
        }
        QStringList PropertyList = DataList.at(1).split(",");
        m_ControlAndProperty[DataList.at(0)] = PropertyList;
    }
    return 0;
}

int Base::GetStructToControl()
{
    m_StructToControl.clear();
    QString strControlNum="";
    strControlNum =  RWIniFile::readIni(QString("Config.ini"),QString("StructToControl"),QString("Count"),strControlNum);
   // qDebug()<< strControlNum;
    if (strControlNum.toInt() <= 0)
    {
        return -1;
    }
    for (int i = 1; i <= strControlNum.toInt(); i++)
    {
        QString strVal ="";
        QString strKey = QString("Key") + QString::number(i,10);
        RWIniFile::readIni("Config.ini","StructToControl",strKey,strVal);

        QStringList DataList = strVal.split(".");
        if (DataList.size() != 6)
        {
            return -2;
        }
        StructToControl info;
        info.strControlMsgType = DataList.at(0).toStdString();
        info.strControlIdField = DataList.at(1).toStdString();
        info.strControlValue = DataList.at(2).toStdString();
        info.strControlClassName = DataList.at(3).toStdString();
        info.strUiControlProportValue = DataList.at(4).toStdString();
        info.strUiControlProportValuetype = DataList.at(5).toStdString();
        m_StructToControl.append(info);

    }
    return 0;
}

int Base::GetSubscribeKey()
{
    m_SubscribeKey.strKey.clear();
    QString strKeyNum="";
    strKeyNum =  RWIniFile::readIni(QString("Config.ini"),QString("SubscribeKey"),QString("KeyNumber"),strKeyNum);
    //qDebug()<< strKeyNum;
    if (strKeyNum.toInt() <= 0)
    {
        return -1;
    }
    for (int i = 1; i <= strKeyNum.toInt(); i++)
    {
        QString strKey = QString("Key") + QString::number(i,10);
        QString strKeyVal = "";
        RWIniFile::readIni("Config.ini","SubscribeKey",strKey,strKeyVal);
        
       // qDebug() << strKeyVal;

        m_SubscribeKey.strKey.push_front(strKeyVal.toStdString());
    }
    return 0;
}

void Base::ConnectRelease()
{
#ifdef MESSAGEQUEUE
    m_pMsgDataSrc->ConnectRelease();
#else
    //m_RedisDataSrc->ConnectRelease();
#endif
}
