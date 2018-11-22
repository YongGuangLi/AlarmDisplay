#ifndef GROUNDDISCONNECTOR_H
#define GROUNDDISCONNECTOR_H
#include "BaseClass.h"
#include "line.h"

class QDESIGNER_WIDGET_EXPORT groundDisconnector :public line
{
    Q_OBJECT

    Q_PROPERTY(BaseClass::EnumState State READ GetGroundDisconnectorState WRITE SetGroundDisconnectorState)
  //  Q_ENUMS(EnumState)
    Q_PROPERTY(QString ClassNm READ GetClassNm WRITE SetClassNm)
public:
    groundDisconnector(QWidget *parent = 0);
    //enum EnumState {On = 0, Off};
public:
    BaseClass::EnumState GetGroundDisconnectorState() const;
    void SetGroundDisconnectorState(BaseClass::EnumState eState);
    
    QString GetClassNm()const;
    void SetClassNm(QString strClassNm);
protected:
    void paintEvent(QPaintEvent *event);
    void mouseReleaseEvent( QMouseEvent * event );
private:
    BaseClass::EnumState m_iState;
    QString m_strClassNm;
signals:
    void dataChanged( QString value  ,int iType = 0 );
};

#endif //GROUNDDISCONNECTOR_H
