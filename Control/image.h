#ifndef IMAGE_H
#define IMAGE_H
#include "BaseClass.h"

class QDESIGNER_WIDGET_EXPORT image: public BaseClass    
{
    Q_OBJECT
    Q_PROPERTY(QString ahref READ Getahref WRITE Setahref)
    Q_PROPERTY(ImageShowType ShowStyle READ GetShowStyle WRITE SetShowStyle)
    Q_PROPERTY(QString ClassNm READ GetClassNm WRITE SetClassNm)
    Q_PROPERTY(QString Id READ GetItemId WRITE SetItemId)
public:
    image(QWidget *parent = 0);
public:
    enum ImageShowType{Middle,Zoom,Paved };
public:
    QString GetItemId() const;
    void SetItemId(QString ilineWidth);

    QString Getahref()const;
    void Setahref(QString strahref);

    ImageShowType GetShowStyle()const;
    void SetShowStyle(ImageShowType ShowStyle);

    QString GetClassNm()const;
    void SetClassNm(QString strClassNm);
protected:
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
private:
    QString m_ahref;
    ImageShowType m_strShowStyle;
    QString m_strClassNm;
    QString m_ItemId;
signals:
    void dataChanged( QString value ,int iType = 0 );

};

#endif //IMAGE_H
