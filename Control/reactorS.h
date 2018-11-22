#ifndef REACTORS_H
#define REACTORS_H
#include "BaseClass.h"
#include <QtGui/QWidget>
#include <QtGui>
#include <QPainter>
#include <QtDesigner/QDesignerExportWidget>

class QDESIGNER_WIDGET_EXPORT reactorS: public BaseClass
{
    Q_OBJECT
    Q_PROPERTY(QString ClassNm READ GetClassNm WRITE SetClassNm)
public:
    reactorS(QWidget *parent = 0);
public:
    QString GetClassNm()const;
    void SetClassNm(QString strClassNm);
protected:
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
private:
    QString m_strClassNm;
signals:
    void dataChanged( QString value ,int iType = 0 );

};

#endif //REACTORS_H
