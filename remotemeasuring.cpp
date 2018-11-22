#include "remotemeasuring.h"
#include "ui_remotemeasuring.h"

RemoteMeasuring::RemoteMeasuring(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RemoteMeasuring)
{
    ui->setupUi(this);
    alarmDetail = new AlarmDetail();
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->verticalHeader()->hide();
    ui->tableWidget->setColumnHidden(ui->tableWidget->columnCount() - 1 , true);
    ui->tableWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    connect(ui->tableWidget, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(showAlarmDetail(QModelIndex)));

    connect(SingletonAlarmData, SIGNAL(receiveRemoteMeasuring(stAlarmReport)), this, SLOT(dispRemoteMeasuring(stAlarmReport)));

}

RemoteMeasuring::~RemoteMeasuring()
{
    delete ui;
}


void RemoteMeasuring::dispRemoteMeasuring(stAlarmReport alarmReport)
{
    emit increase(alarmReport.level_);

    ui->tableWidget->insertRow(0);

    QTableWidgetItem *item0 = new QTableWidgetItem(alarmReport.time_);
    item0->setTextAlignment(Qt::AlignCenter);

    QTableWidgetItem *item1 = new QTableWidgetItem(alarmReport.substation_);
    item1->setTextAlignment(Qt::AlignCenter);

    QTableWidgetItem *item2 = new QTableWidgetItem(alarmReport.bay_);
    item2->setTextAlignment(Qt::AlignCenter);

    QTableWidgetItem *item3 = new QTableWidgetItem(alarmReport.listPointName_.value(0,""));
    item3->setTextAlignment(Qt::AlignCenter);

    QString prefix;
    if(alarmReport.listPointName_.value(0,"").contains(QString::fromUtf8("电流")))
        prefix = "I=";
    else if(alarmReport.listPointName_.value(0,"").contains(QString::fromUtf8("电压")))
        prefix = "U=";
    QTableWidgetItem *item4 = new QTableWidgetItem(prefix + alarmReport.listPointValue_.value(0, ""));
    item4->setTextAlignment(Qt::AlignCenter);

    QTableWidgetItem *item5 = new QTableWidgetItem(prefix + alarmReport.listPointValue_.value(1, ""));
    item5->setTextAlignment(Qt::AlignCenter);

    double changeValue = qAbs(alarmReport.listPointValue_.value(1, "").toDouble() - alarmReport.listPointValue_.value(0, "").toDouble());
    QTableWidgetItem *item6 = new QTableWidgetItem(QString::number(changeValue / alarmReport.listPointValue_.value(0, "").toDouble()));
    item6->setTextAlignment(Qt::AlignCenter);

    QTableWidgetItem *item7 = new QTableWidgetItem(alarmReport.validTime_);
    item7->setTextAlignment(Qt::AlignCenter);

    QTableWidgetItem *item8 = new QTableWidgetItem(alarmReport.filename_);
    item8->setTextAlignment(Qt::AlignCenter);


    ui->tableWidget->setItem(0, 0, item0);
    ui->tableWidget->setItem(0, 1, item1);
    ui->tableWidget->setItem(0, 2, item2);
    ui->tableWidget->setItem(0, 3, item3);
    ui->tableWidget->setItem(0, 4, item4);
    ui->tableWidget->setItem(0, 5, item5);
    ui->tableWidget->setItem(0, 6, item6);
    ui->tableWidget->setItem(0, 7, item7);
    ui->tableWidget->setItem(0, 8, item8);
}

void RemoteMeasuring::showAlarmDetail(QModelIndex index)
{
    QString alarmfile = ui->tableWidget->item(index.row(), ui->tableWidget->columnCount() - 1)->text();
    alarmDetail->showAlarmDetail(alarmfile);
}

