#ifndef PRIMARYEQUIPMENTABNORMAL_H
#define PRIMARYEQUIPMENTABNORMAL_H

#include "alarmdata.h"
#include "alarmdetail.h"
#include <QWidget>
#include <QModelIndex>


namespace Ui {
class PrimaryEquipmentAbnormal;
}

class PrimaryEquipmentAbnormal : public QWidget
{
    Q_OBJECT
    
public:
    explicit PrimaryEquipmentAbnormal(QWidget *parent = 0);
    ~PrimaryEquipmentAbnormal();
signals:
    void increase(int);
    void decrease();

private slots:
    void dispPrimaryEquipmentAbnormal(stAlarmReport);
    void showAlarmDetail(QModelIndex index);
private:
    Ui::PrimaryEquipmentAbnormal *ui;
    AlarmDetail *alarmDetail;
};

#endif // PRIMARYEQUIPMENTABNORMAL_H
