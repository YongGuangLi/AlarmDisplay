#ifndef CT_H
#define CT_H
#include "BaseClass.h"
#include <QtGui/QWidget>
#include <QtGui>
#include <QPainter>
#include <QtDesigner/QDesignerExportWidget>

class QDESIGNER_WIDGET_EXPORT cT: public BaseClass
{
    Q_OBJECT
    Q_PROPERTY(BaseClass::EnumState State READ GetState WRITE SetState)
    Q_PROPERTY(QString ClassNm READ GetClassNm WRITE SetClassNm)
public:
    cT(QWidget *parent = 0);
public:
    BaseClass::EnumState GetState() const;
    void SetState(BaseClass::EnumState eState);

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
    BaseClass::EnumState m_iState;
signals:
    void dataChanged( QString value  ,int iType = 0 );

};

#endif //CT_H
