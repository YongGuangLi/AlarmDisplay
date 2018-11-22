#ifndef TRANSFORMER3_H
#define TRANSFORMER3_H

#include "BaseClass.h"


class QDESIGNER_WIDGET_EXPORT transformer3 : public BaseClass
{
    Q_OBJECT
    Q_PROPERTY(int App1 READ GetApp1 WRITE SetApp1)
    Q_PROPERTY(double KeyId1 READ GetKeyId1 WRITE SetKeyId1)
    Q_PROPERTY(double Voltype1 READ GetVoltype1 WRITE SetVoltype1)
    Q_PROPERTY(int App2 READ GetApp2 WRITE SetApp2)
    Q_PROPERTY(double KeyId2 READ GetKeyId2 WRITE SetKeyId2)
    Q_PROPERTY(double Voltype2 READ GetVoltype2 WRITE SetVoltype2)
    Q_PROPERTY(int App3 READ GetApp3 WRITE SetApp3)
    Q_PROPERTY(double KeyId3 READ GetKeyId3 WRITE SetKeyId3)
    Q_PROPERTY(double Voltype3 READ GetVoltype3 WRITE SetVoltype3)
    Q_PROPERTY(QString ClassNm READ GetClassNm WRITE SetClassNm)
public:
    transformer3(QWidget *parent = 0);
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

    int GetApp3()const;
    void SetApp3(int iApp3);

    double GetKeyId3()const;
    void SetKeyId3(double iKeyId3);

    double GetVoltype3()const;
    void SetVoltype3(double iVoltype3);
    
     QString GetClassNm()const;
    void SetClassNm(QString strClassNm);
protected:
    void paintEvent(QPaintEvent *event);       //--鼠标移动事件  
    void mouseReleaseEvent( QMouseEvent * event );
private:
    int m_App1;
    double m_KeyId1;
    double m_Voltype1;
    int m_App2;
    double m_KeyId2;
    double m_Voltype2;
    int m_App3;
    double m_KeyId3;
    double m_Voltype3;
    
    QString m_strClassNm;

    QPoint m_BeginPoint;
    QPoint m_EndPoint;
    QPoint m_MouseReleasePoint;
signals:
    void dataChanged( QString value ,int iType = 0 );
};

#endif //TRANSFORMER2_H
