#ifndef TRANSFORMER2KETIAOPLUGIN_H
#define TRANSFORMER2KETIAOPLUGIN_H

#include <QDesignerCustomWidgetInterface>

class transformer2KeTiaoPlugin : public QObject, public QDesignerCustomWidgetInterface
{
    Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)


public:
    transformer2KeTiaoPlugin(QObject *parent = 0);

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

#endif //TRANSFORMER2KETIAOPLUGIN_H
