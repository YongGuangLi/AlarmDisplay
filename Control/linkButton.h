#ifndef LINKBUTTON_H
#define LINKBUTTON_H
#include <QPushButton>
#include <QtGui/QWidget>
#include <QtGui>
#include <QPainter>
#include <QtDesigner/QDesignerExportWidget>

class QDESIGNER_WIDGET_EXPORT linkButton: public QPushButton   
{
    Q_OBJECT
    Q_PROPERTY(QString LinkPageName READ GetPageName WRITE SetPageName)
    Q_PROPERTY(QString ClassNm READ GetClassNm WRITE SetClassNm)
    Q_PROPERTY(QString Id READ GetItemId WRITE SetItemId)
public:
    linkButton(QWidget *parent = 0);
public:
    QString GetItemId() const;
    void SetItemId(QString ilineWidth);

    QString GetPageName()const;
    void SetPageName(QString strPageName);

    QString GetClassNm()const;
    void SetClassNm(QString strClassNm);
protected:
    void mouseReleaseEvent(QMouseEvent *event);
private:
    QString m_LinkPageName;
    QString m_strClassNm;
    QString m_ItemId;
signals:
    void dataChanged( QString value  ,int iType = 1 );

};

#endif //LINKBUTTON_H
