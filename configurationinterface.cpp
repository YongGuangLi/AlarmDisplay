#include "configurationinterface.h"
#include "ui_configurationinterface.h"

ConfigurationInterface::ConfigurationInterface(QWidget *parent) :
    QStackedWidget(parent),
    ui(new Ui::ConfigurationInterface)
{
    ui->setupUi(this);

    pBase = new Base(this);
    //给combox中添加UI文件名称
    QMap<QString, int> objFileName = pBase->GetFolderAllUiFile();
    comboBox = new QComboBox(this);
    comboBox->setGeometry(0, 0, 120, 30);
    comboBox->clear();
    QMap<QString, int>::iterator it = objFileName.begin();
    for (; it != objFileName.end(); ++it )
    {
      QString strUiFileName = it.key();
      strUiFileName.chop(3);
      comboBox->addItem(strUiFileName);
    }
    connect(comboBox,SIGNAL(currentIndexChanged(QString)),this, SLOT(ChangeUiFile(QString)));
    //添加所有的UI文件。
    QWidget *widget = pBase->LoadUiFile(this);
    if(widget != NULL)
    {
        ui->gridLayout_2->addWidget(widget);
    }
}


ConfigurationInterface::~ConfigurationInterface()
{
    pBase->deleteLater();
    delete ui;
}


void ConfigurationInterface::receiveControlData(QString strControlId, QString strControlVal)
{
   // if(!pBase->m_AnalysisUi->IdAndValueIsExist(strControlId, strControlVal))
        pBase->SetControlVal(strControlId.toStdString(), strControlVal.toStdString());
}


void ConfigurationInterface::ChangeUiFile(QString)
{
    QString strName = comboBox->currentText();
    strName.append(".ui");
    if (pBase->m_AnalysisUi->m_UiFileAssociatUiId.size() <= 0)
    {
        return ;
    }
    int iIndex = pBase->m_AnalysisUi->m_UiFileAssociatUiId[strName];
    pBase->m_AnalysisUi->pStackedWidget->setCurrentIndex(iIndex);
}






//    QDir dir(QApplication::applicationDirPath() + "/UiConfiguration");
//    QStringList listFiles = dir.entryList(QDir::Files);

//    QUiLoader loader;
//    for(int i = 0; i < listFiles.size();  ++i)
//    {
//        QFile file(QApplication::applicationDirPath() + "/UiConfiguration/" + listFiles.at(i));
//        file.open(QFile::ReadOnly);
//        file.close();
//        this->insertWidget(i, loader.load(&file, this));
//    }
//    this->setCurrentIndex(0);
