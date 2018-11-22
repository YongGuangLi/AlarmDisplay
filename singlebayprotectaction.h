#ifndef SINGLEBAYPROTECTACTION_H
#define SINGLEBAYPROTECTACTION_H

#include "alarmdata.h"
#include "alarmdetail.h"
#include "HorizontalGraph.h"

#include <QWidget>
#include <QModelIndex>

namespace Ui {
class SingleBayProtectAction;
}

class SingleBayProtectAction : public QWidget
{
    Q_OBJECT
    
public:
    explicit SingleBayProtectAction(QWidget *parent = 0);
    ~SingleBayProtectAction();

signals:
    void increase(int);
    void decrease();
private slots:
    void dispSingleBayProtectAction(stAlarmReport);
    void showAlarmDetail(QModelIndex);
private:
    Ui::SingleBayProtectAction *ui;
    AlarmDetail *alarmDetail;
};

#endif // SINGLEBAYPROTECTACTION_H
