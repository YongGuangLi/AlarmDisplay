#include "analysisui.h"
#include <QScrollArea>

AnalysisUi::AnalysisUi(QWidget *parent) :
        QWidget(parent)
{
    m_UiAllObject.clear();
}

AnalysisUi::~AnalysisUi()
{
    foreach(UIObject item ,m_UiAllObject)
    {
        if (item.object != NULL)
        {
            delete item.object;
            item.object = NULL;
        }
    }
    m_UiAllObject.clear();

    foreach(WidgetAndScrollArea item ,m_pQscrollAreaList)
    {
        if (item.pWidget != NULL)
        {
            delete item.pWidget;
            item.pWidget = NULL;
        }

        if (item.pScrollArea != NULL)
        {
            delete item.pScrollArea;
            item.pScrollArea = NULL;
        }
    }

    m_pQscrollAreaList.clear();

    if (pStackedWidget != NULL)
    {
        delete pStackedWidget;
        pStackedWidget = NULL;
    }
    
}

QString AnalysisUi::ReplascEitherEndQuotationMarks(QString strSrc)
{
    QString strtemp ="";
    if (strSrc.length() <= 0)
    {
        return strtemp;
    }
    if (strSrc.mid(0,1).compare("\"") == 0)
    {
        strtemp = strSrc.mid(1,strSrc.length());
    }

    if (strSrc.lastIndexOf("\"") == 0)
    {
        strtemp = strtemp.mid(0,strtemp.length() -1);
    }
    return strtemp;
}


void AnalysisUi::GetFolderAllFile()
{
    QStringList listFileName;
    listFileName.clear();
    if (m_FilePaht.length() <= 0)
    {
       return ;
    }
    QDir dir(m_FilePaht);
    listFileName << "*.ui";
    listFileName = dir.entryList(listFileName,QDir::Files |QDir::Readable,QDir::Name);
    foreach(QString str, listFileName)
    {
        m_UiFileAssociatUiId.insert(str,0);
    }
}

QStackedWidget*  AnalysisUi::LoadUiFile(QWidget *pWidget)
{
    if (m_UiFileAssociatUiId.count() <= 0)
    {
        return NULL;
    }

    QUiLoader loader;
    loader.addPluginPath("Debuglib");
    pStackedWidget = new QStackedWidget(pWidget);
    QMap<QString,int>::iterator it = m_UiFileAssociatUiId.begin();
    int iStackedWidgetIndex = 0;
    for (; it != m_UiFileAssociatUiId.end() ; it++)
    {
        QString strFileName = m_FilePaht + it.key();
        QFile file(strFileName);
        file.open(QFile::ReadOnly);
        QWidget *myWidget = loader.load(&file,pWidget);
        file.close();
        QScrollArea *pScrollarea = new QScrollArea(this);
        pScrollarea->setAlignment(Qt::AlignCenter);
        pScrollarea->setWidget(myWidget);
        pStackedWidget->addWidget(pScrollarea);
        m_UiFileAssociatUiId[it.key()] = iStackedWidgetIndex;
        iStackedWidgetIndex++;
        WidgetAndScrollArea info;
        info.pWidget = myWidget;
        info.pScrollArea = pScrollarea;
        m_pQscrollAreaList.append(info);
        GetUiAllObject(myWidget);
    }
    //为控件添加响应事件
    connectSignalSlot();
    return pStackedWidget;
}

void AnalysisUi::CallBackGetObject(QObjectList pobject)
{
    foreach(QObject *obj , pobject)
    {
        if (obj->objectName().isEmpty() == true)
        {
            continue;
        }
        
        if (QString(obj->metaObject()->className()).isEmpty() == true)
        {
            continue;
        }
        //if(obj->property("Id").isValid () )
        //{
        //    if (obj->property("Id").toString().isEmpty() == true)
        //    {
        //        continue;
        //    }
        //}

        UIObject objectInfo;
        objectInfo.object = obj;
        //qDebug() << obj->metaObject()->className();
        //qDebug() << obj->property("ClassNm").toString();
        objectInfo.strClassNm = obj->metaObject()->className();
        objectInfo.strObjectId = obj->property("Id").toString();
        objectInfo.parentObject = obj->parent();
        //设置所有控件状态都为“0”     
        QVariant variant(1);
        obj->setProperty("State",variant);
        objectInfo.strValue = "0";

        if (objectInfo.strObjectId.length() > 0)
        {
            //qDebug() << objectInfo.strObjectId;
            m_UiAllObject.append(objectInfo);
        }
        if (obj->children().size() > 0)
        {
            CallBackGetObject(obj->children());
        }
     }
}

bool AnalysisUi::IdAndValueIsExist(QString strId,QString strValue)
{
    bool bRet = false;
    if (m_UiAllObject.begin() == m_UiAllObject.end())
    {
        return bRet;
    }
    QList<UIObject>::iterator it;
    for (it = m_UiAllObject.begin(); it != m_UiAllObject.end(); it++ )
    {
        if (it->strObjectId.compare(strId) == 0)
        {
            if (it->strValue.compare(strValue) == 0)
            {
                bRet = true;
            }
            else
            {
                return false;
            }
        }
    }
    return true;
}
void AnalysisUi::SetStructToControl(QList<StructToControl> StructToControl)
{
    m_StructToControl = StructToControl;
}

bool AnalysisUi::GetSetPorpertyField(string strControlClassNm,string &strPropertyField,string &strPropertyType)
{
    bool bRet = false;
    foreach (StructToControl item, m_StructToControl)
    {
        if (item.strControlClassName.compare(strControlClassNm) == 0)
        {
            strPropertyField = item.strUiControlProportValue;
            strPropertyType = item.strUiControlProportValuetype;
            return true;
        }
    }
    return false;
}

void AnalysisUi::SetControlVal(string strControlId, string strControlVal)
{

    qDebug()<<strControlId.c_str()<<strControlVal.c_str();
   QList<UIObject>::iterator it;
   for(it = m_UiAllObject.begin(); it != m_UiAllObject.end(); it++ )
   {
       if (it->strObjectId.toStdString().compare(strControlId) == 0)
       {
           string strObjName = it->strClassNm.toStdString();
           string strPropertyField ="";
           string strPropertyType ="";
           bool bRet = GetSetPorpertyField(strObjName,strPropertyField,strPropertyType);
           if (bRet == true)
           {
               if (strPropertyType.compare("string") == 0)
               {
                   it->strValue = QString::fromStdString(strControlVal);
                   QVariant variant(strControlVal.c_str());
                    it->object->setProperty(strPropertyField.c_str(),variant);
               }
               else if (strPropertyType.compare("int") == 0)
               {
                   it->strValue = QString::fromStdString(strControlVal);
                   int iTemp = atoi(strControlVal.c_str());
                   QVariant variant(iTemp);
                   it->object->setProperty(strPropertyField.c_str(),variant);
               }
               else if (strPropertyType.compare("double") == 0 || strPropertyType.compare("float") == 0)
               {
                   it->strValue = QString::fromStdString(strControlVal);
                   int iTemp = atof(strControlVal.c_str());
                   QVariant variant(iTemp);
                   it->object->setProperty(strPropertyField.c_str(),variant);
               }
           }
           else
           {
               bool ok = false;
               int iTemp = QString::fromStdString(strControlVal).toInt(&ok, 10);
               if(ok)
               {
                   it->strValue = QString::fromStdString(strControlVal);
                   QVariant variant(iTemp);
                   it->object->setProperty("State",variant);
               }
               else
               {
                   qDebug()<<"strControlVal:"<<strControlVal.c_str();
               }

           }
       }
   }
}



void AnalysisUi::connectSignalSlot()
{
   foreach(UIObject item ,m_UiAllObject)
   {
        connect(item.object,SIGNAL(dataChanged(QString,int)),this, SLOT(SetValueToUi(QString,int)) );
   }
}

void AnalysisUi::SetValueToUi(QString info,int iType)
{
    qDebug() << info;

    if (iType == 1)
    {
        if (m_UiFileAssociatUiId.size() <= 0)
        {
            return ;
        }
        int iIndex = m_UiFileAssociatUiId[info];
        pStackedWidget->setCurrentIndex(iIndex);
    }
    
    //添加控制消息
    //pOptionDlg->setWindowFlags(Qt::Window);
    //pOptionDlg->setWindowTitle(QString("控件操作界面"));
    //pOptionDlg->m_ControlId =info;
    //pOptionDlg->SetUiControlVal();
    //pOptionDlg-> exec();
}

 void AnalysisUi::GetUiAllObject(QWidget *uiWidget)
{
    if (uiWidget == NULL || uiWidget->children().size() <= 0)
    {
        return ;
    }
    CallBackGetObject(uiWidget->children());
}

 void AnalysisUi::SetUiFilePath(QString strFilePath)
 {
     if (strFilePath.isEmpty() == true)
     {
         m_FilePaht = "";
     }
     else
     {
         m_FilePaht = strFilePath;
     }
 }

 QMap<QString, int> AnalysisUi::GetUIFileName(void)
 {
     return m_UiFileAssociatUiId;
 }
