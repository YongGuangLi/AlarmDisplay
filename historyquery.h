#ifndef HISTORYQUERY_H
#define HISTORYQUERY_H


#include "remotecontrolwidget.h"
#include "alarmdetail.h"
#include "databasehelper.h"
#include <QWidget>
#include <QLineEdit>
#include <QDebug>
#include <QFile>
#include <QDateTime>
#include <QTextCodec>
#include <QTextStream>
#include <QFileDialog>

namespace Ui {
class HistoryQuery;
}

class HistoryQuery : public QWidget
{
    Q_OBJECT
    
public:
    explicit HistoryQuery(QWidget *parent = 0);
    ~HistoryQuery();
private:
    bool eventFilter(QObject *, QEvent *);
    bool judgeBayValid(QString);

private slots:
    void on_pushButton_query_clicked();
    void dispSmartAlarmData(int, stAlarmData);
    void exportHistoryData();

    void showAlarmDetail(QModelIndex);
private:
    Ui::HistoryQuery *ui;
    AlarmDetail *alarmDetail;
    RemoteControlWidget *remoteControlWidget;
};

#endif // HISTORYQUERY_H
