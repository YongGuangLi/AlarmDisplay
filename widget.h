#ifndef WIDGET_H
#define WIDGET_H

#include "RedisHelper.h"
#include "databasehelper.h"
#include "RtdbMessage.pb.h"
#include "configini.h"

#include "floatingwindow.h"
#include "historyquery.h"
#include "realtimealarm.h"
#include "loginwindow.h"
#include "configurationinterface.h"
#include "remotecontrolprocess.h"
#include "switchstatuscheck.h"
#include "remotesignalling.h"
#include "remotemeasuring.h"
#include "singlebayprotectaction.h"
#include "soetimestampchange.h"
#include "stationtimesyncabnormol.h"
#include "devicecomminterrupt.h"
#include "primaryequipmentabnormal.h"
#include "secondaryequipmentabnormal.h"
#include "switchactiontimecheck.h"
#include "powersystemabnormal.h"
#include "electricruncheck.h"
#include "auxiliarypowercheck.h"
#include "statisticanalysis.h"
#include "userconfig.h"
#include "pushbutton.h"
#include "aboutdialog.h"
#include "paraconfig.h"

#include <QWidget>
#include <QtCore>
#include <QDesktopWidget>
#include <QDebug>
#include <QDockWidget>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT
    
public:
    explicit Widget(QWidget *parent = 0);
    void iniAlarmTableWidget();
    void initButtonConnections();             //初始化导航栏按键信号连接
    void initAlarmButtonsConnects();          //初始化首页告警按键信号连接
    void logoutCurUser();
    ~Widget();
public:
   void centerDisplay();                         //窗口居中显示

   bool eventFilter(QObject *, QEvent *);
private slots:
   void switchCurrentPage();                     //改变当前导航栏显示页面
   void switchAlarmPage();                       //改变当前告警页面，显示不同告警表

   void setCurUser(QString);

   void clickAbout();

   void dispSmartAlarmData(int, stAlarmData);
   void dispOriginalAlarmData(int, stAlarmData);
private:
    void redisSubscribe();         //redis订阅
signals:
    //实时点值,更改组态界面控件值
    void sendControlData(QString, QString);

private:
    Ui::Widget *ui;
    RedisHelper *redisHelper;

    bool isRunning;
    AboutDialog *aboutDialog;
    HistoryQuery *historyQuery;
    RealTimeAlarm *realTimeAlarm;
    LoginWindow *loginWindow;
    UserConfig *userConfig;
    ConfigurationInterface *configurationInterface;
    RemoteControlProcess *remoteControlProcess;
    SwitchStatusCheck *switchStatusCheck;
    RemoteSignalling *remoteSignalling;
    RemoteMeasuring *remoteMeasuring;
    SingleBayProtectAction *singleBayProtectAction;
    SoeTimestampChange *soeTimestampChange;
    StationTimeSyncAbnormol *stationTimeSyncAbnormol;
    DeviceCommInterrupt *deviceCommInterrupt;
    SecondaryEquipmentAbnormal *secondaryEquipmentAbnormal;
    PrimaryEquipmentAbnormal *primaryEquipmentAbnormal;
    SwitchActionTimeCheck *switchActionTimeCheck;
    PowerSystemAbnormal *powerSystemAbnormal;
    ElectricRunCheck *electricRunCheck;
    AuxiliaryPowerCheck *auxiliaryPowerCheck;
    StatisticAnalysis *statisticAnalysis;
    ParaConfig *paraConfig;
private:
    PushButton *pushButton_SwitchStatusCheck;
    PushButton *pushButton_PrimaryEquipmentAbnormal;
    PushButton *pushButton_SecondaryEquipmentAbnormal;
    PushButton *pushButton_SwitchActionTimeCheck;
    PushButton *pushButton_SingleBayProtectAction;
    PushButton *pushButton_PowerSystemAbnormal;
    PushButton *pushButton_ElectricRunCheck;
    PushButton *pushButton_AuxiliaryPowerCheck;
    PushButton *pushButton_RemoteSignalling;
    PushButton *pushButton_RemoteControlProcess;
    PushButton *pushButton_StationTimeSyncAbnormol;
    PushButton *pushButton_DeviceCommInterrupt;
    PushButton *pushButton_SoeTimestampChange;
    PushButton *pushButton_RemoteMeasuring;
    FloatingWindow *floatingWindow;
};

#endif // WIDGET_H
