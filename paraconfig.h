#ifndef PARACONFIG_H
#define PARACONFIG_H

#include <QWidget>
#include <QList>
#include <QLineEdit>
#include <QDebug>

#include "RedisHelper.h"

namespace Ui {
class ParaConfig;
}

class ParaConfig : public QWidget
{
    Q_OBJECT
    
public:
    explicit ParaConfig(QWidget *parent = 0);
    ~ParaConfig();
    
    void init();
private slots:
    void on_pushButton_save_clicked();
    void slot_modifyConnStatus(QString, int);

private:
    Ui::ParaConfig *ui;
    QList<QLineEdit*> listLineEdit;
};

#endif // PARACONFIG_H
