#include "aboutdialog.h"
#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
    this->setWindowTitle(QString::fromLocal8Bit("变电站全景运行信息综合分析系统"));
    this->setWindowIcon(QIcon(":/images/images/aboutlogo.png"));
    connect(ui->pushButton_confirm, SIGNAL(clicked()), this, SLOT(accept()));


  ui->label_AboutInfo->setText(QString::fromLocal8Bit("<b>变电站全景运行信息综合分析系统</b>&nbsp;&nbsp;V1.0&nbsp;&nbsp;&nbsp;&nbsp;<br/><br/>"
                               "Copyright 2019<br/><br/>"
                               "<b>湖南大唐先一科技有限公司</b>"));


}

AboutDialog::~AboutDialog()
{
    delete ui;
}
