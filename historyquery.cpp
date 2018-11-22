#include "historyquery.h"
#include "ui_historyquery.h"

HistoryQuery::HistoryQuery(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HistoryQuery)
{
    ui->setupUi(this);

    ui->dateTimeEdit_begin->setDisplayFormat("yyyy-MM-dd hh:mm:ss");
    ui->dateTimeEdit_begin->setCalendarPopup(true);

    ui->dateTimeEdit_end->setDisplayFormat("yyyy-MM-dd hh:mm:ss");
    ui->dateTimeEdit_end->setCalendarPopup(true);

    ui->dateTimeEdit_begin->setDate(QDateTime::currentDateTime().date());
    ui->dateTimeEdit_end->setDateTime(QDateTime::currentDateTime());


    ui->pushButton_query->setIcon(QIcon(":images/images/HistoryQuery/query.png"));
    ui->pushButton_export->setIcon(QIcon(":images/images/HistoryQuery/export.png"));

    //告警显示表格设置
    ui->tableWidget_AlarmDisp->setWindowTitle(QString::fromUtf8("智能告警"));
    ui->tableWidget_AlarmDisp->setEditTriggers(QAbstractItemView::NoEditTriggers);    //设置内容不可更改
    ui->tableWidget_AlarmDisp->setContextMenuPolicy(Qt::CustomContextMenu);           //允许右击菜单
    ui->tableWidget_AlarmDisp->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget_AlarmDisp->horizontalHeader()->setResizeMode(1, QHeaderView::ResizeToContents);
    ui->tableWidget_AlarmDisp->setColumnHidden(4, true);
    ui->tableWidget_AlarmDisp->setColumnHidden(5, true);
    ui->tableWidget_AlarmDisp->setSelectionBehavior(QAbstractItemView::SelectRows);   //单击选择一行
    ui->tableWidget_AlarmDisp->installEventFilter(this);
    ui->tableWidget_AlarmDisp->setShowGrid(true);
    ui->tableWidget_AlarmDisp->verticalHeader()->hide();
    connect(ui->tableWidget_AlarmDisp, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(showAlarmDetail(QModelIndex)));

    ui->comboBox_bayType->installEventFilter(this);
    connect(ui->pushButton_export, SIGNAL(clicked()), this, SLOT(exportHistoryData()));

    //遥控过程
    remoteControlWidget = new RemoteControlWidget();
    //告警详情
    alarmDetail = new AlarmDetail();
    alarmDetail->addRemoteControlProcess(remoteControlWidget);

    //查询历史告警
    connect(SingletonDBHelper, SIGNAL(sendAlarmData(int, stAlarmData)), this, SLOT(dispSmartAlarmData(int,stAlarmData)));
}

HistoryQuery::~HistoryQuery()
{
    delete ui;
}

bool HistoryQuery::eventFilter(QObject *obj, QEvent *eve)
{
    if(obj == ui->comboBox_bayType && eve->type() == QEvent::MouseButtonPress)
    {
        ui->comboBox_bayType->clear();
        ui->comboBox_bayType->addItem(QString::fromUtf8("所有间隔"));
        ui->comboBox_bayType->addItems(SingletonConfig->getListBay());
    }
    return QObject::eventFilter(obj, eve);
}

bool HistoryQuery::judgeBayValid(QString text)
{
    QString bayName;
    if(text.split(".").size() <= 2 )               //数据错误，间隔获取失败
        return false;
    //智能告警text_=茂名电网.广场变/10kV.F07馈线 线路发生瞬时性故障，重合成功。
    if(ui->comboBox_AlarmType->currentText() == QString::fromUtf8("智能告警"))
    {
        QString tmp = text.split(".").value(2,"");
        bayName = tmp.left(tmp.indexOf(" "));
    }
    //智能告警text_=茂名电网.广场变/110kV.#2主变(B套).#2主变保护测控二/变高智能终端二检修状态   动作
    else if(ui->comboBox_AlarmType->currentText() == QString::fromUtf8("原始告警"))
    {
        bayName = text.split(".").value(2,"");
    }

    if(ui->comboBox_bayType->currentText() != QString::fromUtf8("所有间隔") && ui->comboBox_bayType->currentText() != bayName)
        return false;
    return true;
}

void HistoryQuery::on_pushButton_query_clicked()
{
    ui->tableWidget_AlarmDisp->setRowCount(0);          //每次查询清空告警表
    SingletonDBHelper->queryHistoryData(ui->dateTimeEdit_begin->text(), ui->dateTimeEdit_end->text(), ui->comboBox_AlarmLevel->currentIndex(), ui->comboBox_AlarmType->currentIndex());
}


/*************************************
Description:  显示智能告警数据
Input:
Output:
Return:
Other:
 **************************************/
void HistoryQuery::dispSmartAlarmData(int id, stAlarmData alarmData)
{
    if(ui->comboBox_AlarmLevel->currentIndex() != LEVEL_REALTIME  && ui->comboBox_AlarmLevel->currentIndex() != alarmData.level_)                 //显示所选告警等级
        return;

    QString status = alarmData.confirmed_ == 1 ? QString::fromUtf8("已确认") : QString::fromUtf8("未确认");
    QColor statusColor =  alarmData.confirmed_ == 1 ? Qt::cyan : Qt::red;

    if(ui->comboBox_alarmStatus->currentText() != QString::fromUtf8("全部告警") &&  ui->comboBox_alarmStatus->currentText() != status)
        return;

    if(!judgeBayValid(alarmData.text_))
        return;

    //超过最大设置行数，需要删除
    ui->tableWidget_AlarmDisp->insertRow(0); //插入一个空行

    QTableWidgetItem *item0 = new QTableWidgetItem(status);
    item0->setTextColor(statusColor);
    item0->setTextAlignment(Qt::AlignCenter);

    QTableWidgetItem *item1 = new QTableWidgetItem(alarmData.time_);
    item1->setTextColor(SingletonConfig->getAlarmLevelColor(alarmData.level_));
    item1->setTextAlignment(Qt::AlignCenter);

    QTableWidgetItem *item2 = new QTableWidgetItem(SingletonAlarmData->getDescByAlarmLevel(alarmData.level_));
    item2->setTextColor(SingletonConfig->getAlarmLevelColor(alarmData.level_));
    item2->setTextAlignment(Qt::AlignCenter);

    QTableWidgetItem *item3 = new QTableWidgetItem(alarmData.text_);
    item3->setTextColor(SingletonConfig->getAlarmLevelColor(alarmData.level_));
    item3->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    QTableWidgetItem *item4 = new QTableWidgetItem(QString::number(id));

    QTableWidgetItem *item5 = new QTableWidgetItem(alarmData.filename_);

    ui->tableWidget_AlarmDisp->setItem(0, 0, item0);
    ui->tableWidget_AlarmDisp->setItem(0, 1, item1);
    ui->tableWidget_AlarmDisp->setItem(0, 2, item2);
    ui->tableWidget_AlarmDisp->setItem(0, 3, item3);
    ui->tableWidget_AlarmDisp->setItem(0, 4, item4);
    ui->tableWidget_AlarmDisp->setItem(0, 5, item5);
}

QByteArray Unicode2GBK(QString inStr)
{
    QTextCodec* gbk = QTextCodec::codecForName("gbk");
    return gbk->fromUnicode(inStr);
}


void HistoryQuery::exportHistoryData()
{
    QString fileName = QFileDialog::getSaveFileName(NULL, tr("Save File"),  "/home/" + QDateTime::currentDateTime().toString("yyyyMMddhhmmss") + ".csv", tr("Images (*.csv)"));

    QFile file(fileName);
    file.open(QIODevice::WriteOnly);

    for(int i = 0; i < ui->tableWidget_AlarmDisp->rowCount(); ++i)
    {
        QStringList listColumnDate;
        for(int j = 0; j < ui->tableWidget_AlarmDisp->columnCount(); ++j)
        {
            QTableWidgetItem *item = ui->tableWidget_AlarmDisp->item(i , j);
            QString text = item->text();
            if(j == 1)
            {
                text.replace(11,1, "T");
            }
            listColumnDate.push_back(text);
        }

        file.write(Unicode2GBK(listColumnDate.join(",")) + "\n");
    }
    file.close();
}

void HistoryQuery::showAlarmDetail(QModelIndex index)
{
    QString alarmfile = ui->tableWidget_AlarmDisp->item(index.row(), ui->tableWidget_AlarmDisp->columnCount() - 1)->text();
    AlarmReportParse alarmReportParse = alarmDetail->showAlarmDetail(alarmfile);
    bool enable = alarmReportParse.getScene() == QString::fromLocal8Bit("遥控过程分析") ? true : false;
    alarmDetail->showRemoteControlProcess(enable);

    enable = alarmReportParse.getScene() == QString::fromLocal8Bit("保护动作") ? true : false;
    alarmDetail->showOtherPage(true);
}
