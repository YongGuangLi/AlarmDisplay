#ifndef TEXTEDIT_H
#define TEXTEDIT_H
#include <QTextEdit>
#include <QtGui/QWidget>
#include <QtGui>
#include <QPainter>
#include <QtDesigner/QDesignerExportWidget>

class QDESIGNER_WIDGET_EXPORT textEdit: public QTextEdit
{
    Q_OBJECT
    Q_PROPERTY(QString ClassNm READ GetClassNm WRITE SetClassNm)
    Q_PROPERTY(QString Id READ GetItemId WRITE SetItemId)
public:
    textEdit(QWidget *parent = 0);
public:
    QString GetItemId() const;
    void SetItemId(QString ilineWidth);

    QString GetClassNm()const;
    void SetClassNm(QString strClassNm);
protected:
    void mouseReleaseEvent(QMouseEvent *event);
private:
    int m_FontInterval;
    int m_RowInterVal;
    QString m_strClassNm;
    QString m_ItemId;
signals:
    void dataChanged( QString value ,int iType = 0 );

};

#endif //TEXTEDIT_H
