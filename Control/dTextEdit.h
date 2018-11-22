#ifndef DTEXTEDIT_H
#define DTEXTEDIT_H
#include <QTextEdit>
#include <QtGui/QWidget>
#include <QtGui>
#include <QPainter>
#include <QtDesigner/QDesignerExportWidget>

class QDESIGNER_WIDGET_EXPORT dTextEdit: public QTextEdit
{
    Q_OBJECT
    Q_PROPERTY(int DotLength READ GetDotLength WRITE SetDotLength)
    Q_PROPERTY(bool ShowSignFlag READ GetShowSignFlag WRITE SetShowSignFlag)
    Q_PROPERTY(double DataUpperLimit READ GetDataUpperLimit WRITE SetDataUpperLimit)
    Q_PROPERTY(double DataLowerLimit READ GetDataLowerLimit WRITE SetDataLowerLimit)
    Q_PROPERTY(bool IsAbs READ GetIsAbs WRITE SetIsAbs)
    Q_ENUMS(Abs)
    Q_PROPERTY(QString ClassNm READ GetClassNm WRITE SetClassNm)
    Q_PROPERTY(QString Id READ GetItemId WRITE SetItemId)
public:
    dTextEdit(QWidget *parent = 0);
public:
    QString GetItemId() const;
    void SetItemId(QString ilineWidth);

    int GetDotLength()const;
    void SetDotLength(int iDotLength);

    bool GetShowSignFlag()const;
    void SetShowSignFlag(bool ShowSignFlag);

    double GetDataUpperLimit()const;
    void SetDataUpperLimit(double dDataUpperLimit);

    double GetDataLowerLimit()const;
    void SetDataLowerLimit(double dDataLowerLimit);

    bool GetIsAbs()const;
    void SetIsAbs(bool iIsAbs);

    QString GetClassNm()const;
    void SetClassNm(QString strClassNm);
protected:
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
private:
    QString m_strClassNm;
    int m_iDotLength;
    bool m_bShowSignFlag;
    double m_dDataUpperLimit;
    double m_dDataLowerLimit;
    bool m_bIsAbs;
    QString m_ItemId;
signals:
    void dataChanged( QString value  ,int iType = 1 );

};

#endif //DTEXTEDIT_H
