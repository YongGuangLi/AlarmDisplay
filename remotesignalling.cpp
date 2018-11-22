#include "remotesignalling.h"
#include "ui_remotesignalling.h"

RemoteSignalling::RemoteSignalling(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RemoteSignalling)
{
    ui->setupUi(this);
    alarmDetail = new AlarmDetail();
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->verticalHeader()->hide();
    ui->tableWidget->setColumnHidden(ui->tableWidget->columnCount() - 1 , true);
    ui->tableWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    connect(ui->tableWidget, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(showAlarmDetail(QModelIndex)));
    connect(SingletonAlarmData, SIGNAL(receiveRemoteSignalling(stAlarmReport)), this, SLOT(dispRemoteSignalling(stAlarmReport)));
}

RemoteSignalling::~RemoteSignalling()
{
    delete ui;
}

void RemoteSignalling::dispRemoteSignalling(stAlarmReport alarmReport)
{
    emit increase(alarmReport.level_);

    ui->tableWidget->insertRow(0);

    QTableWidgetItem *item0 = new QTableWidgetItem(alarmReport.time_);
    item0->setTextAlignment(Qt::AlignCenter);

    QTableWidgetItem *item1 = new QTableWidgetItem(alarmReport.substation_);
    item1->setTextAlignment(Qt::AlignCenter);

    QTableWidgetItem *item2 = new QTableWidgetItem(alarmReport.bay_);
    item2->setTextAlignment(Qt::AlignCenter);

    QTableWidgetItem *item3 = new QTableWidgetItem(alarmReport.listPointName_.join(";"));
    item3->setTextAlignment(Qt::AlignCenter);

    QString columnData;
    for(int i = 0; i < alarmReport.listPointValue_.size(); ++i)
    {
        //最后一个点的来源为智能远动，其他都用间隔名
        if(i == alarmReport.listPointValue_.size() - 1)
            columnData += QString::fromLocal8Bit("智能远动:%1").arg(alarmReport.listPointValue_.at(i));
        else
            columnData += QString("%1:%2;").arg(alarmReport.bay_).arg(alarmReport.listPointValue_.at(i));
    }
    QTableWidgetItem *item4 = new QTableWidgetItem(columnData);
    item4->setTextAlignment(Qt::AlignCenter);
    QTableWidgetItem *item5 = new QTableWidgetItem(QString::fromUtf8("不一致"));
    item5->setTextAlignment(Qt::AlignCenter);
    QTableWidgetItem *item6 = new QTableWidgetItem(alarmReport.filename_);
    item6->setTextAlignment(Qt::AlignCenter);

    ui->tableWidget->setItem(0, 0, item0);
    ui->tableWidget->setItem(0, 1, item1);
    ui->tableWidget->setItem(0, 2, item2);
    ui->tableWidget->setItem(0, 3, item3);
    ui->tableWidget->setItem(0, 4, item4);
    ui->tableWidget->setItem(0, 5, item5);
    ui->tableWidget->setItem(0, 6, item6);
}

void RemoteSignalling::showAlarmDetail(QModelIndex index)
{
    QString alarmfile = ui->tableWidget->item(index.row(), ui->tableWidget->columnCount() - 1)->text();
    alarmDetail->showAlarmDetail(alarmfile);
}

