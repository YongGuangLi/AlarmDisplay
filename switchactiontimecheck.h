#ifndef SWITCHACTIONTIMECHECK_H
#define SWITCHACTIONTIMECHECK_H

#include "alarmdata.h"
#include "alarmdetail.h"
#include <QWidget>
#include <QModelIndex>

namespace Ui {
class SwitchActionTimeCheck;
}

class SwitchActionTimeCheck : public QWidget
{
    Q_OBJECT
    
public:
    explicit SwitchActionTimeCheck(QWidget *parent = 0);
    ~SwitchActionTimeCheck();
signals:
    void increase(int);
    void decrease();
private slots:
    void dispSwitchActionTimeCheck(stAlarmReport);

    void showAlarmDetail(QModelIndex index);
private:
    Ui::SwitchActionTimeCheck *ui;

    AlarmDetail *alarmDetail;
};

#endif // SWITCHACTIONTIMECHECK_H
