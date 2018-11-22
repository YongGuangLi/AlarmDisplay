#ifndef DEVICECOMMINTERRUPT_H
#define DEVICECOMMINTERRUPT_H

#include "alarmdata.h"
#include "alarmdetail.h"
#include <QWidget>
#include <QModelIndex>

namespace Ui {
class DeviceCommInterrupt;
}

class DeviceCommInterrupt : public QWidget
{
    Q_OBJECT
    
public:
    explicit DeviceCommInterrupt(QWidget *parent = 0);
    ~DeviceCommInterrupt();
signals:
    void increase(int);
    void decrease();
private slots:
    void dispDeviceCommInterrupt(stAlarmReport);
    void showAlarmDetail(QModelIndex index);
private:
    Ui::DeviceCommInterrupt *ui;
    AlarmDetail *alarmDetail;
};

#endif // DEVICECOMMINTERRUPT_H
