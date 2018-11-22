#ifndef BUS_H
#define BUS_H
#include "BaseClass.h"

class QDESIGNER_WIDGET_EXPORT bus :  public BaseClass
{
    Q_OBJECT
    Q_ENUMS(LineType)
    Q_PROPERTY(LineType lineType READ GetLineType WRITE SetLineType)
    Q_PROPERTY(QString ClassNm READ GetClassNm WRITE SetClassNm)
public:
    bus(QWidget *parent = 0);
    enum LineType{Normal = 0,StartArrow,EndArrow};
public:
    LineType GetLineType() const;
    void SetLineType(LineType lineType);
    
    QString GetClassNm()const;
    void SetClassNm(QString strClassNm);
protected:
    void paintEvent(QPaintEvent *event);
     void mouseReleaseEvent( QMouseEvent * event );
     void mousePressEvent(QMouseEvent * event);
private:
    LineType m_LineType;
    QString m_strClassNm;
signals:
    void dataChanged( QString value ,int iType = 0 );
};

#endif //BUS_H
