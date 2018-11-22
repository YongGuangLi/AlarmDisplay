#ifndef CAR1_H
#define CAR1_H
#include "BaseClass.h"

class QDESIGNER_WIDGET_EXPORT car1 :  public BaseClass 
{
    Q_OBJECT
    Q_PROPERTY(BaseClass::EnumState State READ GetState WRITE SetState)
    Q_PROPERTY(QString ClassNm READ GetClassNm WRITE SetClassNm)
public:
    car1(QWidget *parent = 0);
    BaseClass::EnumState GetState() const;
    void SetState(BaseClass::EnumState eState);

    QString GetClassNm()const;
    void SetClassNm(QString strClassNm);
    
protected:
    void paintEvent(QPaintEvent *event);
    void mouseReleaseEvent( QMouseEvent * event );
signals:
    void dataChanged( QString value ,int iType = 0 );
private:
	    QString m_strClassNm;
        BaseClass::EnumState m_iState;
 };

#endif //CAR1_H
