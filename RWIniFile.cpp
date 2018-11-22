#include "RWIniFile.h"
#include <QSettings>

RWIniFile::RWIniFile()
{

}

RWIniFile::~RWIniFile()
{

}


bool RWIniFile::writeIni(QString path, QString section,QString keyword, QString keyvalue)  
{  
    if(path == QString(""))  
    {  
        return false;  
    }  
    else  
    {  
        //创建配置文件操作对象  
        QSettings *config = new QSettings(path, QSettings::IniFormat);  

        //将信息写入配置文件  
        config->beginGroup(section);  
        config->setValue(keyword, keyvalue);  
        config->endGroup();  

        if(config)  
        {  
            delete config;  
        }  

        return true;  
    }  
}  

bool RWIniFile::writeIni(QString path, QString section,QString keyword, QString keyvalue, bool clear_flag)  
{  
    if(path == QString(""))  
    {  
        return false;  
    }  
    else  
    {  
        //创建配置文件操作对象  
        QSettings *config = new QSettings(path, QSettings::IniFormat);  
        if( clear_flag == true )  
            config->clear();  

        //将信息写入配置文件  
        config->beginGroup(section);  
        config->setValue(keyword, keyvalue);  
        config->endGroup();  

        if(config)  
        {  
            delete config;  
        }  

        return true;  
    }  
}  

QString RWIniFile::readIni(QString path, QString section,QString keyword, QString &keyvalue)  
{  
    if(path == QString(""))  
    {  
        return false;  
    }  
    else  
    {  
        //创建配置文件操作对象  
        QSettings *config = new QSettings(path, QSettings::IniFormat);  

        keyvalue = config->value(section + "/" + keyword).toString();  

        if(config)  
        {  
            delete config;  
        }  

        return keyvalue;  
    }  
} 
