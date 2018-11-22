#ifndef ELECTRICRUNCHECK_H
#define ELECTRICRUNCHECK_H

#include "alarmdata.h"
#include "alarmdetail.h"
#include <QWidget>
#include <QModelIndex>

namespace Ui {
class ElectricRunCheck;
}

class ElectricRunCheck : public QWidget
{
    Q_OBJECT
    
public:
    explicit ElectricRunCheck(QWidget *parent = 0);
    ~ElectricRunCheck();
signals:
    void increase(int);
    void decrease();
private slots:
    void dispElectricRunCheck(stAlarmReport alarmReport);

    void showAlarmDetail(QModelIndex index);
private:
    Ui::ElectricRunCheck *ui;
    AlarmDetail *alarmDetail;
};

#endif // ELECTRICRUNCHECK_H
