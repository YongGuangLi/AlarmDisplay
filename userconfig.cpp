#include "userconfig.h"
#include "ui_userconfig.h"

UserConfig::UserConfig(QDialog *parent) :
    QDialog(parent),
    ui(new Ui::UserConfig)
{
    ui->setupUi(this);
    this->setWindowTitle(QString::fromUtf8("用户管理"));
    currentUserGroupId = -1;

    ui->rightGridLayout->setAlignment(Qt::AlignTop);

    ui->userTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->userTableWidget->verticalHeader()->setHidden(true);
    ui->userTableWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    ui->userTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->userTableWidget->setSelectionMode(QAbstractItemView::SingleSelection);

    connect(ui->addUserButton, SIGNAL(clicked()), this, SLOT(addUserClick()));
    connect(ui->deleteUserButton, SIGNAL(clicked()), this, SLOT(deleteUserClick()));
    connect(ui->modifyUserButton, SIGNAL(clicked()), this, SLOT(modifyUserClick()));

    connect(ui->addGroupButton, SIGNAL(clicked()), this, SLOT(addGroupButtonClick()));
    connect(ui->deleteGroupButton, SIGNAL(clicked()), this, SLOT(deleteGroupButtonClick()));
    initUserTableWidget();

    initGroupTableWidget();

    initUserManageDialog();
}


UserConfig::~UserConfig()
{
    delete ui;
}


void UserConfig::initUserTableWidget()
{
    ui->userTableWidget->setRowCount(0);

    QMapIterator<QString, stUserInfo*> itUserItem(SingletonConfig->getUserInfo());
    while(itUserItem.hasNext())
    {
        itUserItem.next();
        int rowCount = ui->userTableWidget->rowCount();
        ui->userTableWidget->insertRow(rowCount);

        stUserInfo *userInfo = itUserItem.value();
        QTableWidgetItem *userNameItem = new QTableWidgetItem(userInfo->userName);
        QTableWidgetItem *userPassWdItem = new QTableWidgetItem("*********");
        QTableWidgetItem *groupIDItem = new QTableWidgetItem(userInfo->groupName);

        ui->userTableWidget->setItem(rowCount, 0, userNameItem);
        ui->userTableWidget->setItem(rowCount, 1, userPassWdItem);
        ui->userTableWidget->setItem(rowCount, 2, groupIDItem);
    }
}

void UserConfig::initGroupTableWidget()
{
    ui->verticalLayout_2->setAlignment(Qt::AlignTop);

    QList<QString> listUserGroups = SingletonConfig->getUserGroup().keys();
    for(int i = 0; i < listUserGroups.size(); ++i)
    {
        QPushButton *button = new QPushButton(listUserGroups.at(i));
        button->setFixedSize(80, 80);
        ui->verticalLayout_2->addWidget(button);

        connect(button, SIGNAL(clicked()), this, SLOT(dispGroupRight()));

        button->click();
    }

}




void UserConfig::initUserManageDialog()
{
    userManageDialog = new QDialog(this);
    userManageDialog->setFixedSize(200,250);


    QHBoxLayout *hBoxLayout = new QHBoxLayout();
    QLabel *userGroupLabel = new QLabel(QString::fromUtf8("用户组名  "));
    hBoxLayout->addWidget(userGroupLabel);
    userGroupComboBox = new QComboBox();
    userGroupComboBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    hBoxLayout->addWidget(userGroupComboBox);

    QHBoxLayout *hBoxLayout1 = new QHBoxLayout();
    QLabel *userNameLabel = new QLabel(QString::fromUtf8("用户名称  "));
    hBoxLayout1->addWidget(userNameLabel);
    userNameLineEdit = new QLineEdit();
    hBoxLayout1->addWidget(userNameLineEdit);


    QHBoxLayout *hBoxLayout2 = new QHBoxLayout();
    QLabel *userPassWdLabel = new QLabel(QString::fromUtf8("用户密码  "));
    hBoxLayout2->addWidget(userPassWdLabel);
    userPassWdLineEdit = new QLineEdit();
    userPassWdLineEdit->setEchoMode(QLineEdit::Password);
    hBoxLayout2->addWidget(userPassWdLineEdit);

    QHBoxLayout *hBoxLayout3 = new QHBoxLayout();
    QLabel *userPassWdRepLabel = new QLabel(QString::fromUtf8("再次输入  "));
    hBoxLayout3->addWidget(userPassWdRepLabel);
    userPassWdRepLineEdit = new QLineEdit();
    userPassWdRepLineEdit->setEchoMode(QLineEdit::Password);
    hBoxLayout3->addWidget(userPassWdRepLineEdit);

    QHBoxLayout *hBoxLayout4 = new QHBoxLayout();
    //取消增加用户
    cancelAddUserButton = new QPushButton(QString::fromUtf8("取消"));
    connect(cancelAddUserButton, SIGNAL(clicked()), this, SLOT(cancelAddUserClick()));
    hBoxLayout4->addWidget(cancelAddUserButton);
    //确认增加用户
    comfirmAddUserButton = new QPushButton(QString::fromUtf8("确认"));
    connect(comfirmAddUserButton, SIGNAL(clicked()), this, SLOT(comfirmAddUserClick()));
    hBoxLayout4->addWidget(comfirmAddUserButton);
    //取消修改用户
    cancelModifyUserButton = new QPushButton(QString::fromUtf8("取消"));
    connect(cancelModifyUserButton, SIGNAL(clicked()), this, SLOT(modifyUserCancelClick()));
    hBoxLayout4->addWidget(cancelModifyUserButton);
    //确认修改用户
    comfirmModifyUserButton = new QPushButton(QString::fromUtf8("确认"));
    connect(comfirmModifyUserButton, SIGNAL(clicked()), this, SLOT(modifyUserComfirmClick()));
    hBoxLayout4->addWidget(comfirmModifyUserButton);


    QVBoxLayout *rightBoxLayout = new QVBoxLayout();
    rightBoxLayout->addLayout(hBoxLayout);
    rightBoxLayout->addLayout(hBoxLayout1);
    rightBoxLayout->addLayout(hBoxLayout2);
    rightBoxLayout->addLayout(hBoxLayout3);
    rightBoxLayout->addLayout(hBoxLayout4);

    userManageDialog->setLayout(rightBoxLayout);
}

void UserConfig::dispGroupRight()
{
    QPushButton *button = dynamic_cast<QPushButton*>(sender());
    if(button == NULL)
        return;

    ui->rightGroupBox->setTitle(button->text());

    currentUserGroupId = SingletonConfig->getUserGroupId(button->text());
    if(currentUserGroupId == -1)
    {
        qDebug()<<QString::fromUtf8("用户组不存在:")<<button->text();
        return;
    }

    int i = 0;
    QMapIterator<QString, stRightInfo*> itRightItem(SingletonConfig->getRightInfo());
    while(itRightItem.hasNext())
    {
        itRightItem.next();
        stRightInfo *rightInfo = itRightItem.value();
        QCheckBox* checkBox = mapRightCheckBox.value(rightInfo->rightName, NULL);
        if(checkBox == NULL)
        {
            checkBox = new QCheckBox(itRightItem.key());
            connect(checkBox, SIGNAL(clicked(bool)), this, SLOT(rightCheckBoxClick(bool)));

            ui->rightGridLayout->addWidget(checkBox, i / 6, i % 6);
            mapRightCheckBox.insert(itRightItem.key(), checkBox);
            i++;
        }

        if(((rightInfo->groupFlag >> currentUserGroupId) & 0x0001) == 1)
        {
            checkBox->setCheckState(Qt::Checked);
        }else
        {
            checkBox->setCheckState(Qt::Unchecked);
        }
    }

}

void UserConfig::addGroupButtonClick()
{
    if(!SingletonConfig->isCurrentUserhasRight(ADDUSER))
        return;

    bool ok;
    QString inputText = QInputDialog::getText(this, QString::fromUtf8("新增用户组"), QString::fromUtf8("请输入用户组:     "), QLineEdit::Normal, "", &ok);
    if (ok && !inputText.isEmpty())
    {
        if(SingletonConfig->existUserGroup(inputText))
        {
            QMessageBox::information(NULL, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("用户组已存在:") + inputText, QMessageBox::Ok);
            return;
        }
        int groupId = SingletonConfig->getMaxGroupId();
        if(SingletonDBHelper->addUserGroupToDB(inputText, groupId))
        {
            SingletonConfig->setUserGroup(inputText, groupId);
            QPushButton *button = new QPushButton(inputText);
            button->setFixedSize(80, 80);
            ui->verticalLayout_2->addWidget(button);
            connect(button, SIGNAL(clicked()), this, SLOT(dispGroupRight()));
        }
    }
}

void UserConfig::deleteGroupButtonClick()
{
    if(!SingletonConfig->isCurrentUserhasRight(DELETEUSER))
        return;

    QObjectList listChilds= ui->groupBox_UserGroup->children();
    for(int i = 0; i < listChilds.size(); ++i)
    {
        QPushButton *button = dynamic_cast<QPushButton *>(listChilds.at(i));
        if(button == NULL)
            continue;
        //删除当前选中的用户组
        if(button->text() == ui->rightGroupBox->title())
        {
            if(QMessageBox::information(NULL, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("确认删除用户组:") + button->text(), QMessageBox::Ok | QMessageBox::Cancel) == QMessageBox::Ok)
            {
                if(SingletonDBHelper->deleteUserGroupToDB(button->text()))
                    button->deleteLater();
            }
        }
    }
}

void UserConfig::rightCheckBoxClick(bool status)
{
    QCheckBox *checkBox = dynamic_cast<QCheckBox*>(sender());

    if(!SingletonConfig->isCurrentUserhasRight(MODIFYUSERGROUP))
    {
        Qt::CheckState checkState = checkBox->checkState();
        if(checkState == Qt::Unchecked)
        {
            checkBox->setCheckState(Qt::Checked);
        }else if(checkState == Qt::Checked)
        {
            checkBox->setCheckState(Qt::Unchecked);
        }
        return;
    }
    stRightInfo *rightInfo = SingletonConfig->getRightInfo(checkBox->text());

    int flag = rightInfo->groupFlag;
    if(status)
    {
        flag |= (unsigned int)(1 << currentUserGroupId);
    }
    else
    {
        flag &= ~(unsigned int)(1 << currentUserGroupId);
    }

    if(SingletonDBHelper->modifyRightToBD(rightInfo->rightID, flag))
    {
        rightInfo->groupFlag = flag;
    }
}

void UserConfig::addUserClick()
{
    if(!SingletonConfig->isCurrentUserhasRight(ADDUSER))
        return;

    userManageDialog->setWindowTitle(QString::fromUtf8("新增用户"));

    userGroupComboBox->setEnabled(true);
    userGroupComboBox->clear();
    QList<QString> listGroupNames = SingletonConfig->getUserGroup().keys();
    userGroupComboBox->addItems(listGroupNames);

    userNameLineEdit->setEnabled(true);
    userNameLineEdit->clear();
    userPassWdLineEdit->clear();
    userPassWdRepLineEdit->clear();

    comfirmAddUserButton->show();
    cancelAddUserButton->show();
    comfirmModifyUserButton->hide();
    cancelModifyUserButton->hide();
    userManageDialog->show();
}

void UserConfig::deleteUserClick()
{
    if(!SingletonConfig->isCurrentUserhasRight(DELETEUSER))
        return;

    int currentRow = ui->userTableWidget->currentRow();
    if(currentRow == -1)
        return;

    int ret = QMessageBox::warning(this, tr("Warning"), QString::fromUtf8("确认删除用户?"), QMessageBox::Yes | QMessageBox::Cancel);
    if(ret == QMessageBox::Yes)
    {
        QTableWidgetItem *Item = ui->userTableWidget->item(currentRow , 0);

        if(SingletonDBHelper->deleteUserInfoToDB(Item->text()))
        {
            ui->userTableWidget->removeRow(currentRow);
        }
    }
}

void UserConfig::modifyUserClick()
{
    if(!SingletonConfig->isCurrentUserhasRight(MODIFYUSER))
        return;

    int currentRow = ui->userTableWidget->currentRow();
    if(currentRow == -1)
        return;

    userManageDialog->setWindowTitle(QString::fromUtf8("修改用户"));

    //切换新增用户，修改用户按键
    comfirmAddUserButton->hide();
    cancelAddUserButton->hide();
    comfirmModifyUserButton->show();
    cancelModifyUserButton->show();

    userGroupComboBox->clear();
    userGroupComboBox->setEnabled(false);
    QList<QString> listGroupNames;
    listGroupNames<<ui->userTableWidget->item(currentRow, 2)->text();
    userGroupComboBox->addItems(listGroupNames);

    userNameLineEdit->setEnabled(false);
    userNameLineEdit->setText(ui->userTableWidget->item(currentRow, 0)->text());
    userPassWdLineEdit->setText("");
    userPassWdRepLineEdit->setText("");

    userManageDialog->show();
}

void UserConfig::comfirmAddUserClick()
{
    if(userNameLineEdit->text().isEmpty() || userNameLineEdit->text().contains(' '))
    {
        QMessageBox::warning(this, tr("Warning"), QString::fromUtf8("用户名含有空格"),QMessageBox::Yes);
        return;
    }
    if(userPassWdLineEdit->text() != userPassWdRepLineEdit->text())
    {
        QMessageBox::warning(this, tr("Warning"), QString::fromUtf8("密码不一致"),QMessageBox::Yes);
        return;
    }

    if(SingletonConfig->IsUserExits(userNameLineEdit->text()))
    {
        QMessageBox::warning(this, tr("Warning"), QString::fromUtf8("用户名已存在"),QMessageBox::Yes);
    }
    else
    {
        if(SingletonDBHelper->insertUserInfoToDB(userNameLineEdit->text(), userPassWdLineEdit->text(), userGroupComboBox->currentText()))
        {
            stUserInfo *userInfo = new stUserInfo();
            userInfo->userName = userNameLineEdit->text();
            userInfo->userPassWd = userPassWdLineEdit->text();
            userInfo->groupName = userGroupComboBox->currentText();

            SingletonConfig->setUserInfo(userInfo->userName, userInfo);
            initUserTableWidget();
            userManageDialog->hide();
        }
    }
}

void UserConfig::cancelAddUserClick()
{
    userManageDialog->hide();
}


void UserConfig::modifyUserComfirmClick()
{
    if(userPassWdLineEdit->text() != userPassWdRepLineEdit->text())
    {
        QMessageBox::warning(this, tr("Warning"), QString::fromUtf8("密码不一致"),QMessageBox::Yes);
        return;
    }

    int currentRow = ui->userTableWidget->currentRow();
    QString userName = ui->userTableWidget->item(currentRow, 0)->text();


    if(SingletonDBHelper->modifyUserInfoToDB(userName, userPassWdLineEdit->text()))
    {
        stUserInfo* userInfo = SingletonConfig->getUserInfo(userName);
        userInfo->groupName = userGroupComboBox->currentText();
        userInfo->userPassWd = userPassWdLineEdit->text();

        initUserTableWidget();
        userManageDialog->hide();
    }
    else
    {
         QMessageBox::warning(this, tr("Warning"), QString::fromUtf8("数据库修改失败"),QMessageBox::Yes);
    }
}

void UserConfig::modifyUserCancelClick()
{
    userManageDialog->hide();
}




