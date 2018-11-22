#ifndef PIECHART_H
#define PIECHART_H
#include "BaseClass.h"

class QDESIGNER_WIDGET_EXPORT pieChart :  public BaseClass   
{
    Q_OBJECT
    Q_PROPERTY(BaseClass::EnumState State READ GetpieChartState WRITE SetpieChartState)
    Q_PROPERTY(QString ClassNm READ GetClassNm WRITE SetClassNm)
    Q_PROPERTY(QString GetPieValue READ GetPieValue WRITE setGetPieValue )
    
public:
    pieChart(QWidget *parent = 0);
    //enum EnumState {On = 0, Off};
public:
    BaseClass::EnumState GetpieChartState() const;
    void SetpieChartState(BaseClass::EnumState eState);
    
    QString GetClassNm()const;
    void SetClassNm(QString strClassNm);

    QString GetPieValue()const;
    void setGetPieValue(QString strPieValue);

    void gradientArc(QPainter *painter, int radius, int startAngle, int angleLength, int arcHeight, QRgb color);
    void DescribeInfo(QPainter *painter, int startx, int starty, QString strTile, QString strValue, QRgb color);
protected:
    void paintEvent(QPaintEvent *event);
    void mouseReleaseEvent ( QMouseEvent * event );
public:
    BaseClass::EnumState m_iState;
    QString m_strClassNm;
    QString m_strPieValue;
signals:
        void dataChanged( QString value  ,int iType = 0 );

};

#endif //BUS_H
