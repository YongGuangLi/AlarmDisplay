#ifndef CONFIGURATIONINTERFACE_H
#define CONFIGURATIONINTERFACE_H

#include "Base.h"
#include "analysisui.h"
#include <QStackedWidget>
#include <QDebug>
#include <QtUiTools/QUiLoader>
#include <QComboBox>
#include <QDir>

namespace Ui {
class ConfigurationInterface;
}

class ConfigurationInterface : public QStackedWidget
{
    Q_OBJECT
    
public:
    explicit ConfigurationInterface(QWidget *parent = 0);
    ~ConfigurationInterface();
private slots:
    void receiveControlData(QString, QString);
    void ChangeUiFile(QString);
private:
    Ui::ConfigurationInterface *ui;
    Base *pBase;
    QComboBox *comboBox;
};

#endif // CONFIGURATIONINTERFACE_H
