#ifndef NETCONTROLANA_H
#define NETCONTROLANA_H

#include "configini.h"
#include "alarmdata.h"
#include "alarmdetail.h"
#include "remotecontrolwidget.h"

#include <QWidget>
#include <QCloseEvent>
#include <QDebug>
#include <QDesktopWidget>
#include <QPaintEvent>
#include <QTableWidgetItem>
#include <QPainter>
#include <QSplitter>
#include <QDomDocument>


namespace Ui {
class RemoteControlProcess;
}

class RemoteControlProcess : public QWidget
{
    Q_OBJECT

public:
    explicit RemoteControlProcess(QWidget *parent = 0);
    ~RemoteControlProcess();

    QString getCtrlResult(int);
signals:
    void increase(int);

    void decrease();

private slots:
    void dispRemoteControl(stAlarmReport);

    void showAlarmDetail(QModelIndex index);

private:
    Ui::RemoteControlProcess *ui;
    AlarmDetail *alarmDetail;
    RemoteControlWidget *remoteControlWidget;
};

#endif // NETCONTROLANA_H
