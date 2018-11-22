#ifndef TRANSFORMER2_H
#define TRANSFORMER2_H
#include "BaseClass.h"

class QDESIGNER_WIDGET_EXPORT transformer2 : public BaseClass
{
    Q_OBJECT
    Q_PROPERTY(int App1 READ GetApp1 WRITE SetApp1)
    Q_PROPERTY(double KeyId1 READ GetKeyId1 WRITE SetKeyId1)
    Q_PROPERTY(double Voltype1 READ GetVoltype1 WRITE SetVoltype1)
    Q_PROPERTY(int App2 READ GetApp2 WRITE SetApp2)
    Q_PROPERTY(double KeyId2 READ GetKeyId2 WRITE SetKeyId2)
    Q_PROPERTY(double Voltype2 READ GetVoltype2 WRITE SetVoltype2)
    Q_PROPERTY(QString ClassNm READ GetClassNm WRITE SetClassNm)
public:
    transformer2(QWidget *parent = 0);
public:
    int GetApp1()const;
    void SetApp1(int iApp1);

    double GetKeyId1()const;
    void SetKeyId1(double iKeyId1);
    
    double GetVoltype1()const;
    void SetVoltype1(double iVoltype1);

    int GetApp2()const;
    void SetApp2(int iApp2);

    double GetKeyId2()const;
    void SetKeyId2(double iKeyId2);

    double GetVoltype2()const;
    void SetVoltype2(double iVoltype2);
    
    QString GetClassNm()const;
    void SetClassNm(QString strClassNm);

protected:
    void paintEvent(QPaintEvent *event);

    void mouseReleaseEvent( QMouseEvent * e );
    //void mousePressEvent(QMouseEvent *e);		//--鼠标按下事件
    //void mouseMoveEvent(QMouseEvent *e);	    //--鼠标移动事件
    //void mouseDoubleClickEvent(QMouseEvent *e);	//--鼠标双击事件

private:
    int m_App1;
    double m_KeyId1;
    long m_Voltype1;
    int m_App2;
    long m_KeyId2;
    long m_Voltype2;
    QPoint m_BeginPoint;
    QPoint m_EndPoint;
    QPoint m_MouseReleasePoint;
    QString m_strClassNm;

signals:
    void dataChanged( QString value ,int iType = 0 );
};

#endif //TRANSFORMER2_H
