#ifndef BEAKER_H
#define BEAKER_H
#include "BaseClass.h"

class QDESIGNER_WIDGET_EXPORT breaker :  public BaseClass   
{
    Q_OBJECT
    Q_PROPERTY(BaseClass::EnumState State READ GetBreakerState WRITE SetBreakerState)
    Q_PROPERTY(QString ClassNm READ GetClassNm WRITE SetClassNm)
    
public:
    breaker(QWidget *parent = 0);
    //enum EnumState {On = 0, Off};
public:
    BaseClass::EnumState GetBreakerState() const;
    void SetBreakerState(BaseClass::EnumState eState);
    
    QString GetClassNm()const;
    void SetClassNm(QString strClassNm);
protected:
    void paintEvent(QPaintEvent *event);
    void mouseReleaseEvent ( QMouseEvent * event );
public:
    BaseClass::EnumState m_iState;
    QString m_strClassNm;
signals:
        void dataChanged( QString value  ,int iType = 0 );

};

#endif //BUS_H
