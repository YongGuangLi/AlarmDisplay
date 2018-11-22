#ifndef DISCONNECTOR_H
#define DISCONNECTOR_H
#include "BaseClass.h"

class QDESIGNER_WIDGET_EXPORT disconnector :  public BaseClass 
{
    Q_OBJECT
    Q_PROPERTY(BaseClass::EnumState State READ GetBreakerState WRITE SetBreakerState)
    Q_PROPERTY(QString ClassNm READ GetClassNm WRITE SetClassNm)
public:
    disconnector(QWidget *parent = 0);
    
public:
    BaseClass::EnumState GetBreakerState() const;
    void SetBreakerState(BaseClass::EnumState eState);

    QString GetClassNm()const;
    void SetClassNm(QString strClassNm);
private:
      QString m_strClassNm;
      BaseClass::EnumState m_iState;
protected:
    void paintEvent(QPaintEvent *event);
    void mouseReleaseEvent( QMouseEvent * event );
signals:
    void dataChanged( QString value  ,int iType = 0 );
 };

#endif //DISCONNECTOR_H
