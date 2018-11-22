#ifndef REMOTESIGNALLING_H
#define REMOTESIGNALLING_H

#include "alarmdata.h"
#include "alarmdetail.h"
#include <QWidget>
#include <QModelIndex>

namespace Ui {
class RemoteSignalling;
}

class RemoteSignalling : public QWidget
{
    Q_OBJECT
    
public:
    explicit RemoteSignalling(QWidget *parent = 0);
    ~RemoteSignalling();
signals:
    void increase(int);
    void decrease();
private slots:
    void dispRemoteSignalling(stAlarmReport);

    void showAlarmDetail(QModelIndex index);
private:
    Ui::RemoteSignalling *ui;
    AlarmDetail *alarmDetail;
};

#endif // REMOTESIGNALLING_H
