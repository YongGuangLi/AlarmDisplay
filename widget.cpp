#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    isRunning = true;
    this->setWindowIcon(QIcon(":/images/images/aboutlogo.png"));
    this->setWindowTitle(QString::fromLocal8Bit("变电站全景运行信息综合分析系统"));
    this->showMaximized();
    centerDisplay();                                     //窗口居中显示

    SingletonConfig->initConfigIni(QApplication::applicationDirPath() + "/AlarmDisplay.ini");
    SingletonConfig->initActionClassifier(QApplication::applicationDirPath() + "/ActionClassifier.xml");
    SingletonConfig->initIABaySignal(QApplication::applicationDirPath() + "/IABaySignal.xml");
    SingletonConfig->initIAFRule(QApplication::applicationDirPath() + "/IAFRule.xml");
    SingletonConfig->initIASRule(QApplication::applicationDirPath() + "/IASRule.xml");

    //redis订阅
    QtConcurrent::run(this, &Widget::redisSubscribe);

    if(SingletonDBHelper->open(SingletonConfig->getIpMySql(), SingletonConfig->getPortMySql(), SingletonConfig->getDbName(), SingletonConfig->getUserMysql() ,SingletonConfig->getPasswdMysql()))
    {
        qDebug()<<QString("Mysql Connect Success:%1").arg(SingletonConfig->getIpMySql());
        SingletonDBHelper->initUserinfoTable();
        SingletonDBHelper->readUserInfoFromDB();
        SingletonDBHelper->readUserGroupFromDB();
        SingletonDBHelper->readRightFromDB();
    }
    else
    {
        qDebug()<<QString("Mysql Connect Failure:%1 Error:%2").arg(SingletonConfig->getIpMySql()).arg(SingletonDBHelper->getError());
        return;
    }

    ui->label_station->setText(SingletonConfig->getStation());

    //初始化顶部导航栏按键
    initButtonConnections();

    //初始化告警页面切换按键
    initAlarmButtonsConnects();

    initAlarmTableWidget();

    ui->buttonGroup->setExclusive(true);

    ui->label_CurUser->installEventFilter(this);

    floatingWindow = new FloatingWindow(this);
    floatingWindow->show();

    SingletonDBHelper->readIaAlarmDataFromDB();
    SingletonDBHelper->readOaAlarmDataFromDB();
}

void Widget::initAlarmTableWidget()
{
    ui->tableWidget_SmartAlarm->verticalHeader()->hide();
    ui->tableWidget_SmartAlarm->horizontalHeader()->hide();
    ui->tableWidget_SmartAlarm->setEditTriggers(QAbstractItemView::NoEditTriggers);    //设置内容不可更改
    ui->tableWidget_SmartAlarm->horizontalHeader()->setResizeMode(0, QHeaderView::ResizeToContents);
    ui->tableWidget_SmartAlarm->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget_SmartAlarm->setSelectionBehavior(QAbstractItemView::SelectRows);   //单击选择一行
    ui->tableWidget_SmartAlarm->setShowGrid(true);

    ui->tableWidget_OriginalAlarm->verticalHeader()->hide();
    ui->tableWidget_OriginalAlarm->horizontalHeader()->hide();
    ui->tableWidget_OriginalAlarm->setEditTriggers(QAbstractItemView::NoEditTriggers);    //设置内容不可更改
    ui->tableWidget_OriginalAlarm->horizontalHeader()->setResizeMode(0, QHeaderView::ResizeToContents);
    ui->tableWidget_OriginalAlarm->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget_OriginalAlarm->setSelectionBehavior(QAbstractItemView::SelectRows);   //单击选择一行
    ui->tableWidget_OriginalAlarm->setShowGrid(true);


    //接收并显示告警数据
    connect(SingletonAlarmData, SIGNAL(receiverSmartAlarmData(int,stAlarmData)), this, SLOT(dispSmartAlarmData(int, stAlarmData)));
    connect(SingletonAlarmData, SIGNAL(receiverOriginalAlarmData(int,stAlarmData)), this, SLOT(dispOriginalAlarmData(int, stAlarmData)));
}

void Widget::initButtonConnections()
{
    realTimeAlarm = new RealTimeAlarm();

    loginWindow = new LoginWindow();
    connect(loginWindow, SIGNAL(userLogin(QString)), this, SLOT(setCurUser(QString)));

    historyQuery = new HistoryQuery();
    ui->gridLayout_HistoryQuery->addWidget(historyQuery);

    configurationInterface = new ConfigurationInterface();
    ui->gridLayout_6->addWidget(configurationInterface);
    connect(this, SIGNAL(sendControlData(QString,QString)), configurationInterface, SLOT(receiveControlData(QString,QString)));

    userConfig = new UserConfig();
    ui->gridLayout_SystemConfig->addWidget(userConfig);

    statisticAnalysis = new StatisticAnalysis();
    ui->gridLayout_StatisticAnalysis->addWidget(statisticAnalysis);

    aboutDialog = new AboutDialog();

    paraConfig = new ParaConfig();
    connect(this, SIGNAL(modifyConnStatus(QString,int)), paraConfig, SLOT(slot_modifyConnStatus(QString,int)));

    ui->pushButton_HomePage->setIcon(QIcon(":/images/images/HomePage/homepage.png"));
    ui->pushButton_HomePage->setCheckable(true);
    ui->pushButton_HomePage->setFlat(true);
    ui->pushButton_ConfigurationInterface->setIcon(QIcon(":/images/images/HomePage/configuration.png"));
    ui->pushButton_ConfigurationInterface->setCheckable(true);
    ui->pushButton_ConfigurationInterface->setFlat(true);
    ui->pushButton_RealTime->setIcon(QIcon(":/images/images/HomePage/alarm.png"));
    ui->pushButton_RealTime->setCheckable(true);
    ui->pushButton_RealTime->setFlat(true);
    ui->pushButton_SystemConfig->setIcon(QIcon(":/images/images/HomePage/systemconfig.png"));
    ui->pushButton_SystemConfig->setCheckable(true);
    ui->pushButton_SystemConfig->setFlat(true);
    ui->pushButton_HistoryQuery->setIcon(QIcon(":/images/images/HomePage/query.png"));
    ui->pushButton_HistoryQuery->setCheckable(true);
    ui->pushButton_HistoryQuery->setFlat(true);
    ui->pushButton_StatisticAnalysis->setIcon(QIcon(":/images/images/HomePage/statistic.png"));
    ui->pushButton_StatisticAnalysis->setCheckable(true);
    ui->pushButton_StatisticAnalysis->setFlat(true);
    ui->pushButton_login->setIcon(QIcon(":/images/images/HomePage/login.png"));
    ui->pushButton_login->setCheckable(true);
    ui->pushButton_login->setFlat(true);
    ui->pushButton_ParaConfig->setIcon(QIcon(":images/images/RealTimeAlarm/config.png"));
    ui->pushButton_ParaConfig->setCheckable(true);
    ui->pushButton_ParaConfig->setFlat(true);
    ui->pushButton_Help->setIcon(QIcon(":/images/images/HomePage/about.png"));
    ui->pushButton_Help->setCheckable(true);
    ui->pushButton_Help->setFlat(true);

    connect(ui->pushButton_HomePage, SIGNAL(clicked()), this, SLOT(switchCurrentPage()));
    connect(ui->pushButton_HistoryQuery, SIGNAL(clicked()), this, SLOT(switchCurrentPage()));
    connect(ui->pushButton_ConfigurationInterface, SIGNAL(clicked()), this, SLOT(switchCurrentPage()));
    connect(ui->pushButton_SystemConfig, SIGNAL(clicked()), this, SLOT(switchCurrentPage()));
    connect(ui->pushButton_RealTime, SIGNAL(clicked()), this, SLOT(switchCurrentPage()));
    connect(ui->pushButton_StatisticAnalysis, SIGNAL(clicked()), this, SLOT(switchCurrentPage()));
    connect(ui->pushButton_login, SIGNAL(clicked()), this, SLOT(switchCurrentPage()));
    connect(ui->pushButton_ParaConfig, SIGNAL(clicked()), this, SLOT(switchCurrentPage()));

    connect(ui->pushButton_Help, SIGNAL(clicked()), this, SLOT(clickAbout()));

    ui->pushButton_ConfigurationInterface->hide();
}

void Widget::initAlarmButtonsConnects()
{
    ui->gridLayout_4->setAlignment(Qt::AlignTop);

    QLabel *label1 = new QLabel(QString::fromLocal8Bit("一次设备智能告警"));
    label1->setStyleSheet("color: white; font-size: 20px;");
    label1->setAlignment(Qt::AlignHCenter | Qt::AlignBottom);
    ui->gridLayout_4->addWidget(label1, 0, 0, 1, 3);

    pushButton_SwitchStatusCheck = new PushButton(QString::fromUtf8("开关分合\n状态检查"));
    pushButton_SwitchStatusCheck->setIcon(":/images/images/Scene/switchstatuscheck.png");
    pushButton_SwitchStatusCheck->setFixedSize(100,110);
    connect(pushButton_SwitchStatusCheck, SIGNAL(clicked()), this, SLOT(switchAlarmPage()));
    ui->gridLayout_4->addWidget(pushButton_SwitchStatusCheck, 1, 0);

    pushButton_SwitchActionTimeCheck  = new PushButton(QString::fromUtf8("开关分合动\n作时间检验"));
    pushButton_SwitchActionTimeCheck->setIcon(":/images/images/Scene/switchactiontimecheck.png");
    pushButton_SwitchActionTimeCheck->setFixedSize(100,110);
    connect(pushButton_SwitchActionTimeCheck, SIGNAL(clicked()), this, SLOT(switchAlarmPage()));
    ui->gridLayout_4->addWidget(pushButton_SwitchActionTimeCheck, 1, 1);

    pushButton_PrimaryEquipmentAbnormal = new PushButton(QString::fromUtf8("一次设备异常"));
    pushButton_PrimaryEquipmentAbnormal->setIcon(":/images/images/Scene/primaryequipmentabnormal.png");
    pushButton_PrimaryEquipmentAbnormal->setFixedSize(100,110);
    connect(pushButton_PrimaryEquipmentAbnormal, SIGNAL(clicked()), this, SLOT(switchAlarmPage()));
    ui->gridLayout_4->addWidget(pushButton_PrimaryEquipmentAbnormal, 1, 2);


    QLabel *label2 = new QLabel(QString::fromLocal8Bit("二次设备智能告警"));
    label2->setStyleSheet("color: white; font-size: 20px;");
    label2->setAlignment(Qt::AlignHCenter | Qt::AlignBottom);
    label2->setMinimumHeight(50);
    ui->gridLayout_4->addWidget(label2, 2, 0, 1, 3);

    pushButton_SecondaryEquipmentAbnormal = new PushButton(QString::fromUtf8("二次设备异常"));
    pushButton_SecondaryEquipmentAbnormal->setIcon(":/images/images/Scene/secondaryequipmentabnormal.png");
    pushButton_SecondaryEquipmentAbnormal->setFixedSize(100,110);
    connect(pushButton_SecondaryEquipmentAbnormal, SIGNAL(clicked()), this, SLOT(switchAlarmPage()));
    ui->gridLayout_4->addWidget(pushButton_SecondaryEquipmentAbnormal, 3, 0);

    pushButton_SingleBayProtectAction = new PushButton(QString::fromUtf8("保护动作"));
    pushButton_SingleBayProtectAction->setIcon(":/images/images/Scene/singlebayprotectaction.png");
    pushButton_SingleBayProtectAction->setFixedSize(100,110);
    connect(pushButton_SingleBayProtectAction, SIGNAL(clicked()), this, SLOT(switchAlarmPage()));
    ui->gridLayout_4->addWidget(pushButton_SingleBayProtectAction, 3, 1);

    pushButton_RemoteControlProcess = new PushButton(QString::fromUtf8("遥控过程分析"));
    pushButton_RemoteControlProcess->setIcon(":/images/images/Scene/remotecontrolprocess.png");
    pushButton_RemoteControlProcess->setFixedSize(100,110);
    connect(pushButton_RemoteControlProcess, SIGNAL(clicked()), this, SLOT(switchAlarmPage()));
    ui->gridLayout_4->addWidget(pushButton_RemoteControlProcess, 3, 2);

    pushButton_StationTimeSyncAbnormol = new PushButton(QString::fromUtf8("全站时间\n同步异常"));
    pushButton_StationTimeSyncAbnormol->setIcon(":/images/images/Scene/stationtimesyncabnormol.png");
    pushButton_StationTimeSyncAbnormol->setFixedSize(100,110);
    connect(pushButton_StationTimeSyncAbnormol, SIGNAL(clicked()), this, SLOT(switchAlarmPage()));
    ui->gridLayout_4->addWidget(pushButton_StationTimeSyncAbnormol, 4, 0);

    pushButton_DeviceCommInterrupt = new PushButton(QString::fromUtf8("通信异常"));
    pushButton_DeviceCommInterrupt->setIcon(":/images/images/Scene/devicecomminterrupt.png");
    pushButton_DeviceCommInterrupt->setFixedSize(100,110);
    connect(pushButton_DeviceCommInterrupt, SIGNAL(clicked()), this, SLOT(switchAlarmPage()));
    ui->gridLayout_4->addWidget(pushButton_DeviceCommInterrupt, 4, 1);

    pushButton_SoeTimestampChange = new PushButton(QString::fromUtf8("SOE时标跳变"));
    pushButton_SoeTimestampChange->setIcon(":/images/images/Scene/soetimestampchange.png");
    pushButton_SoeTimestampChange->setFixedSize(100,110);
    connect(pushButton_SoeTimestampChange, SIGNAL(clicked()), this, SLOT(switchAlarmPage()));
    ui->gridLayout_4->addWidget(pushButton_SoeTimestampChange, 4, 2);

    pushButton_RemoteMeasuring = new PushButton(QString::fromUtf8("遥测跳变"));
    pushButton_RemoteMeasuring->setIcon(":/images/images/Scene/remotemeasuring.png");
    pushButton_RemoteMeasuring->setFixedSize(100,110);
    connect(pushButton_RemoteMeasuring, SIGNAL(clicked()), this, SLOT(switchAlarmPage()));
    ui->gridLayout_4->addWidget(pushButton_RemoteMeasuring, 5, 0);

    pushButton_RemoteSignalling = new PushButton(QString::fromUtf8("遥信状态\n不一致"));
    pushButton_RemoteSignalling->setIcon(":/images/images/Scene/remotesignalling.png");
    pushButton_RemoteSignalling->setFixedSize(100,110);
    connect(pushButton_RemoteSignalling, SIGNAL(clicked()), this, SLOT(switchAlarmPage()));
    ui->gridLayout_4->addWidget(pushButton_RemoteSignalling, 5, 1);

    QLabel *label3 = new QLabel(QString::fromLocal8Bit("辅助系统智能告警"));
    label3->setStyleSheet("color: white; font-size: 20px;");
    label3->setAlignment(Qt::AlignHCenter | Qt::AlignBottom);
    label3->setMinimumHeight(50);
    ui->gridLayout_4->addWidget(label3, 6, 0, 1, 3);

    pushButton_AuxiliaryPowerCheck = new PushButton(QString::fromUtf8("辅助电源\n状态检测"));
    pushButton_AuxiliaryPowerCheck->setIcon(":/images/images/Scene/auxiliarypowercheck.png");
    pushButton_AuxiliaryPowerCheck->setFixedSize(100,110);
    connect(pushButton_AuxiliaryPowerCheck, SIGNAL(clicked()), this, SLOT(switchAlarmPage()));
    ui->gridLayout_4->addWidget(pushButton_AuxiliaryPowerCheck, 7, 0);

    pushButton_ElectricRunCheck = new PushButton(QString::fromUtf8("电气运行\n环境检测"));
    pushButton_ElectricRunCheck->setIcon(":/images/images/Scene/electricruncheck.png");
    pushButton_ElectricRunCheck->setFixedSize(100,110);
    connect(pushButton_ElectricRunCheck, SIGNAL(clicked()), this, SLOT(switchAlarmPage()));
    ui->gridLayout_4->addWidget(pushButton_ElectricRunCheck, 7, 1);

    pushButton_PowerSystemAbnormal = new PushButton(QString::fromUtf8("电源系统异常"));
    pushButton_PowerSystemAbnormal->setIcon(":/images/images/Scene/powersystemabnormal.png");
    pushButton_PowerSystemAbnormal->setFixedSize(100,110);
    connect(pushButton_PowerSystemAbnormal, SIGNAL(clicked()), this, SLOT(switchAlarmPage()));
    ui->gridLayout_4->addWidget(pushButton_PowerSystemAbnormal, 7, 2);

    //遥控过程分析
    remoteControlProcess = new RemoteControlProcess();
    ui->gridLayout_RemoteControl->addWidget(remoteControlProcess);
    connect(remoteControlProcess, SIGNAL(increase(int)), pushButton_RemoteControlProcess, SLOT(increase(int)));

    //开关分合状态检查
    switchStatusCheck = new SwitchStatusCheck();
    ui->gridLayout_SwitchStatusCheck->addWidget(switchStatusCheck);
    connect(switchStatusCheck, SIGNAL(increase(int)), pushButton_SwitchStatusCheck, SLOT(increase(int)));

    //开关分合动作时间检验
    switchActionTimeCheck = new SwitchActionTimeCheck();
    ui->gridLayout_SwitchActionTimeCheck->addWidget(switchActionTimeCheck);
    connect(switchActionTimeCheck, SIGNAL(increase(int)), pushButton_SwitchActionTimeCheck, SLOT(increase(int)));

    //遥信状态\n不一致
    remoteSignalling = new RemoteSignalling();
    ui->gridLayout_RemoteSignalling->addWidget(remoteSignalling);
    connect(remoteSignalling, SIGNAL(increase(int)), pushButton_RemoteSignalling, SLOT(increase(int)));

    //遥测跳变
    remoteMeasuring = new RemoteMeasuring();
    ui->gridLayout_RemoteMeasuring->addWidget(remoteMeasuring);
    connect(remoteMeasuring, SIGNAL(increase(int)), pushButton_RemoteMeasuring, SLOT(increase(int)));

    //单间隔保护
    singleBayProtectAction = new SingleBayProtectAction();
    ui->gridLayout_SingleBayProtectAction->addWidget(singleBayProtectAction);
    connect(singleBayProtectAction, SIGNAL(increase(int)), pushButton_SingleBayProtectAction, SLOT(increase(int)));

    //SOE时标\n跳变
    soeTimestampChange = new SoeTimestampChange();
    ui->gridLayout_SoeTimestampChange->addWidget(soeTimestampChange);
    connect(soeTimestampChange, SIGNAL(increase(int)), pushButton_SoeTimestampChange, SLOT(increase(int)));

    //全站时间\n同步异常
    stationTimeSyncAbnormol = new StationTimeSyncAbnormol();
    ui->gridLayout_StationTimeSyncAbnormol->addWidget(stationTimeSyncAbnormol);
    connect(stationTimeSyncAbnormol, SIGNAL(increase(int)), pushButton_StationTimeSyncAbnormol, SLOT(increase(int)));

    //装置通信中断
    deviceCommInterrupt = new DeviceCommInterrupt();
    ui->gridLayout_DeviceCommInterrupt->addWidget(deviceCommInterrupt);
    connect(deviceCommInterrupt, SIGNAL(increase(int)), pushButton_DeviceCommInterrupt, SLOT(increase(int)));

    //一次设备异常
    primaryEquipmentAbnormal = new PrimaryEquipmentAbnormal();
    ui->gridLayout_PrimaryEquipmentAbnormal->addWidget(primaryEquipmentAbnormal);
    connect(primaryEquipmentAbnormal, SIGNAL(increase(int)), pushButton_PrimaryEquipmentAbnormal, SLOT(increase(int)));

    //二次设备异常
    secondaryEquipmentAbnormal = new SecondaryEquipmentAbnormal();
    ui->gridLayout_SecondaryEquipmentAbnormal->addWidget(secondaryEquipmentAbnormal);
    connect(secondaryEquipmentAbnormal, SIGNAL(increase(int)), pushButton_SecondaryEquipmentAbnormal, SLOT(increase(int)));

    //电源系统异常
    powerSystemAbnormal = new PowerSystemAbnormal();
    ui->gridLayout_PowerSystemAbnormal->addWidget(powerSystemAbnormal);
    connect(powerSystemAbnormal, SIGNAL(increase(int)), pushButton_PowerSystemAbnormal, SLOT(increase(int)));

    //电气运行环境检测
    electricRunCheck = new ElectricRunCheck();
    ui->gridLayout_ElectricRunCheck->addWidget(electricRunCheck);
    connect(electricRunCheck, SIGNAL(increase(int)), pushButton_ElectricRunCheck, SLOT(increase(int)));

    //辅助电源状态检测
    auxiliaryPowerCheck = new AuxiliaryPowerCheck();
    ui->gridLayout_AuxiliaryPowerCheck->addWidget(auxiliaryPowerCheck);
    connect(auxiliaryPowerCheck, SIGNAL(increase(int)), pushButton_AuxiliaryPowerCheck, SLOT(increase(int)));
}



void Widget::logoutCurUser()
{
    if(ui->label_CurUser->text().isEmpty())
        return;

    QMessageBox::StandardButton button = QMessageBox::information(NULL, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("确认注销当前用户?"), QMessageBox::Ok | QMessageBox::Cancel);
    if(button == QMessageBox::Ok)
    {
        ui->label_CurUser->setText("");
        SingletonConfig->setCurUser("");
    }
}

Widget::~Widget()
{
    SingletonConfig->writeColorToFile();
    redisHelper->publish(REDIS_CHANNEL_ALARM, "exit");
    delete ui;
}

void Widget::centerDisplay()
{
    QDesktopWidget *screenResolution = QApplication::desktop();
    int appWindowWidth = this->geometry().width();
    int appWindowHeight = this->geometry().height();
    int center_y = screenResolution->height()/2 - appWindowHeight/2;
    int center_x = screenResolution->width()/2 - appWindowWidth/2;
    setGeometry(center_x, center_y, appWindowWidth,appWindowHeight);
}

bool Widget::eventFilter(QObject *obj, QEvent *eve)
{
    if(obj == ui->label_CurUser && eve->type() == QEvent::MouseButtonPress)
    {
        logoutCurUser();
    }
    return QWidget::eventFilter(obj, eve);
}

void Widget::switchCurrentPage()
{
    QPushButton *button = static_cast<QPushButton*>(sender());         //根据触发的按钮来进行所要显示的QWidget
    realTimeAlarm->hide();
    if (button == ui->pushButton_HomePage)
    {
        ui->stackedWidget->setCurrentIndex(0);
        ui->tabWidget->show();
        ui->frame_scene->show();
    }
    else if (button == ui->pushButton_HistoryQuery)
    {
        ui->stackedWidget->setCurrentIndex(1);
    }
    else if (button == ui->pushButton_ConfigurationInterface)
    {
        ui->stackedWidget->setCurrentIndex(0);
        ui->tabWidget->hide();
        ui->frame_scene->hide();
    }
    else if (button == ui->pushButton_StatisticAnalysis)
    {
        ui->stackedWidget->setCurrentIndex(4);
    }
    else if (button == ui->pushButton_SystemConfig)
    {
        ui->stackedWidget->setCurrentIndex(5);
    }
    else if (button == ui->pushButton_RealTime)
    {
        realTimeAlarm->show();
    }
    else if (button == ui->pushButton_login)
    {
        loginWindow->show();
    }
    else if (button == ui->pushButton_ParaConfig)
    {
        paraConfig->show();
    }

}

void Widget::switchAlarmPage()
{
    QPushButton *button = static_cast<QPushButton*>(sender());     //根据触发的按钮来进行所要显示的QWidget
    ui->stackedWidget->setCurrentIndex(2);
    if (button == pushButton_RemoteControlProcess)
    {
        ui->stackedWidget_AlarmPage->setCurrentIndex(0);
    }else if(button == pushButton_SwitchStatusCheck)
    {
        ui->stackedWidget_AlarmPage->setCurrentIndex(1);
    }else if(button == pushButton_RemoteSignalling)
    {
        ui->stackedWidget_AlarmPage->setCurrentIndex(2);
    }else if(button == pushButton_RemoteMeasuring)
    {
        ui->stackedWidget_AlarmPage->setCurrentIndex(3);
    }else if(button == pushButton_SingleBayProtectAction)
    {
        ui->stackedWidget_AlarmPage->setCurrentIndex(4);
    }else if(button == pushButton_SoeTimestampChange)
    {
        ui->stackedWidget_AlarmPage->setCurrentIndex(5);
    }else if(button == pushButton_StationTimeSyncAbnormol)
    {
        ui->stackedWidget_AlarmPage->setCurrentIndex(6);
    }else if(button == pushButton_DeviceCommInterrupt)
    {
        ui->stackedWidget_AlarmPage->setCurrentIndex(7);
    }else if(button == pushButton_PrimaryEquipmentAbnormal)
    {
        ui->stackedWidget_AlarmPage->setCurrentIndex(8);
    }else if(button == pushButton_SecondaryEquipmentAbnormal)
    {
        ui->stackedWidget_AlarmPage->setCurrentIndex(9);
    }else if(button == pushButton_SwitchActionTimeCheck)
    {
        ui->stackedWidget_AlarmPage->setCurrentIndex(10);
    }else if(button == pushButton_PowerSystemAbnormal)
    {
        ui->stackedWidget_AlarmPage->setCurrentIndex(11);
    }
    else if(button == pushButton_ElectricRunCheck)
    {
        ui->stackedWidget_AlarmPage->setCurrentIndex(12);
    } else if(button == pushButton_AuxiliaryPowerCheck)
    {
        ui->stackedWidget_AlarmPage->setCurrentIndex(13);
    }
}


void Widget::redisSubscribe()
{
    SingleRedisHelp->setConnParas(QString("%1:%2").arg(SingletonConfig->getIpRedis()).arg(SingletonConfig->getPortRedis()).toStdString(), SingletonConfig->getPasswdRedis().toStdString());

    while(isRunning)
    {
        if(!SingleRedisHelp->check_connect())
        {
            if(SingleRedisHelp->open())
            {
                qDebug()<<QString("Redis Connect Success:%1:%2").arg(SingletonConfig->getIpRedis()).arg(SingletonConfig->getPortRedis());
                if(SingleRedisHelp->subscribe(REDIS_CHANNEL_ALARM,REDIS_CHANNEL_ALARMCALC, NULL) >= 1)
                    qDebug()<<QString("Redis Subscribe Success:%1, %2").arg(REDIS_CHANNEL_ALARM).arg(REDIS_CHANNEL_ALARMCALC);
            }else
            {
                qDebug()<<QString("Redis Connect Failure:%1:%2").arg(SingletonConfig->getIpRedis()).arg(SingletonConfig->getPortRedis());
                sleep(1);
                continue;
            }
        }

        string message;
        if(SingleRedisHelp->getMessage(message))
        {
            qDebug()<<message.c_str();
            RtdbMessage rtdbMessage;
            if(rtdbMessage.ParseFromString(message))
            {
                switch(rtdbMessage.messagetype())
                {
                case TYPE_ALARMDATAMSG:
                {
                    AlarmDataMsg alarmDataMsg = rtdbMessage.alarmdatamsg();
                    qDebug()<<alarmDataMsg.id().c_str()<<alarmDataMsg.level()<<alarmDataMsg.time().c_str()<<
                              QString::fromUtf8(alarmDataMsg.devicename().c_str())<<QString::fromUtf8(alarmDataMsg.msg().c_str())
                           <<QString::fromUtf8(alarmDataMsg.scene().c_str())<<alarmDataMsg.filename().c_str();               //对方用QString::toStdString，不能用UTF8接受 要用toUtf8

                    if(alarmDataMsg.type() == 1)           //原始告警
                    {
                        stAlarmData alarmData;
                        alarmData.id_ = QString::fromStdString(alarmDataMsg.id()).toInt();
                        alarmData.level_ = alarmDataMsg.level();
                        alarmData.time_ = QString::fromStdString(alarmDataMsg.time());
                        alarmData.text_ = QString::fromUtf8(alarmDataMsg.devicename().c_str()) + " " + QString::fromUtf8(alarmDataMsg.msg().c_str());
                        alarmData.confirmed_ = 0;
                        SingletonAlarmData->insertOriginalAlarmData(alarmData);
                    }
                    else if(alarmDataMsg.type() == 2)    //智能告警
                    {
                        stAlarmData alarmData;
                        string id = alarmDataMsg.id();
                        alarmData.id_ = QString::fromStdString(id).left(id.length() - 1).toInt();
                        alarmData.level_ = alarmDataMsg.level();
                        alarmData.time_ = QString::fromStdString(alarmDataMsg.time());
                        alarmData.text_ = QString::fromUtf8(alarmDataMsg.devicename().c_str()) + " " + QString::fromUtf8(alarmDataMsg.msg().c_str());
                        alarmData.confirmed_ = 0;
                        alarmData.filename_ =  QString(alarmDataMsg.filename().c_str());

                        SingletonAlarmData->insertSmartAlarmData(alarmData);

                        //告警弹出
                        if(SingletonConfig->getAlarmLevelFlag(alarmDataMsg.level()) == 1)
                            ui->pushButton_RealTime->click();

                        dispSmartAlarmData(alarmData.id_, alarmData);
                    }
                    break;
                }
                case TYPE_REMOTECONTROL:
                    break;
                case TYPE_REALPOINT:
                {
                    QString strPointName = QString(rtdbMessage.realpointvalue().pointaddr().c_str());
                    QString strValue = QString(rtdbMessage.realpointvalue().pointvalue().c_str());

                    emit sendControlData(strPointName, strValue);
                    break;
                }
                case TYPE_STATIONCONMSG:
                {
                    StationConMsg stationConMsg = rtdbMessage.stationconmsg();
                    QString stationIp = QString::fromStdString(stationConMsg.stationip());
                    int status = stationConMsg.statetype();

                    emit modifyConnStatus(stationIp, status);
                    break;
                }
                default:
                    break;
                }
            }
            else
            {
                qDebug()<<"ParseFromString Failure";
            }
        }
        usleep(1000);
    }
}

void Widget::setCurUser(QString curUser)
{
    ui->label_CurUser->setText(curUser);
}

void Widget::clickAbout()
{
    aboutDialog->show();
}

void Widget::dispSmartAlarmData(int id, stAlarmData alarmData)
{
    ui->tableWidget_SmartAlarm->insertRow(0); //插入一个空行

    QTableWidgetItem *item0 = new QTableWidgetItem(alarmData.time_);
    item0->setTextColor(SingletonConfig->getAlarmLevelColor(alarmData.level_));
    item0->setTextAlignment(Qt::AlignCenter);

    QTableWidgetItem *item1 = new QTableWidgetItem(alarmData.text_);
    item1->setTextColor(SingletonConfig->getAlarmLevelColor(alarmData.level_));
    item1->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    ui->tableWidget_SmartAlarm->setItem(0,0,item0);
    ui->tableWidget_SmartAlarm->setItem(0,1,item1);
}

void Widget::dispOriginalAlarmData(int id, stAlarmData alarmData)
{
    ui->tableWidget_OriginalAlarm->insertRow(0); //插入一个空行

    QTableWidgetItem *item0 = new QTableWidgetItem(alarmData.time_);
    item0->setTextColor(SingletonConfig->getAlarmLevelColor(alarmData.level_));
    item0->setTextAlignment(Qt::AlignCenter);

    QTableWidgetItem *item1 = new QTableWidgetItem(alarmData.text_);
    item1->setTextColor(SingletonConfig->getAlarmLevelColor(alarmData.level_));
    item1->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    ui->tableWidget_OriginalAlarm->setItem(0,0,item0);
    ui->tableWidget_OriginalAlarm->setItem(0,1,item1);
}
