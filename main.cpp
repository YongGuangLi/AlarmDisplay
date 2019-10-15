#include "widget.h"
#include <QApplication>
#include <QGtkStyle>
#include "singleapplication.h"

#include "log4qt/propertyconfigurator.h"
#include "log4qt/logmanager.h"

#include "stylehelper.h"

int main(int argc, char *argv[])
{
    SingleApplication a(argc, argv);

    SingleApplication::addLibraryPath(SingleApplication::applicationDirPath() + "/plugins");     //Qt加载动态庫有2种方式 1.在执行路径下新建plugins/sqldrivers,代码设置庫路径，
                                                                                                 //                   2.在执行路径下新建sqldrivers,就不用代码设置庫路径
    Log4Qt::PropertyConfigurator::configure(SingleApplication::applicationDirPath() + "/log4qt.properties");
    Log4Qt::LogManager::setHandleQtMessages(true);

    //StyleHelper::setStyle(":/qssstyle/qssstyle/whiteStyle.qss");
    StyleHelper::setStyle(":/qssstyle/qssstyle/homepage.qss");

    if(!a.isRunning())
    {
        Widget w;
        w.show();
        return a.exec();
    }
    return 0;
}
