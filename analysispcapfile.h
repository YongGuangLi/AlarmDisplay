#ifndef ANALYSISPCAPFILE_H
#define ANALYSISPCAPFILE_H

#include "StructClass.h"

#include <QWidget>
#include <QList>
#include <QTreeWidget>
#include <QTableWidget>
#include <QTextEdit>
#include <QFile>
#include <QFileDialog>
#include <QDateTime>
#include <QMenu>
#include <QDir>
#include <pcap.h>

#define PCAPFILEPATH "/home/GM2000/PcapSegment/"



namespace Ui {
class AnalysisPcapFile;
}

class AnalysisPcapFile : public QWidget
{
    Q_OBJECT
    
public:
    explicit AnalysisPcapFile(QWidget *parent = 0);
    ~AnalysisPcapFile();
public:
    QMap<QString ,FileInfo> m_filePaht;
    QMap<QString ,QTreeWidgetItem *> m_PcapNameItem;
    QMap<int,TableShowInfo*> m_pkgInfo;
    QMap<int,void*> m_TreeWidget;
    QTableWidget* m_PcapFileName ;
    QTableWidget* m_PcapHreadInfo;
    QTreeWidget* m_PcapTreeinfo ;
    QTextEdit *m_PcapBinaryInfo;
private:
    QTreeWidgetItem * PrentTreeItem;
    pcap_t *handle;//会话句柄

    int  treeItemLevel;
public:
    void SetFileToTree(QList<QString> filePath);
public slots:
    void PcapFileName(int irow, int icolumn);
    //初始化wireshark，在new完这个里后面使用。
    void InitWireshark();
    //释放wireshark，在使用析构此类之前使用。
    void ReleaseWireshark();
    //切换文件名称时候调用。释放本文件申请的控件。
    void ReleaseEpanDissect();
    void checkColumnSlot(int irow, int icolumn);
    void treeInfoItemClicked(QTreeWidgetItem* ,int icolumn);


    void showCustomMenu(QPoint);
private slots:
    void PrintBuffer(void* pBuff, unsigned int nLen);
    QString HexcharToQstring(void* pBuff, int nLen);
private:
    bool GetFileName(FileInfo & fileinfo,QString filePath);
    void AddFileNameToTree();
    void ReadPcapFile(QString strFileFullName);
    void FormatTime(time_t time1, char *szTime);
    void TryDissect(guchar* data, const struct pcap_pkthdr *header);
    static void LoopCallback(u_char *args, const struct pcap_pkthdr *header, const u_char *packet);
    void print_tree(proto_tree* tree, int level);
private:
    Ui::AnalysisPcapFile *ui;
     QMenu *menu;
};

#endif // ANALYSISPCAPFILE_H
