#ifndef ROUNDRECT_H
#define ROUNDRECT_H
#include "BaseClass.h"

class QDESIGNER_WIDGET_EXPORT roundRect :  public BaseClass
{
    Q_OBJECT
    Q_PROPERTY(int x READ GetX WRITE SetX)
    Q_PROPERTY(int y READ GetY WRITE SetY)
    Q_PROPERTY(int h READ GetH WRITE SetH)
    Q_PROPERTY(int Rx READ GetRx WRITE SetRx)
    Q_PROPERTY(int Ry READ GetRy WRITE SetRy)
    Q_PROPERTY(QString ClassNm READ GetClassNm WRITE SetClassNm)
public:
    roundRect(QWidget *parent = 0);
public:
    int GetX() const;
    void SetX(int iX);

    int GetY() const;
    void SetY(int iY);

    int GetH() const;
    void SetH(int iH);

    int GetRx() const;
    void SetRx(bool iRx);

    int GetRy() const;
    void SetRy(bool iRy);
    QString GetClassNm()const;
    void SetClassNm(QString strClassNm);

protected:
    void paintEvent(QPaintEvent *event);
    void mouseReleaseEvent( QMouseEvent * event );
public:
    int m_x;
    int m_y;
    int m_h;
    int m_Rx;
    int m_Ry;
signals:
    void dataChanged( QString value ,int iType = 0 );
private:
	    QString m_strClassNm;
};

#endif //ROUNDRECT_H
