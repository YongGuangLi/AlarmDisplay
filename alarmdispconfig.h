#ifndef ALARMDISPCONFIG_H
#define ALARMDISPCONFIG_H

#include "configini.h"
#include"alarmdata.h"

#include <QWidget>
#include <QDesktopWidget>
#include <QColorDialog>
#include <QButtonGroup>
#include <QDebug>
#include <QCloseEvent>
#include <QShowEvent>

namespace Ui {
class AlarmDispConfig;
}

class AlarmDispConfig : public QWidget
{
    Q_OBJECT
    
public:
    explicit AlarmDispConfig(QWidget *parent = 0);
    ~AlarmDispConfig();

    void centerDisplay();

    void initButtonConnections();

    void initCheckConnections();

    void showEvent(QShowEvent *);
signals:
    void alarmLevelColorChange();
private slots:
    void changeAlarmLevelColor();              //改变告警等级颜色

    void on_pushButton_confirm_clicked();     //确认修改配置

private:
    Ui::AlarmDispConfig *ui;
    QButtonGroup buttonGroup;        //修改颜色按键
    QButtonGroup checkGroup;         //修改告警弹框设置
};

#endif // ALARMDISPCONFIG_H
