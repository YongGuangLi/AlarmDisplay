#include "paraconfig.h"
#include "ui_paraconfig.h"

ParaConfig::ParaConfig(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ParaConfig)
{
    ui->setupUi(this);
    this->setWindowTitle(QString::fromLocal8Bit("参数配置"));
    ui->tableWidget->verticalHeader()->hide();

    listLineEdit.push_back(ui->lineEdit);
    listLineEdit.push_back(ui->lineEdit_2);
    listLineEdit.push_back(ui->lineEdit_3);
    listLineEdit.push_back(ui->lineEdit_4);
    listLineEdit.push_back(ui->lineEdit_5);
    listLineEdit.push_back(ui->lineEdit_6);
    listLineEdit.push_back(ui->lineEdit_7);
    listLineEdit.push_back(ui->lineEdit_8);
    listLineEdit.push_back(ui->lineEdit_9);
    listLineEdit.push_back(ui->lineEdit_10);
    listLineEdit.push_back(ui->lineEdit_11);
    listLineEdit.push_back(ui->lineEdit_12);
    listLineEdit.push_back(ui->lineEdit_13);
    listLineEdit.push_back(ui->lineEdit_14);
    listLineEdit.push_back(ui->lineEdit_15);
    listLineEdit.push_back(ui->lineEdit_16);
    listLineEdit.push_back(ui->lineEdit_17);
    listLineEdit.push_back(ui->lineEdit_18);
    listLineEdit.push_back(ui->lineEdit_19);
    listLineEdit.push_back(ui->lineEdit_20);
    listLineEdit.push_back(ui->lineEdit_21);
    listLineEdit.push_back(ui->lineEdit_22);
    listLineEdit.push_back(ui->lineEdit_23);
    listLineEdit.push_back(ui->lineEdit_24);

    init();
}

ParaConfig::~ParaConfig()
{
    delete ui;
}

void ParaConfig::init()
{
    ui->tableWidget->setRowCount(0);

    string whiteListValue;
    SingleRedisHelp->get("WhiteList", whiteListValue);

    QStringList whitelist = QString::fromStdString(whiteListValue).split(",",QString::SkipEmptyParts);
    for(int i = 0; i < whitelist.size(); ++i)
    {
        listLineEdit.value(i)->setText(whitelist.at(i));
    }

    string OnlineIpValue;
    SingleRedisHelp->get("OnlineIp", OnlineIpValue);
    QStringList OnlineIp =  QString::fromStdString(OnlineIpValue).split(",");

    for(int i = whitelist.size() - 1; i >= 0; --i)
    {
        ui->tableWidget->insertRow(0); //插入一个空行

        QTableWidgetItem *item0 = new QTableWidgetItem(whitelist.at(i));
        item0->setTextAlignment(Qt::AlignCenter);

        qDebug()<<OnlineIp.contains(whitelist.at(i));
        QString connStatus = OnlineIp.contains(whitelist.at(i)) ? QString::fromUtf8("连接") : QString::fromUtf8("断开");
        QTableWidgetItem *item1 = new QTableWidgetItem(connStatus);
        item1->setTextAlignment(Qt::AlignCenter);

        ui->tableWidget->setItem(0,0,item0);
        ui->tableWidget->setItem(0,1,item1);
    }
}

void ParaConfig::on_pushButton_save_clicked()
{
    QStringList whitelist;
    for(int i = 0; i < listLineEdit.size(); ++i)
    {
        QString ip = listLineEdit.at(i)->text();
        whitelist.push_back(ip);
    }

    SingleRedisHelp->set("WhiteList", whitelist.join(",").toStdString());

    init();
}

void ParaConfig::slot_modifyConnStatus(QString ip, int status)
{
    QList<QTableWidgetItem *> listItem = ui->tableWidget->findItems(ip, Qt::MatchExactly);
    for(int i = 0; i < listItem.size(); ++i)
    {
        QString connStatus = status == 1 ? QString::fromUtf8("连接") : QString::fromUtf8("断开");
        ui->tableWidget->item(listItem.at(i)->row(), listItem.at(i)->column() + 1)->setText(connStatus);
    }
}
