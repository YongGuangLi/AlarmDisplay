#include "remotecontrolprocess.h"
#include "ui_remotecontrolprocess.h"

RemoteControlProcess::RemoteControlProcess(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RemoteControlProcess)
{
    ui->setupUi(this);
    this->setWindowTitle(QString::fromUtf8("遥控分析"));
    this->setWindowIcon(QIcon(":/images/images/aboutlogo.png"));

    remoteControlWidget = new RemoteControlWidget();
    alarmDetail = new AlarmDetail();
    alarmDetail->showSequenceDiagram(true);
    alarmDetail->addRemoteControlProcess(remoteControlWidget);

    ui->tableWidget->verticalHeader()->hide();
    ui->tableWidget->setColumnHidden(ui->tableWidget->columnCount() - 1 , true);
    ui->tableWidget->horizontalHeader()->show();
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

    connect(ui->tableWidget, SIGNAL(doubleClicked(QModelIndex)) , this ,SLOT(showAlarmDetail(QModelIndex)));

    connect(SingletonAlarmData, SIGNAL(receiveRemoteControl(stAlarmReport)), this , SLOT(dispRemoteControl(stAlarmReport)));

}

RemoteControlProcess::~RemoteControlProcess()
{
    delete ui;
}


void RemoteControlProcess::dispRemoteControl(stAlarmReport alarmReport)
{
    
   
    
    emit increase(alarmReport.level_);

    ui->tableWidget->insertRow(0);

    QTableWidgetItem *item0 = new QTableWidgetItem();
    item0->setTextAlignment(Qt::AlignCenter);
    if(alarmReport.listPointTime_.size() != 0)
        item0->setText(alarmReport.listPointTime_.first());

    QTableWidgetItem *item1 = new QTableWidgetItem();
    item1->setTextAlignment(Qt::AlignCenter);
    if(alarmReport.listPointTime_.size() != 0)
        item1->setText(alarmReport.listPointTime_.last());

    QTableWidgetItem *item2 = new QTableWidgetItem(alarmReport.substation_);
    item2->setTextAlignment(Qt::AlignCenter);

    QTableWidgetItem *item3 = new QTableWidgetItem(alarmReport.bay_);
    item3->setTextAlignment(Qt::AlignCenter);

    QTableWidgetItem *item4 = new QTableWidgetItem();
    item4->setTextAlignment(Qt::AlignCenter);
    if(alarmReport.listPointName_.size() != 0)
        item4->setText(alarmReport.listPointName_.first());

    QTableWidgetItem *item5 = new QTableWidgetItem(alarmReport.conclusion_);
    item5->setTextAlignment(Qt::AlignCenter);

    QTableWidgetItem *item6 = new QTableWidgetItem(alarmReport.direction_);
    item6->setTextAlignment(Qt::AlignCenter);

    QTableWidgetItem *item7 = new QTableWidgetItem(alarmReport.filename_);
    item7->setTextAlignment(Qt::AlignCenter);

    ui->tableWidget->setItem(0, 0, item0);
    ui->tableWidget->setItem(0, 1, item1);
    ui->tableWidget->setItem(0, 2, item2);
    ui->tableWidget->setItem(0, 3, item3);
    ui->tableWidget->setItem(0, 4, item4);
    ui->tableWidget->setItem(0, 5, item5);
    ui->tableWidget->setItem(0, 6, item6);
    ui->tableWidget->setItem(0, 7, item7);
}



void RemoteControlProcess::showAlarmDetail(QModelIndex index)
{
    QString alarmfile = ui->tableWidget->item(index.row(), ui->tableWidget->columnCount() - 1)->text();
    alarmDetail->showAlarmDetail(alarmfile);

    remoteControlWidget->showRemoteControl(alarmfile);
}

