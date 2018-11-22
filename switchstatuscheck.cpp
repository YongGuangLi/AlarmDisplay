#include "switchstatuscheck.h"
#include "ui_switchstatuscheck.h"

SwitchStatusCheck::SwitchStatusCheck(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SwitchStatusCheck)
{
    ui->setupUi(this);

    alarmDetail = new AlarmDetail();
    alarmDetail->showSequenceDiagram(true);

    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->verticalHeader()->hide();
    ui->tableWidget->setColumnHidden(ui->tableWidget->columnCount() - 1 , true);
    ui->tableWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setResizeMode(0, QHeaderView::ResizeToContents);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    connect(ui->tableWidget, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(showAlarmDetail(QModelIndex)));
    connect(SingletonAlarmData, SIGNAL(receiveSwitchStatusCheck(stAlarmReport)), this, SLOT(dispSwitchStatusCheck(stAlarmReport)));
}

SwitchStatusCheck::~SwitchStatusCheck()
{
    delete ui;
}

void SwitchStatusCheck::dispSwitchStatusCheck(stAlarmReport alarmReport)
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

    QTableWidgetItem *item4 = new QTableWidgetItem(alarmReport.conclusion_);
    item4->setTextAlignment(Qt::AlignCenter);

    QTableWidgetItem *item5 = new QTableWidgetItem(alarmReport.listPointValue_.join(";"));
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


void SwitchStatusCheck::showAlarmDetail(QModelIndex index)
{
    QString alarmfile = ui->tableWidget->item(index.row(), ui->tableWidget->columnCount() - 1)->text();
    alarmDetail->showAlarmDetail(alarmfile);
}
