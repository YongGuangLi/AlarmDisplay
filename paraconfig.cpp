#include "paraconfig.h"
#include "ui_paraconfig.h"

ParaConfig::ParaConfig(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ParaConfig)
{
    ui->setupUi(this);
    ui->tableWidget->verticalHeader()->hide();

    for(int i = 20; i > 0; --i)
    {
        ui->tableWidget->insertRow(0); //插入一个空行

        QTableWidgetItem *item0 = new QTableWidgetItem(QString::number(i));
        item0->setTextAlignment(Qt::AlignCenter);

        QTableWidgetItem *item1 = new QTableWidgetItem("2");
        item1->setTextAlignment(Qt::AlignCenter);

        ui->tableWidget->setItem(0,0,item0);
        ui->tableWidget->setItem(0,1,item1);
    }
}

ParaConfig::~ParaConfig()
{
    delete ui;
}
