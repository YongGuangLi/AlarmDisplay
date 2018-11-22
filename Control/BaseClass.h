#ifndef BASECLASS_H
#define BASECLASS_H
#include <QtGui/QWidget>
#include <QtGui>
#include <QPainter>
#include <QtDesigner/QDesignerExportWidget>

class BaseClass : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(RotateAngle Angle READ GetAngle WRITE SetAngle)
    Q_ENUMS(RotateAngle)
    Q_PROPERTY(PenColor LineColor READ GetLineColor WRITE SetLineColor)
    Q_ENUMS(PenColor)
    Q_PROPERTY(int LineWidth READ GetLineWidth WRITE SetLineWidth)
    Q_PROPERTY(QString Id READ GetItemId WRITE SetItemId)
    Q_ENUMS(TelecontrolSignal)
    Q_PROPERTY(TelecontrolSignal Telecontrol READ GetItemTelecontrol WRITE SetItemTelecontrol)
    Q_ENUMS(EnumState)
public:
    BaseClass(QWidget *parent = 0);
public:
    //公用的枚举
     enum RotateAngle{Angle0=0,Angle90,Angle180,Angle270};
     enum PenColor {black,red,green,blue,white};
     enum TelecontrolSignal {ON=0,OFF};
     enum EnumState {OFF_STATE=0,ON_STATE,MIDDLE_STATE,BAD_STATE};
public:
    //公共属性
    RotateAngle GetAngle() const;
    void SetAngle(RotateAngle rotateAngle);

    PenColor GetLineColor() const;
    void SetLineColor(PenColor lineColor);

    int GetLineWidth() const;
    void SetLineWidth(int ilineWidth);

    QString GetItemId() const;
    void SetItemId(QString ilineWidth);

    TelecontrolSignal GetItemTelecontrol() const;
    void SetItemTelecontrol(TelecontrolSignal signalValue);
public:
    //公共函数
    void SetRotate(QPainter &painter,int iWidth,int iHeight);
    void SetPenColor(QPen &pen);

    void paintMiddleState(QPainter &painter,int iWidth,int iHeight,QPen &pen);

public:
    //公共属性变量
    RotateAngle m_Angle;
    PenColor m_PenColor;  
    int m_PenWidth;
    QString m_ItemId;
    TelecontrolSignal m_signalValue;

};

#endif //BASECLASS_H
