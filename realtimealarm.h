#ifndef REALTIMEALARM_H
#define REALTIMEALARM_H

#include "remotecontrolwidget.h"
#include "databasehelper.h"
#include "alarmdispconfig.h"
#include "alarmreportparse.h"
#include "lightindicator.h"
#include "filterbay.h"
#include "alarmdetail.h"

#include <string.h>

#include <QWidget>
#include <QPoint>
#include <QDesktopWidget>
#include <QMessageBox>
#include <QMenu>
#include <QMutexLocker>
#include <QtCore>
#include <QButtonGroup>
#include <QDateTime>

namespace Ui {
class RealTimeAlarm;
}

class RealTimeAlarm : public QWidget
{
    Q_OBJECT
    
public:
    explicit RealTimeAlarm(QWidget *parent = 0);
    ~RealTimeAlarm();

    void centerDisplay();

    void initWidgetDisp();

    void initSmartRadioButtons();

    void initOriginalRadioButtons();

private slots:
    void on_pushButton_alarmConfig_clicked();
    //接收并显示智能告警数据
    void dispSmartAlarmData(int, stAlarmData);
    //接收并显示原始告警数据
    void dispOriginalAlarmData(int, stAlarmData);

private slots:
    //告警表格右击菜单
    void showCustomMenu(QPoint);

    //告警表右击菜单槽函数
    void confirmAlarm();                  //确认告警
    void showAlarmDetail();               //告警详情
    void deleteAlarm();                   //删除告警

    //选择间隔过滤
    void selectFilterBay();

    //修改当前智能告警过滤等级
    void changeCurIaAlarmLevel();
    //修改当前原始告警过滤等级
    void changeCurOrAlarmLevel();

    //暂停智能告警数据显示
    void pauseSmartAlarmData(bool);
    //暂停原始告警数据显示
    void pauseOriginalAlarmData(bool);

    //由于告警颜色改变，间隔过滤，间隔屏蔽，修改过滤告警等级，重新显示
    void replaySmartAlarmData();
    //由于告警颜色改变，间隔过滤，间隔屏蔽，修改过滤告警等级，重新显示
    void replayOriginalAlarmData();

    //告警页面切换
    void alarmTabWidgetChanged(int);

private:
    Ui::RealTimeAlarm *ui;
    AlarmDispConfig *alarmDispConfig;

    int curSelectIaAlarmLevel;
    int curSelectOrAlarmLevel;

    QMenu *pSmartAlarmMenu;
    QMenu *pOriginalAlarmMenu;

    //用与修改告警表显示和插入之间保证数据同步
    QMutex mutex;

    QMap<int, QIcon> mapAlarmLevelLogo;

    FilterBay *pFilterBay;

    AlarmDetail *alarmDetail;

    bool isSmartAlarmPause;
    bool isOriginalAlarmPause;

    QButtonGroup smartButtonGroup;
    QButtonGroup originalButtonGroup;

    RemoteControlWidget *remoteControlWidget;
};

#endif // REALTIMEALARM_H
