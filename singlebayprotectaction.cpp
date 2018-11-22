#include "singlebayprotectaction.h"
#include "ui_singlebayprotectaction.h"

SingleBayProtectAction::SingleBayProtectAction(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SingleBayProtectAction)
{
     qRegisterMetaType<stAlarmReport>("stAlarmReport");
    ui->setupUi(this);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->verticalHeader()->hide();
    ui->tableWidget->setColumnHidden(ui->tableWidget->columnCount() - 1 , true);
    ui->tableWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    connect(ui->tableWidget, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(showAlarmDetail(QModelIndex)));

    connect(SingletonAlarmData, SIGNAL(receiveSingleBayProtectAction(stAlarmReport)), this, SLOT(dispSingleBayProtectAction(stAlarmReport)));
    alarmDetail = new AlarmDetail();
    alarmDetail->showOtherPage(true);
}

SingleBayProtectAction::~SingleBayProtectAction()
{
    delete ui;
}

void SingleBayProtectAction::dispSingleBayProtectAction(stAlarmReport alarmReport)
{
    
   
    
    emit increase(alarmReport.level_);

    ui->tableWidget->insertRow(0);
    QTableWidgetItem *item0 = new QTableWidgetItem(alarmReport.time_);
    item0->setTextAlignment(Qt::AlignCenter);

    QTableWidgetItem *item1 = new QTableWidgetItem(alarmReport.substation_);
    item1->setTextAlignment(Qt::AlignCenter);

    QTableWidgetItem *item2 = new QTableWidgetItem(alarmReport.bay_);
    item2->setTextAlignment(Qt::AlignCenter);

    QTableWidgetItem *item3 = new QTableWidgetItem(SingletonAlarmData->getDescByAlarmLevel(alarmReport.level_));
    item3->setTextAlignment(Qt::AlignCenter);

    QTableWidgetItem *item4 = new QTableWidgetItem(alarmReport.conclusion_);
    item4->setTextAlignment(Qt::AlignCenter);

    QTableWidgetItem *item5 = new QTableWidgetItem(alarmReport.filename_);
    item5->setTextAlignment(Qt::AlignCenter);

    ui->tableWidget->setItem(0, 0, item0);
    ui->tableWidget->setItem(0, 1, item1);
    ui->tableWidget->setItem(0, 2, item2);
    ui->tableWidget->setItem(0, 3, item3);
    ui->tableWidget->setItem(0, 4, item4);
    ui->tableWidget->setItem(0, 5, item5);
}

void SingleBayProtectAction::showAlarmDetail(QModelIndex index)
{
     QString alarmfile = ui->tableWidget->item(index.row(), ui->tableWidget->columnCount() - 1)->text();
    alarmDetail->showAlarmDetail(alarmfile);
}
