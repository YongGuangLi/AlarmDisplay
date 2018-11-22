#ifndef TOKEN_H
#define TOKEN_H
#include "BaseClass.h"

class QDESIGNER_WIDGET_EXPORT token :  public BaseClass 
{
    Q_OBJECT
    Q_PROPERTY(QString ShowInfo READ GetShowInfo WRITE SetShowInfo)
    Q_PROPERTY(QString ClassNm READ GetClassNm WRITE SetClassNm)
public:
    token(QWidget *parent = 0);

    QString GetShowInfo()const;
    void SetShowInfo(QString strShowInfo);
    
    QString GetClassNm()const;
    void SetClassNm(QString strClassNm);

protected:
    void paintEvent(QPaintEvent *event);
    void mouseReleaseEvent( QMouseEvent * event );
signals:
    void dataChanged( QString value ,int iType = 0 );
private:
    QString m_ShowInfo;
    QFont   m_Font;
    QString m_strClassNm;
 };

#endif //TOKEN_H
