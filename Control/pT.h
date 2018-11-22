#ifndef PT_H
#define PT_H
#include "BaseClass.h"
#include <QtGui/QWidget>
#include <QtGui>
#include <QPainter>
#include <QtDesigner/QDesignerExportWidget>

class QDESIGNER_WIDGET_EXPORT pT: public BaseClass
{
    Q_OBJECT
    Q_PROPERTY(QString ClassNm READ GetClassNm WRITE SetClassNm)
public:
    pT(QWidget *parent = 0);
public:
    QString GetClassNm()const;
    void SetClassNm(QString strClassNm);
    void gradientArc(QPainter *painter, int radius, int startAngle, int angleLength, int arcHeight, QRgb color);
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

#endif //PT_H
