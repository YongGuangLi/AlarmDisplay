#ifndef POWERFLOWTOKEN_H
#define POWERFLOWTOKEN_H
#include "BaseClass.h"

class QDESIGNER_WIDGET_EXPORT powerFlow :  public BaseClass 
{
    Q_OBJECT
    Q_PROPERTY(QString ClassNm READ GetClassNm WRITE SetClassNm)
public:
    powerFlow(QWidget *parent = 0);
    
    QString GetClassNm()const;
    void SetClassNm(QString strClassNm);
protected:
    void paintEvent(QPaintEvent *event);
    void mouseReleaseEvent( QMouseEvent * event );
signals:
    void dataChanged( QString value ,int iType = 0 );
private:
	    QString m_strClassNm;;
 };

#endif //POWERFLOWTOKEN_H
