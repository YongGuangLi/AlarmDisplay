#ifndef RWINIFILE_H
#define RWINIFILE_H
#include <QString>
class RWIniFile
{
public:
    RWIniFile();
    ~RWIniFile();
public:
  
    bool static writeIni(QString path, QString section,QString keyword, QString keyvalue,bool clear_flag);  
    bool static writeIni(QString path, QString section,QString keyword, QString keyvalue);  
    QString static readIni(QString path, QString section,QString keyword, QString &keyvalue);  
private:
    
};

#endif // RWINIFILE_H
