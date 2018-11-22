#include "alarmdispconfig.h"
#include "ui_alarmdispconfig.h"

AlarmDispConfig::AlarmDispConfig(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AlarmDispConfig)
{
    ui->setupUi(this);
    this->setWindowTitle(QString::fromLocal8Bit("告警展示配置"));
    this->setWindowIcon(QIcon(":/images/images/config.png"));
    this->setWindowFlags(Qt::Dialog);

    ui->pushButton_confirm->setIcon(QIcon(":/images/images/confirm.png"));
    ui->pushButton_cancel->setIcon(QIcon(":/images/images/cancel.png"));

    centerDisplay();
    initButtonConnections();
    initCheckConnections();

    ui->pushButton_cancel->hide();
}

AlarmDispConfig::~AlarmDispConfig()
{
    delete ui;
}

void AlarmDispConfig::centerDisplay()
{
    QDesktopWidget *screenResolution = QApplication::desktop();
    int appWindowWidth = this->geometry().width();
    int appWindowHeight = this->geometry().height();
    int center_y = screenResolution->height()/2 - appWindowHeight/2;
    int center_x = screenResolution->width()/2 - appWindowWidth/2;
    setGeometry(center_x, center_y, appWindowWidth,appWindowHeight);
}

void AlarmDispConfig::initButtonConnections()
{
    buttonGroup.addButton(ui->pushButton_event, LEVEL_EVENT);            //按键id和告警等级对应
    buttonGroup.addButton(ui->pushButton_abnormal, LEVEL_ABNORMAL);
    buttonGroup.addButton(ui->pushButton_outLimit, LEVEL_OUTOFLIMIT);
    buttonGroup.addButton(ui->pushButton_change, LEVEL_CHANGE);
    buttonGroup.addButton(ui->pushButton_inform, LEVEL_INFORM);

    QList<QAbstractButton *> buttons = buttonGroup.buttons();
    for(int i = 0; i < buttons.size(); ++i)
    {
        QAbstractButton *button = buttons.at(i);
        button->setFocusPolicy(Qt::NoFocus);                           //去掉按下的焦点虚线框
        int buttonID = buttonGroup.id(button);                         //按键id和告警等级对应
        QColor color = SingletonConfig->getAlarmLevelColor(buttonID);
        button->setStyleSheet(QString("background-color:%1;").arg(color.name()));

        connect(button, SIGNAL(clicked()), this, SLOT(changeAlarmLevelColor()));
    }
}

void AlarmDispConfig::initCheckConnections()
{
    checkGroup.setExclusive(false);                                   //设置不互斥
    checkGroup.addButton(ui->checkBox_event, LEVEL_EVENT);            //按键id和告警等级对应
    checkGroup.addButton(ui->checkBox_abnormal, LEVEL_ABNORMAL);
    checkGroup.addButton(ui->checkBox_outLimit, LEVEL_OUTOFLIMIT);
    checkGroup.addButton(ui->checkBox_change, LEVEL_CHANGE);
    checkGroup.addButton(ui->checkBox_inform, LEVEL_INFORM);

    QList<QAbstractButton *> checkBoxs = checkGroup.buttons();
    for(int i = 0; i < checkBoxs.size(); ++i)
    {
        QCheckBox *checkBox = (QCheckBox*)checkBoxs.at(i);
        int checkBoxID = checkGroup.id(checkBox);                         //按键id和告警等级对应
        int flag = SingletonConfig->getAlarmLevelFlag(checkBoxID);
        Qt::CheckState checkState = flag == 1? Qt::Checked : Qt::Unchecked;
        checkBox->setCheckState(checkState);
    }
}



void AlarmDispConfig::showEvent(QShowEvent *)
{
    QList<QAbstractButton *> buttons = buttonGroup.buttons();
    for(int i = 0; i < buttons.size(); ++i)
    {
        QAbstractButton *button = buttons.at(i);
        int buttonID = buttonGroup.id(button);                                      //按键id和告警等级对应
        QColor color = SingletonConfig->getAlarmLevelColor(buttonID);

        button->setStyleSheet(QString("background-color:%1;").arg(color.name()));   //设置按键背景色
    }

    QList<QAbstractButton *> checkBoxs = checkGroup.buttons();
    for(int i = 0; i < checkBoxs.size(); ++i)
    {
        QCheckBox *checkBox = (QCheckBox*)checkBoxs.at(i);
        int checkBoxID = checkGroup.id(checkBox);                                 //按键id和告警等级对应
        int flag = SingletonConfig->getAlarmLevelFlag(checkBoxID);
        Qt::CheckState checkState = flag == 1? Qt::Checked : Qt::Unchecked;
        checkBox->setCheckState(checkState);                                      //设置弹出标志
    }
}


void AlarmDispConfig::changeAlarmLevelColor()
{
    QPushButton *button = static_cast<QPushButton*>(sender());
    QColor color = QColorDialog::getColor(Qt::black);
    if (color.isValid())
    {
        button->setStyleSheet(QString("background-color:%1;").arg(color.name()));
    }
}


void AlarmDispConfig::on_pushButton_confirm_clicked()
{
    QList<QAbstractButton *> buttons = buttonGroup.buttons();
    for(int i = 0; i < buttons.size(); ++i)
    {
        QAbstractButton *button = buttons.at(i);
        int buttonID = buttonGroup.id(button);
        QString styleSheet = button->styleSheet();                                   //"background-color:#55ff7f;"
        QString colorName = styleSheet.mid(17, 7);

        SingletonConfig->setAlarmLevelColor(buttonID, QColor(colorName));           //按键id和告警等级对应, 确认设置，保存背景色
    }

    QList<QAbstractButton *> checkBoxs = checkGroup.buttons();
    for(int i = 0; i < checkBoxs.size(); ++i)
    {
        QCheckBox *checkBox = (QCheckBox*)checkBoxs.at(i);
        int checkBoxID = checkGroup.id(checkBox);
        Qt::CheckState checkState = checkBox->checkState();
        int flag = checkState == Qt::Checked ? 1 : 0;

        SingletonConfig->setAlarmLevelFlag( checkBoxID,flag);                       //按键id和告警等级对应
    }

    emit alarmLevelColorChange();                                                   //发射告警颜色改变的信号

    this->hide();
}





