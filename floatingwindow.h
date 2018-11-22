#ifndef FLOATINGWINDOW_H
#define FLOATINGWINDOW_H

#include "alarmdata.h"
#include <QWidget>
#include <QMouseEvent>
#include <QTimer>

namespace Ui {
class FloatingWindow;
}

class FloatingWindow : public QWidget
{
    Q_OBJECT
    
public:
    explicit FloatingWindow(QWidget *parent = 0);
    ~FloatingWindow();
    
    bool eventFilter(QObject *obj, QEvent *evt);
private slots:
    void dispAlarmCount();
private:
    Ui::FloatingWindow *ui;
    QTimer *timer;
};

#endif // FLOATINGWINDOW_H
