#ifndef POWERITEM_H
#define POWERITEM_H

//#ifdef MAKEDLL_EXPORTS
//#define MAKEDLL_API __declspec(dllexport)
//#else
//#define MAKEDLL_API __declspec(dllimport)
//#endif


#include <QtDesigner>
#include <qplugin.h>

class PowerItem : public QObject, public QDesignerCustomWidgetCollectionInterface
{
    Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetCollectionInterface)
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QDesignerCustomWidgetCollectionInterface")
#endif // QT_VERSION >= 0x050000

public:
    explicit PowerItem(QObject *parent = 0);

    virtual QList<QDesignerCustomWidgetInterface*> customWidgets() const;

private:
    QList<QDesignerCustomWidgetInterface*> m_widgets;
};



#endif
