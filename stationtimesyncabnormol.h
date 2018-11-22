#ifndef STATIONTIMESYNCABNORMOL_H
#define STATIONTIMESYNCABNORMOL_H

#include "alarmdata.h"
#include "alarmdetail.h"
#include <QWidget>
#include <QModelIndex>

namespace Ui {
class StationTimeSyncAbnormol;
}

class StationTimeSyncAbnormol : public QWidget
{
    Q_OBJECT
    
public:
    explicit StationTimeSyncAbnormol(QWidget *parent = 0);
    ~StationTimeSyncAbnormol();
signals:
    void increase(int);
    void decrease();
private slots:
    void dispStationTimeSyncAbnormol(stAlarmReport);

    void showAlarmDetail(QModelIndex index);
private:
    Ui::StationTimeSyncAbnormol *ui;
    AlarmDetail *alarmDetail;
};

#endif // STATIONTIMESYNCABNORMOL_H
