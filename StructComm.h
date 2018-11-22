#ifndef __STRUCTCOMM_H_INCLUDED__
#define __STRUCTCOMM_H_INCLUDED__

#include <list>
#include <string>
#include <QScrollArea>
#include <QStackedWidget>

using namespace std;

typedef struct _WidgetAndScrollArea
{
    QScrollArea* pScrollArea;
    QWidget    * pWidget;
}WidgetAndScrollArea;

typedef struct _StructToControl
{
    string strControlMsgType;
    string strControlIdField;
    string strControlValue;
    string strControlClassName;
    string strUiControlProportValue;
    string strUiControlProportValuetype;

}StructToControl;

typedef struct _ControlMessage
{
    string strId;
    string strVal;
    int    iVal;
    float  fVal;
    double dVal;
    string strPropertyNm;
    string strPropertyType;
    _ControlMessage()
    {
        strId ="";
        strVal ="";
        iVal = -1;
        fVal = -1;
        dVal = -1;
        strPropertyNm ="";
        strPropertyType = "";
    }
}ControlMessage;

typedef struct _SubscribeKey
{
    list<string>strKey;
    _SubscribeKey()
    {
        strKey.clear();
    }
}SubscribeKey;

typedef struct _UIObject{
    QObject * object;
    QObject * parentObject;
    QString strObjectId;
    QString strClassNm;
    QString strValue;
}UIObject;

#endif 
