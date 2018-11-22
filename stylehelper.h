#ifndef STYLEHELPER_H
#define STYLEHELPER_H

#include <QFile>
#include <QApplication>
#include <QDebug>

class StyleHelper
{
public:
    static void setStyle(const QString &style)
    {
        QFile qss(style);
        if(qss.open(QFile::ReadOnly))
        {
            qApp->setStyleSheet(qss.readAll());
            qss.close();
        }
    }
};

#endif // STYLEHELPER_H
