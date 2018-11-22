#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>
#include <QDesktopWidget>
#include <QKeyEvent>
#include "configini.h"

namespace Ui {
class LoginWindow;
}

class LoginWindow : public QWidget
{
    Q_OBJECT
    
public:
    explicit LoginWindow(QWidget *parent = 0);
    ~LoginWindow();
    void centerDisplay();

    void keyPressEvent(QKeyEvent  *event);
signals:
    void userLogin(QString);
private slots:
    void on_pushButton_Login_clicked();

private:
    Ui::LoginWindow *ui;
};

#endif // LOGINWINDOW_H
