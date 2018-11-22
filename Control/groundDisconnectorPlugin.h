#ifndef GROUNDDISCONNECTORPLUGIN_H
#define GROUNDDISCONNECTORPLUGIN_H

#include <QDesignerCustomWidgetInterface>

class groundDisconnectorPlugin : public QObject, public QDesignerCustomWidgetInterface
{
    Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)


public:
    groundDisconnectorPlugin(QObject *parent = 0);

    bool isContainer() const;
    bool isInitialized() const;
    QIcon icon() const;
    QString domXml() const;
    QString group() const;
    QString includeFile() const;
    QString name() const;
    QString toolTip() const;
    QString whatsThis() const;
    QWidget *createWidget(QWidget *parent);
    void initialize(QDesignerFormEditorInterface *core);

private:
    bool m_initialized;
};

#endif //GROUNDDISCONNECTORPLUGIN_H
