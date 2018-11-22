#ifndef LINE_H
#define LINE_H
#include "BaseClass.h"

class QDESIGNER_WIDGET_EXPORT line :  public BaseClass
{
    Q_OBJECT
    Q_PROPERTY(int x1 READ GetX1 WRITE SetX1)
    Q_PROPERTY(int x2 READ GetX2 WRITE SetX2)
    Q_PROPERTY(int y1 READ GetY1 WRITE SetY1)
    Q_PROPERTY(int y2 READ GetY2 WRITE SetY2)
    Q_PROPERTY(bool StartArrawType READ GetStartArrawType WRITE SetStartArrawType)
    Q_PROPERTY(bool EndArrowType READ GetEndArrowType WRITE SetEndArrowType)
    Q_PROPERTY(int StartArrowSize READ GetStartArrowSize WRITE SetStartArrowSize)
    Q_PROPERTY(int EndArrowSize READ GetEndArrowSize WRITE SetEndArrowSize)
    Q_PROPERTY(QString ClassNm READ GetClassNm WRITE SetClassNm)
public:
    line(QWidget *parent = 0);
public:
    int GetX1() const;
    void SetX1(int iX1);

    int GetX2() const;
    void SetX2(int iX2);

    int GetY1() const;
    void SetY1(int iY1);

    int GetY2() const;
    void SetY2(int iY2);

    bool GetStartArrawType() const;
    void SetStartArrawType(bool bStartArrawType);

    bool GetEndArrowType() const;
    void SetEndArrowType(bool bEndArrowType);

    int GetStartArrowSize() const;
    void SetStartArrowSize(int iStartArrowSize);

    int GetEndArrowSize() const;
    void SetEndArrowSize(int iEndArrowSize);
    
    QString GetClassNm()const;
    void SetClassNm(QString strClassNm);

protected:
    void paintEvent(QPaintEvent *event);
    void mouseReleaseEvent( QMouseEvent * event );

public:
    int m_x1;
    int m_x2;
    int m_y1;
    int m_y2;
    bool m_StartArrawType;
    bool m_EndArrowType;
    int m_StartArrowSize;
    int m_EndArrowSize;
    QString m_strClassNm;
signals:
    void dataChanged( QString value  ,int iType = 0 );
};

#endif //LINE_H
