#ifndef ASCOIL_H
#define ASCOIL_H
#include "BaseClass.h"

class QDESIGNER_WIDGET_EXPORT ascoil: public BaseClass
{
    Q_OBJECT
    Q_PROPERTY(QString ClassNm READ GetClassNm WRITE SetClassNm)
public:
    ascoil(QWidget *parent = 0);
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
    void dataChanged( QString value  ,int iType = 0 );

};

#endif //ASCOIL_H
