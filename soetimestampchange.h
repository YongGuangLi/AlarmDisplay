#ifndef SOETIMESTAMPCHANGE_H
#define SOETIMESTAMPCHANGE_H

#include "alarmdata.h"
#include "alarmdetail.h"
#include <QWidget>
#include <QModelIndex>

namespace Ui {
class SoeTimestampChange;
}

class SoeTimestampChange : public QWidget
{
    Q_OBJECT
    
public:
    explicit SoeTimestampChange(QWidget *parent = 0);
    ~SoeTimestampChange();
signals:
    void increase(int);
    void decrease();
private slots:
    void dispSoeTimestampChange(stAlarmReport);

    void showAlarmDetail(QModelIndex index);
private:
    Ui::SoeTimestampChange *ui;
    AlarmDetail *alarmDetail;
};

#endif // SOETIMESTAMPCHANGE_H
