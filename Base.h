#ifndef BASE_H
#define BASE_H
#include "analysisui.h"
#include "IncludeFile.h"

class Base : public QWidget
{
    Q_OBJECT

public:
    Base(QWidget *parent);
    ~Base();
public slots:
    void ReceiveMessage(QString strControlId, QString strControlVal);
    void ReceiveMessage(QString strResult);
    void SetRemoteControlInfo(QString strId, int iVal);
signals:
    void MessageBox(QString strMsg);
public:
    AnalysisUi * m_AnalysisUi;
    SubscribeKey m_SubscribeKey;
#ifdef MESSAGEQUEUE
    MessageQueueDataSource * m_pMsgDataSrc;
#else
   // RTDBReadThread * m_RedisDataSrc;
#endif
    QMap<QString, QStringList> m_ControlAndProperty;
    QList<StructToControl> m_StructToControl;
    string m_strServerAddr;
    string m_strServerPort;

public:
    QMap<QString, int> GetFolderAllUiFile();
    QStackedWidget * LoadUiFile(QWidget *pWidget);
    int LinkDataSource();
    int DisconnectDataSource();
    void GetServerAddress();
    int ControlAndProperty();
    int GetStructToControl();
    void SetControlVal(string strControlId, string strControlVal);
    int GetSubscribeKey();
    bool GetLinkState();
    void ConnectRelease();
    
};

#endif // BASE_H
