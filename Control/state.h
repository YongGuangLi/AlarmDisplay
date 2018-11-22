#ifndef STATE_H
#define STATE_H
#include "BaseClass.h"

class QDESIGNER_WIDGET_EXPORT state :  public BaseClass 
{
    Q_OBJECT
    Q_PROPERTY(QString ClassNm READ GetClassNm WRITE SetClassNm)
    Q_PROPERTY(BaseClass::EnumState State READ GetState WRITE SetState)
public:
    state(QWidget *parent = 0);

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

#endif //STATE_H
