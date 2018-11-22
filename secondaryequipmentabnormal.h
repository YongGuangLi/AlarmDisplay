#ifndef SECONDARYEQUIPMENTABNORMAL_H
#define SECONDARYEQUIPMENTABNORMAL_H

#include "alarmdata.h"
#include "alarmdetail.h"
#include <QWidget>
#include <QModelIndex>


namespace Ui {
class SecondaryEquipmentAbnormal;
}

class SecondaryEquipmentAbnormal : public QWidget
{
    Q_OBJECT
    
public:
    explicit SecondaryEquipmentAbnormal(QWidget *parent = 0);
    ~SecondaryEquipmentAbnormal();
signals:
    void increase(int);
    void decrease();

private slots:
    void dispSecondaryEquipmentAbnormal(stAlarmReport);
    void showAlarmDetail(QModelIndex index);
private:
    Ui::SecondaryEquipmentAbnormal *ui;
    AlarmDetail *alarmDetail;
};

#endif // SECONDARYEQUIPMENTABNORMAL_H
