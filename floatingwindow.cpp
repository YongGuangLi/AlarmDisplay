#include "floatingwindow.h"
#include "ui_floatingwindow.h"

FloatingWindow::FloatingWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FloatingWindow)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
    this->installEventFilter(this);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(dispAlarmCount()));
    timer->start(1000);
}

FloatingWindow::~FloatingWindow()
{
    delete ui;
}



bool FloatingWindow::eventFilter(QObject *obj, QEvent *evt)
{
    QWidget *w = (QWidget *)obj;

    static QPoint mousePoint;
    static bool mousePressed = false;

    QMouseEvent *event = static_cast<QMouseEvent *>(evt);
    if (event->type() == QEvent::MouseButtonPress) {
        if (event->button() == Qt::LeftButton) {
            mousePressed = true;
            mousePoint = event->globalPos() - w->pos();
            return true;
        }
    } else if (event->type() == QEvent::MouseButtonRelease) {
        mousePressed = false;
        return true;
    } else if (event->type() == QEvent::MouseMove) {
        if (mousePressed && (event->buttons() && Qt::LeftButton)) {
            w->move(event->globalPos() - mousePoint);
            return true;
        }
    }

    return QObject::eventFilter(obj, evt);
}

void FloatingWindow::dispAlarmCount()
{
    int evevtCnt = 0;
    int abnormolCnt = 0;
    int outoflimitCnt = 0;
    int changeCnt = 0;
    int informCnt = 0;
    QMap<int, stAlarmData> mapSmartAlarmData = SingletonAlarmData->getSmartAlarmData();

    QMapIterator<int, stAlarmData> itSmartAlarmData(mapSmartAlarmData);
    while(itSmartAlarmData.hasNext())
    {
        itSmartAlarmData.next();
        stAlarmData alarmData = itSmartAlarmData.value();
        if(alarmData.confirmed_ == 1)
            continue;

        switch(alarmData.level_)
        {
        case LEVEL_EVENT: evevtCnt++;
        break;
        case LEVEL_ABNORMAL: abnormolCnt++;
        break;
        case  LEVEL_OUTOFLIMIT: outoflimitCnt++;
        break;
        case  LEVEL_CHANGE: changeCnt++;
        break;
        case  LEVEL_INFORM: informCnt++;
        break;
        default:
        break;
        }
    }

    ui->label_evevt->setStyleSheet(QString("QLabel#label_evevt{color:%1}").arg(SingletonConfig->getAlarmLevelColor(LEVEL_EVENT).name()));
    ui->label_evevt->setText(QString::fromLocal8Bit("事件: %1").arg(evevtCnt));

    ui->label_abnormol->setStyleSheet(QString("QLabel#label_abnormol{color:%1}").arg(SingletonConfig->getAlarmLevelColor(LEVEL_ABNORMAL).name()));
    ui->label_abnormol->setText(QString::fromLocal8Bit("异常: %1").arg(abnormolCnt));

    ui->label_outoflimit->setStyleSheet(QString("QLabel#label_outoflimit{color:%1}").arg(SingletonConfig->getAlarmLevelColor(LEVEL_OUTOFLIMIT).name()));
    ui->label_outoflimit->setText(QString::fromLocal8Bit("越限: %1").arg(outoflimitCnt));

    ui->label_change->setStyleSheet(QString("QLabel#label_change{color:%1}").arg(SingletonConfig->getAlarmLevelColor(LEVEL_CHANGE).name()));
    ui->label_change->setText(QString::fromLocal8Bit("变位: %1").arg(changeCnt));

    ui->label_inform->setStyleSheet(QString("QLabel#label_inform{color:%1}").arg(SingletonConfig->getAlarmLevelColor(LEVEL_INFORM).name()));
    ui->label_inform->setText(QString::fromLocal8Bit("告知: %1").arg(informCnt));
}

