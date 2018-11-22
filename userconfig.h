#ifndef USERCONFIG_H
#define USERCONFIG_H

#include <QDebug>
#include <QWidget>
#include <QMap>
#include <QString>
#include <QCheckBox>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QDialog>
#include <QFile>
#include <QInputDialog>
#include <QDir>
#include <QSettings>
#include <QTextCodec>
#include "configini.h"
#include "databasehelper.h"



namespace Ui {
class UserConfig;
}

class UserConfig : public QDialog
{
    Q_OBJECT
    
public:
    explicit UserConfig(QDialog *parent = 0);
    ~UserConfig();

    //显示所有用户信息
    void initUserTableWidget();

    //显示所有用户组信息
    void initGroupTableWidget();

    //创建用户管理对话窗口
    void initUserManageDialog();
private slots:
    //显示用户组权限
    void dispGroupRight();
    //新增用户组
    void addGroupButtonClick();

    void deleteGroupButtonClick();

    //修改用户组权限
    void rightCheckBoxClick(bool);

    //添加成员 修改成员
    void addUserClick();                          //点击新增用户按钮
    void deleteUserClick();                       //点击删除用户按钮
    void modifyUserClick();                       //点击修改用户按钮

    //点击确认新增用户按钮
    void comfirmAddUserClick();
    //点击取消新增用户按钮
    void cancelAddUserClick();

    //点击确认修改用户按钮
    void modifyUserComfirmClick();
    //点击取消修改用户按钮
    void modifyUserCancelClick();

private:
    Ui::UserConfig *ui;


    QMap<QString, QCheckBox*> mapRightCheckBox;  //所有权限checkBox key:权限名 value:checkBox

    int currentUserGroupId;

    //添加成员 修改成员
    QDialog *userManageDialog;      //添加,修改用户界面

    QComboBox *userGroupComboBox;
    QLineEdit *userNameLineEdit;
    QLineEdit *userPassWdLineEdit;
    QLineEdit *userPassWdRepLineEdit;

    QPushButton *comfirmAddUserButton;
    QPushButton *cancelAddUserButton;

    QPushButton *comfirmModifyUserButton;
    QPushButton *cancelModifyUserButton;


    QString configIp;
    int configPort;
};



#endif // USERCONFIG_H
