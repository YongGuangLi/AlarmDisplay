#ifndef STATISTICANALYSIS_H
#define STATISTICANALYSIS_H

#include "databasehelper.h"

#include <QWidget>
#include <QShowEvent>
#include <QDebug>
namespace Ui {
class StatisticAnalysis;
}

class StatisticAnalysis : public QWidget
{
    Q_OBJECT
    
public:
    explicit StatisticAnalysis(QWidget *parent = 0);
    ~StatisticAnalysis();

    void showEvent(QShowEvent *);
private slots:
    void queryStatisticAnalysis();
private:
    Ui::StatisticAnalysis *ui;
};

#endif // STATISTICANALYSIS_H
