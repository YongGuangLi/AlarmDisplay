#include "realtimealarm.h"
#include "ui_realtimealarm.h"

RealTimeAlarm::RealTimeAlarm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RealTimeAlarm)
{
    ui->setupUi(this);
    this->setWindowTitle(QString::fromLocal8Bit("智能告警实时展示"));

    centerDisplay();

    initWidgetDisp();
    //智能告警等级过滤
    initSmartRadioButtons();
    //原始告警等级过滤
    initOriginalRadioButtons();
    //隐藏最小化窗口
    this->setWindowFlags(this->windowFlags() &~ Qt::WindowMinimizeButtonHint);
    this->setWindowIcon(QIcon(":/images/images/aboutlogo.png"));

    //接收并显示告警数据
    connect(SingletonAlarmData, SIGNAL(receiverSmartAlarmData(int,stAlarmData)), this, SLOT(dispSmartAlarmData(int, stAlarmData)));
    connect(SingletonAlarmData, SIGNAL(receiverOriginalAlarmData(int,stAlarmData)), this, SLOT(dispOriginalAlarmData(int, stAlarmData)));

    //遥控过程
    remoteControlWidget = new RemoteControlWidget();

    //告警详情
    alarmDetail = new AlarmDetail();
    alarmDetail->addRemoteControlProcess(remoteControlWidget);

    //间隔过滤
    connect(ui->pushButton_SmartBayFilter, SIGNAL(clicked()), this, SLOT(selectFilterBay()));
    connect(ui->pushButton_SmartBayShield, SIGNAL(clicked()), this, SLOT(selectFilterBay()));
    ui->pushButton_SmartBayFilter->setFlat(true);
    ui->pushButton_SmartBayShield->setFlat(true);

    connect(ui->pushButton_OriginalBayFilter, SIGNAL(clicked()), this, SLOT(selectFilterBay()));
    connect(ui->pushButton_OriginalBayShield, SIGNAL(clicked()), this, SLOT(selectFilterBay()));
    ui->pushButton_OriginalBayFilter->setFlat(true);
    ui->pushButton_OriginalBayShield->setFlat(true);

    //修改告警等级颜色,重新显示
    alarmDispConfig = new AlarmDispConfig();
    connect(alarmDispConfig, SIGNAL(alarmLevelColorChange()), this, SLOT(replaySmartAlarmData()));
    connect(alarmDispConfig, SIGNAL(alarmLevelColorChange()), this, SLOT(replayOriginalAlarmData()));

    curSelectIaAlarmLevel = LEVEL_REALTIME;
    curSelectOrAlarmLevel = LEVEL_REALTIME;

    mapAlarmLevelLogo[LEVEL_EVENT] = QIcon(":/images/images/AlarmLogo/Event.png");
    mapAlarmLevelLogo[LEVEL_ABNORMAL] = QIcon(":/images/images/AlarmLogo/Abnormal.png");
    mapAlarmLevelLogo[LEVEL_OUTOFLIMIT] = QIcon(":/images/images/AlarmLogo/OutLimit.png");
    mapAlarmLevelLogo[LEVEL_CHANGE] = QIcon(":/images/images/AlarmLogo/Change.png");
    mapAlarmLevelLogo[LEVEL_INFORM] = QIcon(":/images/images/AlarmLogo/Inform.png");

    pFilterBay = new FilterBay();
    connect(pFilterBay, SIGNAL(sendSmartConfirmCmd()), this, SLOT(replaySmartAlarmData()));
    connect(pFilterBay, SIGNAL(sendOriginalConfirmCmd()), this, SLOT(replayOriginalAlarmData()));

    ui->pushButton_historyQuery->hide();
    ui->pushButton_RealTime_Ia->click();
    ui->pushButton_RealTime_Oa->click();

    connect(ui->pushButton_SmartPause, SIGNAL(clicked(bool)), this, SLOT(pauseSmartAlarmData(bool)));
    connect(ui->pushButton_OriginalPause, SIGNAL(clicked(bool)), this, SLOT(pauseOriginalAlarmData(bool)));

    ui->pushButton_SmartPause->setCheckable(true);
    ui->pushButton_SmartPause->setFlat(true);
    ui->pushButton_OriginalPause->setCheckable(true);
    ui->pushButton_OriginalPause->setFlat(true);

    ui->pushButton_alarmConfig->setFlat(true);
}

RealTimeAlarm::~RealTimeAlarm()
{
    delete ui;
}

void RealTimeAlarm::centerDisplay()
{
    QDesktopWidget *screenResolution = QApplication::desktop();
    int appWindowWidth = this->geometry().width();
    int appWindowHeight = this->geometry().height();
    int center_y = screenResolution->height()/2 - appWindowHeight/2;
    int center_x = screenResolution->width()/2 - appWindowWidth/2;
    setGeometry(center_x, center_y, appWindowWidth,appWindowHeight);
}

void RealTimeAlarm::initWidgetDisp()
{
    connect(ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(alarmTabWidgetChanged(int)));

    ui->tableWidget_SmartAlarm->verticalHeader()->hide();
    ui->tableWidget_SmartAlarm->setEditTriggers(QAbstractItemView::NoEditTriggers);    //设置内容不可更改
    ui->tableWidget_SmartAlarm->setContextMenuPolicy(Qt::CustomContextMenu);           //允许右击菜单
    ui->tableWidget_SmartAlarm->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget_SmartAlarm->horizontalHeader()->setResizeMode(1, QHeaderView::ResizeToContents);
    ui->tableWidget_SmartAlarm->horizontalHeader()->setResizeMode(3, QHeaderView::ResizeToContents);
    ui->tableWidget_SmartAlarm->setColumnHidden(4, true);
    ui->tableWidget_SmartAlarm->setSelectionBehavior(QAbstractItemView::SelectRows);   //单击选择一行
    ui->tableWidget_SmartAlarm->setShowGrid(true);

    ui->tableWidget_OriginalAlarm->verticalHeader()->hide();
    ui->tableWidget_OriginalAlarm->setEditTriggers(QAbstractItemView::NoEditTriggers);       //设置内容不可更改
    ui->tableWidget_OriginalAlarm->setContextMenuPolicy(Qt::CustomContextMenu);              //允许右击菜单
    ui->tableWidget_OriginalAlarm->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget_OriginalAlarm->horizontalHeader()->setResizeMode(1, QHeaderView::ResizeToContents);
    ui->tableWidget_OriginalAlarm->horizontalHeader()->setResizeMode(3, QHeaderView::ResizeToContents);
    ui->tableWidget_OriginalAlarm->setColumnHidden(4, true);
    ui->tableWidget_OriginalAlarm->setSelectionBehavior(QAbstractItemView::SelectRows);      //单击选择一行
    ui->tableWidget_OriginalAlarm->setShowGrid(true);

    ui->pushButton_SmartBayFilter->setIcon(QIcon(":/images/images/RealTimeAlarm/filter.png"));
    ui->pushButton_SmartBayShield->setIcon(QIcon(":/images/images/RealTimeAlarm/shield.png"));
    ui->pushButton_SmartPause->setIcon(QIcon(":/images/images/RealTimeAlarm/pause.png"));

    ui->pushButton_OriginalBayFilter->setIcon(QIcon(":/images/images/RealTimeAlarm/filter.png"));
    ui->pushButton_OriginalBayShield->setIcon(QIcon(":/images/images/RealTimeAlarm/shield.png"));
    ui->pushButton_OriginalPause->setIcon(QIcon(":/images/images/RealTimeAlarm/pause.png"));

    ui->pushButton_historyQuery->setIcon(QIcon(":/images/images/RealTimeAlarm/query.png"));
    ui->pushButton_alarmConfig->setIcon(QIcon(":/images/images/RealTimeAlarm/config.png"));

    ui->pushButton_Event_Ia->setIcon(QIcon(":/images/images/AlarmLogo/event.png"));
    ui->pushButton_Abnormal_Ia->setIcon(QIcon(":/images/images/AlarmLogo/abnormal.png"));
    ui->pushButton_OutLimit_Ia->setIcon(QIcon(":/images/images/AlarmLogo/outlimit.png"));
    ui->pushButton_Change_Ia->setIcon(QIcon(":/images/images/AlarmLogo/change.png"));
    ui->pushButton_Inform_Ia->setIcon(QIcon(":/images/images/AlarmLogo/inform.png"));
    ui->pushButton_RealTime_Ia->setIcon(QIcon(":/images/images/AlarmLogo/realtime.png"));


    ui->pushButton_Event_Oa->setIcon(QIcon(":/images/images/AlarmLogo/event.png"));
    ui->pushButton_Abnormal_Oa->setIcon(QIcon(":/images/images/AlarmLogo/abnormal.png"));
    ui->pushButton_OutLimit_Oa->setIcon(QIcon(":/images/images/AlarmLogo/outlimit.png"));
    ui->pushButton_Change_Oa->setIcon(QIcon(":/images/images/AlarmLogo/change.png"));
    ui->pushButton_Inform_Oa->setIcon(QIcon(":/images/images/AlarmLogo/inform.png"));
    ui->pushButton_RealTime_Oa->setIcon(QIcon(":/images/images/AlarmLogo/realtime.png"));


    //告警表右击菜单
    connect(ui->tableWidget_SmartAlarm, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showCustomMenu(QPoint)));
    connect(ui->tableWidget_OriginalAlarm, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showCustomMenu(QPoint)));


    //智能告警表右击菜单列表
    pSmartAlarmMenu = new QMenu(ui->tableWidget_SmartAlarm);
    //修改告警状态
    QAction *iaAlarmStatusAction = new QAction(QString::fromLocal8Bit("确认"), ui->tableWidget_SmartAlarm);
    connect(iaAlarmStatusAction,SIGNAL(triggered(bool)),this,SLOT(confirmAlarm()));
    pSmartAlarmMenu->addAction(iaAlarmStatusAction);
    //查看告警详情
    QAction *alarmDetailAction = new QAction(QString::fromUtf8("详情"),ui->tableWidget_SmartAlarm);
    connect(alarmDetailAction,SIGNAL(triggered(bool)),this,SLOT(showAlarmDetail()));
    pSmartAlarmMenu->addAction(alarmDetailAction);
    //删除告警
    QAction *iaDeleteAlarmAction = new QAction(QString::fromUtf8("删除告警"),ui->tableWidget_SmartAlarm);
    connect(iaDeleteAlarmAction,SIGNAL(triggered(bool)),this,SLOT(deleteAlarm()));
    pSmartAlarmMenu->addAction(iaDeleteAlarmAction);

    //原始告警表右击菜单
    pOriginalAlarmMenu = new QMenu(ui->tableWidget_OriginalAlarm);
    QAction *oaAlarmStatusAction = new QAction(QString::fromLocal8Bit("确认"), ui->tableWidget_OriginalAlarm);
    connect(oaAlarmStatusAction,SIGNAL(triggered(bool)),this,SLOT(confirmAlarm()));
    pOriginalAlarmMenu->addAction(oaAlarmStatusAction);

    QAction *oaDeleteAlarmAction = new QAction(QString::fromUtf8("删除告警"), ui->tableWidget_OriginalAlarm);
    connect(oaDeleteAlarmAction,SIGNAL(triggered(bool)),this,SLOT(deleteAlarm()));
    pOriginalAlarmMenu->addAction(oaDeleteAlarmAction);
}

void RealTimeAlarm::initSmartRadioButtons()
{
    smartButtonGroup.addButton(ui->pushButton_Event_Ia);
    smartButtonGroup.addButton(ui->pushButton_Abnormal_Ia);
    smartButtonGroup.addButton(ui->pushButton_OutLimit_Ia);
    smartButtonGroup.addButton(ui->pushButton_Change_Ia);
    smartButtonGroup.addButton(ui->pushButton_Inform_Ia);
    smartButtonGroup.addButton(ui->pushButton_RealTime_Ia);

    ui->pushButton_Event_Ia->setCheckable(true);
    ui->pushButton_Abnormal_Ia->setCheckable(true);
    ui->pushButton_OutLimit_Ia->setCheckable(true);
    ui->pushButton_Change_Ia->setCheckable(true);
    ui->pushButton_Inform_Ia->setCheckable(true);
    ui->pushButton_RealTime_Ia->setCheckable(true);

    connect(ui->pushButton_Event_Ia, SIGNAL(clicked()), this, SLOT(changeCurIaAlarmLevel()));
    connect(ui->pushButton_Abnormal_Ia, SIGNAL(clicked()), this, SLOT(changeCurIaAlarmLevel()));
    connect(ui->pushButton_OutLimit_Ia, SIGNAL(clicked()), this, SLOT(changeCurIaAlarmLevel()));
    connect(ui->pushButton_Change_Ia, SIGNAL(clicked()), this, SLOT(changeCurIaAlarmLevel()));
    connect(ui->pushButton_Inform_Ia, SIGNAL(clicked()), this, SLOT(changeCurIaAlarmLevel()));
    connect(ui->pushButton_RealTime_Ia, SIGNAL(clicked()), this, SLOT(changeCurIaAlarmLevel()));
}

void RealTimeAlarm::initOriginalRadioButtons()
{
    originalButtonGroup.addButton(ui->pushButton_Event_Oa);
    originalButtonGroup.addButton(ui->pushButton_Abnormal_Oa);
    originalButtonGroup.addButton(ui->pushButton_OutLimit_Oa);
    originalButtonGroup.addButton(ui->pushButton_Change_Oa);
    originalButtonGroup.addButton(ui->pushButton_Inform_Oa);
    originalButtonGroup.addButton(ui->pushButton_RealTime_Oa);

    ui->pushButton_Event_Oa->setCheckable(true);
    ui->pushButton_Abnormal_Oa->setCheckable(true);
    ui->pushButton_OutLimit_Oa->setCheckable(true);
    ui->pushButton_Change_Oa->setCheckable(true);
    ui->pushButton_Inform_Oa->setCheckable(true);
    ui->pushButton_RealTime_Oa->setCheckable(true);

    connect(ui->pushButton_Event_Oa, SIGNAL(clicked()), this, SLOT(changeCurOrAlarmLevel()));
    connect(ui->pushButton_Abnormal_Oa, SIGNAL(clicked()), this, SLOT(changeCurOrAlarmLevel()));
    connect(ui->pushButton_OutLimit_Oa, SIGNAL(clicked()), this, SLOT(changeCurOrAlarmLevel()));
    connect(ui->pushButton_Change_Oa, SIGNAL(clicked()), this, SLOT(changeCurOrAlarmLevel()));
    connect(ui->pushButton_Inform_Oa, SIGNAL(clicked()), this, SLOT(changeCurOrAlarmLevel()));
    connect(ui->pushButton_RealTime_Oa, SIGNAL(clicked()), this, SLOT(changeCurOrAlarmLevel()));
}


//由于告警颜色改变，间隔过滤，间隔屏蔽，修改过滤告警等级，重新显示
void RealTimeAlarm::replaySmartAlarmData()
{
    ui->tableWidget_SmartAlarm->setRowCount(0);

    QMap<int, stAlarmData> mapSmartAlarmData = SingletonAlarmData->getSmartAlarmData();
    QMapIterator<int, stAlarmData> itSmartAlarmData(mapSmartAlarmData);
    while(itSmartAlarmData.hasNext())
    {
        itSmartAlarmData.next();
        dispSmartAlarmData(itSmartAlarmData.key(), itSmartAlarmData.value());
    }

    //根据告警时间排序
    ui->tableWidget_SmartAlarm->sortByColumn(1, Qt::DescendingOrder);
}

//由于告警颜色改变，间隔过滤，间隔屏蔽，修改过滤告警等级，重新显示
void RealTimeAlarm::replayOriginalAlarmData()
{
    ui->tableWidget_OriginalAlarm->setRowCount(0);

    QMap<int, stAlarmData> mapOriginalAlarmData = SingletonAlarmData->getOriginalAlarmData();
    QMapIterator<int, stAlarmData> itOriginalAlarmData(mapOriginalAlarmData);
    while(itOriginalAlarmData.hasNext())
    {
        itOriginalAlarmData.next();
        dispOriginalAlarmData(itOriginalAlarmData.key(), itOriginalAlarmData.value());
    }
    //根据告警时间排序
    ui->tableWidget_OriginalAlarm->sortByColumn(1, Qt::DescendingOrder);
}

void RealTimeAlarm::alarmTabWidgetChanged(int index)
{
    ui->stackedWidget->setCurrentIndex(index);
}


void RealTimeAlarm::on_pushButton_alarmConfig_clicked()          //修改告警配置
{
    alarmDispConfig->show();
}


/*************************************
Description:  显示智能告警数据
Input:
Output:
Return:
Other:
 **************************************/
void RealTimeAlarm::dispSmartAlarmData(int id, stAlarmData alarmData)
{
    if(curSelectIaAlarmLevel != LEVEL_REALTIME  && curSelectIaAlarmLevel != alarmData.level_)                 //显示所选告警等级
        return;

    if(alarmData.text_.split(".").size() <= 2 )               //数据错误，间隔获取失败
        return;
    QString tmp = alarmData.text_.split(".").at(2);
    QString bayName = tmp.left(tmp.indexOf(" "));
    if(!pFilterBay->judgeBayValid(bayName, 0))
        return;

    QString status = alarmData.confirmed_ == 1 ? QString::fromUtf8("已确认") : QString::fromUtf8("未确认");
    QColor statusColor =  alarmData.confirmed_ == 1 ? Qt::cyan : Qt::red;

    //超过最大设置行数，需要删除
    if(ui->tableWidget_SmartAlarm->rowCount() > SingletonConfig->getMaxSize())
    {
        ui->tableWidget_SmartAlarm->removeRow(SingletonConfig->getMaxSize());
    }

    ui->tableWidget_SmartAlarm->insertRow(0); //插入一个空行

    QTableWidgetItem *item0 = new QTableWidgetItem(status);
    item0->setTextColor(statusColor);
    item0->setTextAlignment(Qt::AlignCenter);

    QTableWidgetItem *item1 = new QTableWidgetItem(alarmData.time_);
    item1->setTextColor(SingletonConfig->getAlarmLevelColor(alarmData.level_));
    item1->setTextAlignment(Qt::AlignCenter);

    QTableWidgetItem *item2 = new QTableWidgetItem(mapAlarmLevelLogo[alarmData.level_], SingletonAlarmData->getDescByAlarmLevel(alarmData.level_));
    item2->setTextColor(SingletonConfig->getAlarmLevelColor(alarmData.level_));
    item2->setTextAlignment(Qt::AlignCenter);

    QTableWidgetItem *item3 = new QTableWidgetItem(alarmData.text_);
    item3->setTextColor(SingletonConfig->getAlarmLevelColor(alarmData.level_));
    item3->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    QTableWidgetItem *item4 = new QTableWidgetItem(QString::number(id));

    ui->tableWidget_SmartAlarm->setItem(0,0,item0);
    ui->tableWidget_SmartAlarm->setItem(0,1,item1);
    ui->tableWidget_SmartAlarm->setItem(0,2,item2);
    ui->tableWidget_SmartAlarm->setItem(0,3,item3);
    ui->tableWidget_SmartAlarm->setItem(0,4,item4);

    //显示第一条
    ui->tableWidget_SmartAlarm->scrollToTop();
}


/*************************************
Description:  显示原始告警数据
Input:
Output:
Return:
Other:
 **************************************/
void RealTimeAlarm::dispOriginalAlarmData(int id, stAlarmData alarmData)
{
    if(curSelectOrAlarmLevel != LEVEL_REALTIME  && curSelectOrAlarmLevel != alarmData.level_)                 //显示所选告警等级
        return;

    //alarmData.text_ 茂名电网.广场变/10kV.F05馈线.SL10049/B02GOOSE报警
    if(alarmData.text_.split(".").size() <= 2 )               //数据错误，间隔获取失败
        return;
    QString bayName = alarmData.text_.split(".").at(2);
    if(!pFilterBay->judgeBayValid(bayName, 1))
        return;

    QString status = alarmData.confirmed_ == 1 ? QString::fromUtf8("已确认") : QString::fromUtf8("未确认");
    QColor statusColor =  alarmData.confirmed_ == 1 ? Qt::cyan : Qt::red;

    //超过最大设置行数，需要删除
    if(ui->tableWidget_OriginalAlarm->rowCount() > SingletonConfig->getMaxSize())
    {
        ui->tableWidget_OriginalAlarm->removeRow(SingletonConfig->getMaxSize());
    }

    ui->tableWidget_OriginalAlarm->insertRow(0); //插入一个空行

    QTableWidgetItem *item0 = new QTableWidgetItem(status);
    item0->setTextColor(statusColor);
    item0->setTextAlignment(Qt::AlignCenter);

    QTableWidgetItem *item1 = new QTableWidgetItem(alarmData.time_);
    item1->setTextColor(SingletonConfig->getAlarmLevelColor(alarmData.level_));
    item1->setTextAlignment(Qt::AlignCenter);

    QTableWidgetItem *item2 = new QTableWidgetItem(mapAlarmLevelLogo[alarmData.level_], SingletonAlarmData->getDescByAlarmLevel(alarmData.level_));
    item2->setTextColor(SingletonConfig->getAlarmLevelColor(alarmData.level_));
    item2->setTextAlignment(Qt::AlignCenter);

    QTableWidgetItem *item3 = new QTableWidgetItem(alarmData.text_);
    item3->setTextColor(SingletonConfig->getAlarmLevelColor(alarmData.level_));
    item3->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    QTableWidgetItem *item4 = new QTableWidgetItem(QString::number(id));

    ui->tableWidget_OriginalAlarm->setItem(0,0,item0);
    ui->tableWidget_OriginalAlarm->setItem(0,1,item1);
    ui->tableWidget_OriginalAlarm->setItem(0,2,item2);
    ui->tableWidget_OriginalAlarm->setItem(0,3,item3);
    ui->tableWidget_OriginalAlarm->setItem(0,4,item4);

    //显示第一条
    ui->tableWidget_OriginalAlarm->scrollToTop();
}

void RealTimeAlarm::showCustomMenu(QPoint mousePos)
{
    QTableWidget *tableWidget = static_cast<QTableWidget*>(sender());
    if(tableWidget == ui->tableWidget_SmartAlarm)
    {
        QTableWidgetItem *item = tableWidget->itemAt(QPoint(mousePos.x(),mousePos.y()));
        if(item == NULL)
             return;

        pSmartAlarmMenu->move (cursor().pos ());
        pSmartAlarmMenu->show ();
    }
    else if(tableWidget == ui->tableWidget_OriginalAlarm)
    {
        QTableWidgetItem *item = tableWidget->itemAt(QPoint(mousePos.x(),mousePos.y()));
        if(item == NULL)
             return;

        pOriginalAlarmMenu->move (cursor().pos ());
        pOriginalAlarmMenu->show ();
    }
}

void RealTimeAlarm::confirmAlarm()
{
    if(!SingletonConfig->isCurrentUserhasRight(CONFIRMALARM))
        return;

    QAction *AlarmStatus = dynamic_cast<QAction *>(sender());
    if(AlarmStatus->parent() == ui->tableWidget_SmartAlarm)                    //确认智能告警
    {

        int row = ui->tableWidget_SmartAlarm->currentRow();
        QString id =  ui->tableWidget_SmartAlarm->item(row, 4)->text();        //获取当前行的告警序号

        qDebug()<<QDateTime::currentDateTime();
        if(SingletonDBHelper->modifyIaAlarmStatus(id.toInt()))
        {
              qDebug()<<QDateTime::currentDateTime();
            SingletonAlarmData->modifySmartAlarmStatus(id.toInt());
            ui->tableWidget_SmartAlarm->item(row,0)->setTextColor(Qt::cyan);
            ui->tableWidget_SmartAlarm->item(row,0)->setText(QString::fromLocal8Bit("已确认"));
        }
    }
    else if(AlarmStatus->parent() == ui->tableWidget_OriginalAlarm)            //确认原始告警
    {
        int row = ui->tableWidget_OriginalAlarm->currentRow();
        QString alarmTime = ui->tableWidget_OriginalAlarm->item(row, 1)->text();      //获取当前行的告警时间
        QString id = ui->tableWidget_OriginalAlarm->item(row, 4)->text();             //获取当前行的告警序号
        if(SingletonDBHelper->modifyOaAlarmStatus(alarmTime ,id.toInt()))
        {
            SingletonAlarmData->modifyOriginalAlarmStatus(id.toInt());
            ui->tableWidget_OriginalAlarm->item(row,0)->setTextColor(Qt::cyan);
            ui->tableWidget_OriginalAlarm->item(row,0)->setText(QString::fromLocal8Bit("已确认"));
        }
    }
}

void RealTimeAlarm::showAlarmDetail()
{
    if(!SingletonConfig->isCurrentUserhasRight(ALAEMDETAIL))
        return;

    int row = ui->tableWidget_SmartAlarm->currentRow();        //鼠标左击时，可能当前行被删除了
    if(row >= 0)
    {
        QString id = ui->tableWidget_SmartAlarm->item(row, 4)->text();
        QString fileName = SingletonAlarmData->getSmartAlarmFileNameByID(id.toInt());

        AlarmReportParse alarmReportParse = alarmDetail->showAlarmDetail(fileName);
        bool enable = alarmReportParse.getScene() == QString::fromLocal8Bit("遥控过程分析") ? true : false;
        alarmDetail->showRemoteControlProcess(enable);
    }
}




void RealTimeAlarm::deleteAlarm()
{
    if(!SingletonConfig->isCurrentUserhasRight(DELETEALARM))
        return;

    QAction *action = dynamic_cast<QAction*>(sender());
    if(action->parent() == ui->tableWidget_SmartAlarm)
    {
        int row = ui->tableWidget_SmartAlarm->currentRow();
        QString id = ui->tableWidget_SmartAlarm->item(row, 4)->text();

        if(SingletonAlarmData->removeSmartAlarmByID(id.toInt()))
        {
            ui->tableWidget_SmartAlarm->removeRow(row);
            qDebug()<<QString::fromUtf8("用户:%1 删除智能告警，告警ID:%2").arg(SingletonConfig->getCurUser()).arg(id);
        }
    }
    else if(action->parent() == ui->tableWidget_OriginalAlarm)
    {
        int row = ui->tableWidget_OriginalAlarm->currentRow();
        QString id = ui->tableWidget_OriginalAlarm->item(row, 4)->text();

        if(SingletonAlarmData->removeOriginalAlarmByID(id.toInt()))
        {
            ui->tableWidget_OriginalAlarm->removeRow(row);
            qDebug()<<QString::fromUtf8("用户:%1 删除原始告警，告警ID:%2").arg(SingletonConfig->getCurUser()).arg(id);
        }
    }
}

//暂停智能告警数据显示
void RealTimeAlarm::pauseSmartAlarmData(bool status)
{
    if(status == true)
    {
        ui->pushButton_SmartPause->setText(QString::fromUtf8("恢复"));
        disconnect(SingletonAlarmData, SIGNAL(receiverSmartAlarmData(int,stAlarmData)), this, SLOT(dispSmartAlarmData(int, stAlarmData)));
    }
    else
    {
        ui->pushButton_SmartPause->setText(QString::fromUtf8("暂停"));
        connect(SingletonAlarmData, SIGNAL(receiverSmartAlarmData(int,stAlarmData)), this, SLOT(dispSmartAlarmData(int, stAlarmData)));
        replaySmartAlarmData();
    }
}


//暂停原始告警数据显示
void RealTimeAlarm::pauseOriginalAlarmData(bool status)
{
    if(status == true)
    {
        ui->pushButton_OriginalPause->setText(QString::fromUtf8("恢复"));
        disconnect(SingletonAlarmData, SIGNAL(receiverOriginalAlarmData(int,stAlarmData)), this, SLOT(dispOriginalAlarmData(int, stAlarmData)));
    }
    else
    {
        ui->pushButton_OriginalPause->setText(QString::fromUtf8("暂停"));
        connect(SingletonAlarmData, SIGNAL(receiverOriginalAlarmData(int,stAlarmData)), this, SLOT(dispOriginalAlarmData(int, stAlarmData)));
        replayOriginalAlarmData();
    }
}



void RealTimeAlarm::changeCurIaAlarmLevel()
{
    QPushButton* pushButton = static_cast<QPushButton*>(sender());
    if(pushButton == ui->pushButton_Event_Ia)
    {
        curSelectIaAlarmLevel = LEVEL_EVENT;
    }else if(pushButton == ui->pushButton_Abnormal_Ia)
    {
        curSelectIaAlarmLevel = LEVEL_ABNORMAL;
    }else if(pushButton == ui->pushButton_OutLimit_Ia)
    {
        curSelectIaAlarmLevel = LEVEL_OUTOFLIMIT;
    }else if(pushButton == ui->pushButton_Change_Ia)
    {
        curSelectIaAlarmLevel = LEVEL_CHANGE;
    }else if(pushButton == ui->pushButton_Inform_Ia)
    {
        curSelectIaAlarmLevel = LEVEL_INFORM;
    }else if(pushButton == ui->pushButton_RealTime_Ia)
    {
        curSelectIaAlarmLevel = LEVEL_REALTIME;
    }

    replaySmartAlarmData();
}

void RealTimeAlarm::changeCurOrAlarmLevel()
{
    QPushButton* pushButton = static_cast<QPushButton*>(sender());
    if(pushButton == ui->pushButton_Event_Oa)
    {
        curSelectOrAlarmLevel = LEVEL_EVENT;
    }else if(pushButton == ui->pushButton_Abnormal_Oa)
    {
        curSelectOrAlarmLevel = LEVEL_ABNORMAL;
    }else if(pushButton == ui->pushButton_OutLimit_Oa)
    {
        curSelectOrAlarmLevel = LEVEL_OUTOFLIMIT;
    }else if(pushButton == ui->pushButton_Change_Oa)
    {
        curSelectOrAlarmLevel = LEVEL_CHANGE;
    }else if(pushButton == ui->pushButton_Inform_Oa)
    {
        curSelectOrAlarmLevel = LEVEL_INFORM;
    }else if(pushButton == ui->pushButton_RealTime_Oa)
    {
        curSelectOrAlarmLevel = LEVEL_REALTIME;
    }

    replayOriginalAlarmData();
}


void RealTimeAlarm::selectFilterBay()
{
    QPushButton *button = dynamic_cast<QPushButton*>(sender());
    if(button == ui->pushButton_SmartBayFilter)
    {
        pFilterBay->showSelectBay(0);
    }
    else if(button == ui->pushButton_SmartBayShield)
    {
        pFilterBay->showSelectBay(1);
    }
    else if(button == ui->pushButton_OriginalBayFilter)
    {
        pFilterBay->showSelectBay(2);
    }
    else if(button == ui->pushButton_OriginalBayShield)
    {
        pFilterBay->showSelectBay(3);
    }
}



