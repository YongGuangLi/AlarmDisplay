#ifndef SWITCHSTATUSCHECK_H
#define SWITCHSTATUSCHECK_H

#include "alarmdata.h"
#include "alarmdetail.h"
#include <QWidget>
#include <QDebug>
#include <QModelIndex>

namespace Ui {
class SwitchStatusCheck;
}

class SwitchStatusCheck : public QWidget
{
    Q_OBJECT
    
public:
    explicit SwitchStatusCheck(QWidget *parent = 0);
    ~SwitchStatusCheck();
signals:
    void increase(int);
    void decrease();

private slots:
    void dispSwitchStatusCheck(stAlarmReport);

    void showAlarmDetail(QModelIndex);
private:
    Ui::SwitchStatusCheck *ui;
    AlarmDetail *alarmDetail;
};

#endif // SWITCHSTATUSCHECK_H
