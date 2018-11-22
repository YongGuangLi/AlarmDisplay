#ifndef REMOTEMEASURING_H
#define REMOTEMEASURING_H

#include "alarmdata.h"
#include "alarmdetail.h"
#include <QWidget>
#include <QModelIndex>

namespace Ui {
class RemoteMeasuring;
}

class RemoteMeasuring : public QWidget
{
    Q_OBJECT
    
public:
    explicit RemoteMeasuring(QWidget *parent = 0);
    ~RemoteMeasuring();
signals:
    void increase(int);
    void decrease();
private slots:
    void dispRemoteMeasuring(stAlarmReport);
    void showAlarmDetail(QModelIndex index);
private:
    Ui::RemoteMeasuring *ui;
    AlarmDetail *alarmDetail;
};

#endif // REMOTEMEASURING_H
