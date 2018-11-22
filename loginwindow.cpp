#include "loginwindow.h"
#include "ui_loginwindow.h"

LoginWindow::LoginWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
    centerDisplay();
    this->setWindowTitle(QString::fromUtf8("用户登录"));
    this->setWindowFlags(Qt::Dialog);
    this->setFixedSize(this->width(), this->height());

    ui->lineEdit_UserName->setPlaceholderText (QString::fromUtf8("请输入用户名"));
    ui->lineEdit_PassWord->setPlaceholderText (QString::fromUtf8("请输入密码"));
    ui->lineEdit_PassWord->setEchoMode(QLineEdit::Password);
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::centerDisplay()
{
    QDesktopWidget *screenResolution = QApplication::desktop();
    int appWindowWidth = this->geometry().width();
    int appWindowHeight = this->geometry().height();
    int center_y = screenResolution->height()/2 - appWindowHeight/2;
    int center_x = screenResolution->width()/2 - appWindowWidth/2;
    setGeometry(center_x, center_y, appWindowWidth,appWindowHeight);
}

void LoginWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Return)
    {
        ui->pushButton_Login->click();
    }
}

void LoginWindow::on_pushButton_Login_clicked()
{
    if(SingletonConfig->IsUserExits(ui->lineEdit_UserName->text()))
    {
        if(SingletonConfig->CheckUserInfo(ui->lineEdit_UserName->text(),ui->lineEdit_PassWord->text()))
        {
            SingletonConfig->setCurUser(ui->lineEdit_UserName->text());
            emit userLogin(ui->lineEdit_UserName->text());
            ui->lineEdit_PassWord->clear();
            ui->label_remind->clear();
            qDebug()<<QString::fromLocal8Bit("用户登录:").append(SingletonConfig->getCurUser());
            this->hide();
        }
        else
        {
            ui->label_remind->setText(QString::fromLocal8Bit("密码错误"));
        }
    }
    else
    {
        ui->label_remind->setText(QString::fromLocal8Bit("用户名不存在"));
    }
}
