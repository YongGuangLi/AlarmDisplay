#ifndef ANALYSISUI_H
#define ANALYSISUI_H

#include "IncludeFile.h"

class AnalysisUi  : public QWidget
{
    Q_OBJECT
public:
    AnalysisUi(QWidget *parent = 0);
    ~AnalysisUi();

private:
    QString m_strCurrentControlId;
    int     m_iCurrentControlType;
public:
    void GetFolderAllFile();
    QStackedWidget* LoadUiFile(QWidget *pWidget);
    void GetUiAllObject(QWidget *uiWidget);
    void CallBackGetObject(QObjectList pobject);
    void SetUiFilePath(QString strFilePath);
    void connectSignalSlot();
    bool IdAndValueIsExist(QString strId,QString strValue);
public slots:
    void SetValueToUi(QString info,int iType);
    QString ReplascEitherEndQuotationMarks(QString strSrc);
    void SetControlVal(string strControlId, string strControlVal);
    void SetStructToControl(QList<StructToControl> StructToControl);
    bool GetSetPorpertyField(string strControlClassNm,string &strPropertyField,string &strPropertyType);
public:
    QList<UIObject>m_UiAllObject;
    QList<StructToControl> m_StructToControl;
    QMap<QString, int> m_UiFileAssociatUiId;
 
    QStackedWidget *pStackedWidget;
    
    QString m_FilePaht;
    QMap<QString, int> GetUIFileName(void);
    QList <WidgetAndScrollArea> m_pQscrollAreaList;
};

#endif // ANALYSISUI_H
