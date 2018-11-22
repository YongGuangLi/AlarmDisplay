#ifndef ARRESTER_H
#define ARRESTER_H
#include "BaseClass.h"
#include <QtGui/QWidget>
#include <QtGui>
#include <QPainter>
#include <QtDesigner/QDesignerExportWidget>

class QDESIGNER_WIDGET_EXPORT arrester: public BaseClass
{
    Q_OBJECT
    Q_PROPERTY(QString ClassNm READ GetClassNm WRITE SetClassNm)
public:
    arrester(QWidget *parent = 0);
public:
 

    QString GetClassNm()const;
    void SetClassNm(QString strClassNm);
protected:
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
private:
   
    QString m_strClassNm;
    int iSizeFont ;
    QPainterPath path;

signals:
    void dataChanged( QString value ,int iType = 0 );

};

#endif //ARRESTER_H
