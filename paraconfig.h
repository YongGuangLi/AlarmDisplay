#ifndef PARACONFIG_H
#define PARACONFIG_H

#include <QWidget>

namespace Ui {
class ParaConfig;
}

class ParaConfig : public QWidget
{
    Q_OBJECT
    
public:
    explicit ParaConfig(QWidget *parent = 0);
    ~ParaConfig();
    
private:
    Ui::ParaConfig *ui;
};

#endif // PARACONFIG_H
