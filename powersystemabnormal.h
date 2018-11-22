#ifndef POWERSYSTEMABNORMAL_H
#define POWERSYSTEMABNORMAL_H

#include "alarmdata.h"
#include "alarmdetail.h"
#include <QWidget>
#include <QModelIndex>

namespace Ui {
class PowerSystemAbnormal;
}

class PowerSystemAbnormal : public QWidget
{
    Q_OBJECT
    
public:
    explicit PowerSystemAbnormal(QWidget *parent = 0);
    ~PowerSystemAbnormal();
signals:
    void increase(int);
    void decrease();
private slots:
    void dispPowerSystemAbnormal(stAlarmReport alarmReport);

    void showAlarmDetail(QModelIndex index);
private:
    Ui::PowerSystemAbnormal *ui;
    AlarmDetail *alarmDetail;
};

#endif // POWERSYSTEMABNORMAL_H
