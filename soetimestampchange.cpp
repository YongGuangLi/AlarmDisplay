#include "soetimestampchange.h"
#include "ui_soetimestampchange.h"

SoeTimestampChange::SoeTimestampChange(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SoeTimestampChange)
{
    ui->setupUi(this);

    alarmDetail = new AlarmDetail();
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->verticalHeader()->hide();
    ui->tableWidget->setColumnHidden(ui->tableWidget->columnCount() - 1 , true);
    ui->tableWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    connect(ui->tableWidget, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(showAlarmDetail(QModelIndex)));

    connect(SingletonAlarmData, SIGNAL(receiveSoeTimestampChange(stAlarmReport)), this, SLOT(dispSoeTimestampChange(stAlarmReport)));
}

SoeTimestampChange::~SoeTimestampChange()
{
    delete ui;
}

void SoeTimestampChange::dispSoeTimestampChange(stAlarmReport alarmReport)
{
    
   
    
    emit increase(alarmReport.level_);

    ui->tableWidget->insertRow(0);

    QString startTime = alarmReport.time_;
    if(alarmReport.listPointTime_.size() != 0)
        startTime = alarmReport.listPointTime_.first();

    QTableWidgetItem *item0 = new QTableWidgetItem(startTime);
    item0->setTextAlignment(Qt::AlignCenter);

    QTableWidgetItem *item1 = new QTableWidgetItem(alarmReport.time_);
    item1->setTextAlignment(Qt::AlignCenter);

    QTableWidgetItem *item2 = new QTableWidgetItem(alarmReport.substation_);
    item2->setTextAlignment(Qt::AlignCenter);

    QTableWidgetItem *item3 = new QTableWidgetItem(alarmReport.bay_);
    item3->setTextAlignment(Qt::AlignCenter);

    QTableWidgetItem *item4 = new QTableWidgetItem(alarmReport.listPointName_.join(";"));
    item4->setTextAlignment(Qt::AlignCenter);

    QTableWidgetItem *item5 = new QTableWidgetItem(QString::fromLocal8Bit("跳变"));
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

void SoeTimestampChange::showAlarmDetail(QModelIndex index)
{
    QString alarmfile = ui->tableWidget->item(index.row(), ui->tableWidget->columnCount() - 1)->text();
    alarmDetail->showAlarmDetail(alarmfile);
}

