#include "switchactiontimecheck.h"
#include "ui_switchactiontimecheck.h"

SwitchActionTimeCheck::SwitchActionTimeCheck(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SwitchActionTimeCheck)
{
    ui->setupUi(this);
    alarmDetail = new AlarmDetail();
    alarmDetail->showSequenceDiagram(true);

    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->verticalHeader()->hide();
    ui->tableWidget->setColumnHidden(ui->tableWidget->columnCount() - 1 , true);
    ui->tableWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setResizeMode(0, QHeaderView::ResizeToContents);
    ui->tableWidget->horizontalHeader()->setResizeMode(6, QHeaderView::ResizeToContents);
    ui->tableWidget->setColumnHidden(9, true);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    connect(ui->tableWidget, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(showAlarmDetail(QModelIndex)));
    connect(SingletonAlarmData, SIGNAL(receiveSwitchActionTimeCheck(stAlarmReport)), this, SLOT(dispSwitchActionTimeCheck(stAlarmReport)));
}

SwitchActionTimeCheck::~SwitchActionTimeCheck()
{
    delete ui;
}

void SwitchActionTimeCheck::dispSwitchActionTimeCheck(stAlarmReport alarmReport)
{
    emit increase(alarmReport.level_);

    ui->tableWidget->insertRow(0);

    QTableWidgetItem *item0 = new QTableWidgetItem(alarmReport.time_);
    item0->setTextAlignment(Qt::AlignCenter);

    QTableWidgetItem *item1 = new QTableWidgetItem(alarmReport.substation_);
    item1->setTextAlignment(Qt::AlignCenter);

    QTableWidgetItem *item2 = new QTableWidgetItem(alarmReport.bay_);
    item2->setTextAlignment(Qt::AlignCenter);

    int i = alarmReport.listPointType_.indexOf(QString::fromUtf8("遥控"));
    QTableWidgetItem *item3 = new QTableWidgetItem(alarmReport.listPointName_.value(i, ""));
    item3->setTextAlignment(Qt::AlignCenter);

    QTableWidgetItem *item4 = new QTableWidgetItem(alarmReport.listPointValue_.value(i, ""));
    item4->setTextAlignment(Qt::AlignCenter);

    QTableWidgetItem *item5 = new QTableWidgetItem(alarmReport.direction_);
    item5->setTextAlignment(Qt::AlignCenter);

    i = alarmReport.listPointType_.indexOf(QString::fromUtf8("遥信"));

    QTableWidgetItem *item6 = new QTableWidgetItem(alarmReport.listPointTime_.value(i, ""));
    item6->setTextAlignment(Qt::AlignCenter);

    QTableWidgetItem *item7 = new QTableWidgetItem(alarmReport.listPointName_.value(i, ""));
    item7->setTextAlignment(Qt::AlignCenter);

    QTableWidgetItem *item8 = new QTableWidgetItem(alarmReport.listPointValue_.value(i, ""));
    item8->setTextAlignment(Qt::AlignCenter);

    QTableWidgetItem *item9 = new QTableWidgetItem(alarmReport.validTime_);
    item9->setTextAlignment(Qt::AlignCenter);

    QTableWidgetItem *item10 = new QTableWidgetItem(alarmReport.conclusion_);
    item10->setTextAlignment(Qt::AlignCenter);

    QTableWidgetItem *item11 = new QTableWidgetItem(alarmReport.filename_);
    item11->setTextAlignment(Qt::AlignCenter);

    ui->tableWidget->setItem(0, 0, item0);
    ui->tableWidget->setItem(0, 1, item1);
    ui->tableWidget->setItem(0, 2, item2);
    ui->tableWidget->setItem(0, 3, item3);
    ui->tableWidget->setItem(0, 4, item4);
    ui->tableWidget->setItem(0, 5, item5);
    ui->tableWidget->setItem(0, 6, item6);
    ui->tableWidget->setItem(0, 7, item7);
    ui->tableWidget->setItem(0, 8, item8);
    ui->tableWidget->setItem(0, 9, item9);
    ui->tableWidget->setItem(0, 10, item10);
    ui->tableWidget->setItem(0, 11, item11);
}

void SwitchActionTimeCheck::showAlarmDetail(QModelIndex index)
{
    QString alarmfile = ui->tableWidget->item(index.row(), ui->tableWidget->columnCount() - 1)->text();
    alarmDetail->showAlarmDetail(alarmfile);
}
