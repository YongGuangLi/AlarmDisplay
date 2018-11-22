#ifndef __INCLUDEFILE_H_INCLUDED__
#define __INCLUDEFILE_H_INCLUDED__

//#define  MESSAGEQUEUE

#include "RWIniFile.h"
#include <QWidget>
#include <QMap>
#include <list>
#include "StructComm.h"
#include <QThread>
#include <QScrollArea>
#include <QtUiTools/QtUiTools>


//**********图元头文件添加*****************/
#include"arrester.h"
#include"ascoil.h"
#include"BaseClass.h"
#include"breaker.h"
#include"bus.h"
#include"capacitor.h"
#include"cT.h"
#include"disconnector.h"
#include"fuse.h"
#include"generator.h"
#include"groundDisconnector.h"
#include"image.h"
#include"line.h"
#include"linkButton.h"
#include"LogWriter.h"
#include"powerFlow.h"
#include"protect.h"
#include"pT.h"
#include"reactor.h"
#include"reactorS.h"
#include"renderarea.h"
#include"roundRect.h"
#include"state.h"
#include"status.h"
#include"terminal.h"
#include"textEdit.h"
#include"token.h"
#include"transformer2.h"
#include"transformer3.h"
#include"dTextEdit.h"
//**********图元头文件添加*****************/


#ifdef MESSAGEQUEUE
#include "MessageQueueDataSource.h"
#include "MessageInvocation.h"
#include "StructBase.h"
#else
//#include "WgjHiRedis.h"
//#include "RtdbMessage.pb.h"
#endif
using namespace std;


#endif

