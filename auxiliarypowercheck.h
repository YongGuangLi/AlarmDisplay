#ifndef AUXILIARYPOWERCHECK_H
#define AUXILIARYPOWERCHECK_H

#include "alarmdata.h"
#include "alarmdetail.h"
#include <QWidget>
#include <QModelIndex>

namespace Ui {
class AuxiliaryPowerCheck;
}

class AuxiliaryPowerCheck : public QWidget
{
    Q_OBJECT
    
public:
    explicit AuxiliaryPowerCheck(QWidget *parent = 0);
    ~AuxiliaryPowerCheck();
signals:
    void increase(int);
    void decrease();
private slots:
    void dispAuxiliaryPowerCheck(stAlarmReport);
    void showAlarmDetail(QModelIndex index);
private:
    Ui::AuxiliaryPowerCheck *ui;
    AlarmDetail *alarmDetail;
};

#endif // AUXILIARYPOWERCHECK_H
