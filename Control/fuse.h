#ifndef FUSE_H
#define FUSE_H
#include "BaseClass.h"

class QDESIGNER_WIDGET_EXPORT fuse :  public BaseClass 
{
    Q_OBJECT
    Q_PROPERTY(QString ClassNm READ GetClassNm WRITE SetClassNm)
public:
    fuse(QWidget *parent = 0);
    
    QString GetClassNm()const;
    void SetClassNm(QString strClassNm);
protected:
    void paintEvent(QPaintEvent *event);
    void mouseReleaseEvent( QMouseEvent * event );
signals:
    void dataChanged( QString value ,int iType = 0 );
private:
    QString m_strClassNm;
 };

#endif //FUSE_H
