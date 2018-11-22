#include "analysispcapfile.h"
#include "ui_analysispcapfile.h"
#include "time.h"
#include <QFileInfo>
#include<QDebug>
#define DATA_LEN 257

AnalysisPcapFile::AnalysisPcapFile(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AnalysisPcapFile)
{
    ui->setupUi(this);
    //设置界面

    QDir dir;
    if(!dir.exists(PCAPFILEPATH))
        dir.mkdir(PCAPFILEPATH);

    ui->splitter->setHandleWidth(3);
    ui->splitter_3->setHandleWidth(3);
    ui->splitter->setHandleWidth(3);

    ui->splitter->setStretchFactor(0, 2);
    ui->splitter->setStretchFactor(1, 8);

    ui->splitter_3->setStretchFactor(0, 4);
    ui->splitter_3->setStretchFactor(1, 6);

    ui->splitter_2->setStretchFactor(0, 6);
    ui->splitter_2->setStretchFactor(1, 4);

    ui->tabWidget->setTabText(0,QString::fromLocal8Bit("报文解析"));

    m_PcapFileName = ui->PcapFileName;
    m_PcapFileName->setMinimumWidth(200);
    m_PcapFileName->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    m_PcapFileName->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_PcapFileName->verticalHeader()->hide();
    m_PcapFileName->horizontalHeader()->setStretchLastSection(true);
    connect(m_PcapFileName,SIGNAL(cellClicked(int, int)), this,SLOT(PcapFileName (int, int )));

    m_PcapHreadInfo = ui->PcapHreadInfo;
    m_PcapHreadInfo->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    m_PcapHreadInfo->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    m_PcapTreeinfo =  ui->PcapTreeinfo;
    m_PcapTreeinfo->header()->setVisible(false);
    m_PcapTreeinfo->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    m_PcapTreeinfo->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    connect(m_PcapTreeinfo,SIGNAL(itemClicked(QTreeWidgetItem*,int)), this,SLOT(treeInfoItemClicked(QTreeWidgetItem* ,int)));//检测点击事件，信号槽机制

    menu = new QMenu();
    QAction *action = new QAction(QString::fromLocal8Bit("导出报文"), menu);
    menu->addAction(action);

    m_PcapHreadInfo = ui->PcapHreadInfo;
    m_PcapHreadInfo->setColumnCount(7);
    m_PcapHreadInfo->setSelectionBehavior ( QAbstractItemView::SelectRows);
    m_PcapHreadInfo->horizontalHeader()->setClickable(false); //设置表头不可点击（默认点击后进行排序）
    m_PcapHreadInfo->verticalHeader()->setDefaultSectionSize(20); //设置行高
    m_PcapHreadInfo->setFrameShape(QFrame::NoFrame); //设置无边框
    m_PcapHreadInfo->horizontalHeader()->setStretchLastSection(true);
    m_PcapHreadInfo->horizontalHeader()->setVisible(true); //设置垂直头不可见
    m_PcapHreadInfo->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_PcapHreadInfo->horizontalHeader()->setStretchLastSection(true);
    m_PcapHreadInfo->setContextMenuPolicy(Qt::CustomContextMenu);           //允许右击菜单
    connect(m_PcapHreadInfo, SIGNAL(cellClicked(int, int)),this, SLOT(checkColumnSlot(int, int )));
    connect(m_PcapHreadInfo, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showCustomMenu(QPoint)));

    QStringList header;  //QString类型的List容器
    header<<QString::fromUtf8("时间")<<QString::fromUtf8("源地址")<<QString::fromUtf8("目标地址")<<QString::fromUtf8("协议")<<QString::fromUtf8("长度")<<QString::fromUtf8("端口")<<QString::fromUtf8("描述");
    m_PcapHreadInfo->setHorizontalHeaderLabels(header);
    handle = NULL;

/*****************测试代码************************/

    InitWireshark();
    treeItemLevel = -1 ;



/*************************************************/

   m_PcapBinaryInfo = ui->PcapBinaryInfo;

   m_PcapBinaryInfo->setReadOnly(true);
}

AnalysisPcapFile::~AnalysisPcapFile()
{
    delete ui;
}
void AnalysisPcapFile::treeInfoItemClicked(QTreeWidgetItem* item,int icolumn)
{
//    QVariant v = item->data(icolumn,0);
//    PackageTreeItemToData info = v.value<PackageTreeItemToData>();

    //    printf("---------------%d\n",info.iLen);
}



void AnalysisPcapFile::showCustomMenu(QPoint point)
{
    menu->move(cursor().pos ());
    menu->exec();

    QString m_strFileName = PCAPFILEPATH + m_PcapFileName->currentItem()->text() + "_" + QDateTime::currentDateTime().toString("yyyyMMddhhmmss") + ".pcap" ;
    m_strFileName = QFileDialog::getSaveFileName(NULL, QString::fromLocal8Bit("保存文件"), m_strFileName, tr("Image Files (*.pcap)"));
    if (m_strFileName.isEmpty() || m_strFileName.isNull())
        return;

    //qDebug()<<m_strFileName;
    char errBuf[PCAP_ERRBUF_SIZE] = {0};
    char *dev = pcap_lookupdev(errBuf);
    pcap_t * handle = pcap_open_live(dev, 65535, 1, 0, errBuf);
    if(handle == NULL)
    {
        qDebug()<<"device open failure:"<<dev <<" err msg:"<<errBuf;
        return;
    }

    pcap_dumper_t *dumpfile = pcap_dump_open(handle, m_strFileName.toLocal8Bit().data());

    QList<QTableWidgetSelectionRange>ranges=m_PcapHreadInfo->selectedRanges();
    int count=ranges.count();
    for(int i = 0;i < count; i++)
    {
        int topRow = ranges.at(i).topRow();

        int bottomRow = ranges.at(i).bottomRow();

        for(int j = topRow;j <= bottomRow; j++)
        {
            pcap_dump((u_char*)dumpfile,  m_pkgInfo[j]->header, m_pkgInfo[j]->data);
        }
    }

    pcap_dump_close(dumpfile);
    pcap_close(handle);
}

void AnalysisPcapFile::checkColumnSlot(int irow, int icolumn)
{
    m_PcapBinaryInfo->clear();
    m_PcapTreeinfo->clear();
    print_tree(m_pkgInfo[irow]->edt->tree->first_child, 0);
    PrintBuffer(m_pkgInfo[irow]->data, m_pkgInfo[irow]->iLen);
}

void AnalysisPcapFile::PcapFileName(int irow, int icolumn)
{

        ReleaseEpanDissect();
        m_PcapBinaryInfo->clear();
        m_PcapTreeinfo->clear();
        m_PcapHreadInfo->clearContents();
        printf("----------------------%d--------\n",m_PcapHreadInfo->rowCount());
        m_PcapHreadInfo->setRowCount(0);

        QString strFileFullPath =  m_filePaht[m_PcapFileName->item(irow,icolumn)->text()].strFilePath;
        ReadPcapFile(strFileFullPath);
}

void AnalysisPcapFile::AddFileNameToTree()
{
    m_PcapHreadInfo->setRowCount(0);
    while(m_PcapTreeinfo->topLevelItemCount())
    {
        QTreeWidgetItem* item = m_PcapTreeinfo->takeTopLevelItem(0);
        delete item;
    }
    m_PcapBinaryInfo->clear();
    ReleaseEpanDissect();

    m_PcapFileName->setRowCount(0);
    int RowCont = m_PcapFileName->rowCount();
    QMap<QString ,FileInfo>::iterator it;
    for(it = m_filePaht.begin(); it != m_filePaht.end();it++)
    {
        m_PcapFileName->insertRow(RowCont);//增加一行
        m_PcapFileName->setItem(RowCont,0,new QTableWidgetItem(it.key()));
    }
}

bool AnalysisPcapFile::GetFileName(FileInfo & fileinfo,QString filePath)
{
    QFileInfo info =  QFileInfo(filePath);

    if(info.isFile() == true)
    {
        fileinfo.strFileName = info.baseName();
        fileinfo.strFilePath = filePath;
        return true;
    }
    return false;
}
void AnalysisPcapFile::SetFileToTree(QList<QString> filePath)
{
    m_filePaht.clear();
    QList<QString>::iterator it = filePath.begin();
    bool bRet = false;
    for(;it < filePath.end(); it++)
    {
        FileInfo fileinfo;
        bRet = GetFileName(fileinfo,*it);
        if (bRet == true)
        {
            m_filePaht.insert(fileinfo.strFileName,  fileinfo);
        }
    }
    AddFileNameToTree();
}
void AnalysisPcapFile::InitWireshark()
{

}

void AnalysisPcapFile::ReleaseWireshark()
{
    cleanup_dissection();
    epan_cleanup();
}

void AnalysisPcapFile::ReleaseEpanDissect()
{
    if ( 0 == m_pkgInfo.count() )
    {
        return;
    }
printf("------m_pkgInfo.size() = %d--------\n", m_pkgInfo.size() );
    QMap<int,TableShowInfo*>::const_iterator it;
    int i = 0, j = 0;
    for (it = m_pkgInfo.constBegin(); it != m_pkgInfo.constEnd(); it++)
    {
 //       printf("---ReleaseEpanDissect------------------*****--------------%d   edt:%p\n",i,it.value()->edt);

        if ( NULL != it.value()->edt )
        {
            j++;

            epan_dissect_free(it.value()->edt);
            it.value()->edt = NULL;
        }

        delete it.value();

        //g_free(it.value().fdata);
        i++;
    }

    printf("------ReleaseEpanDissect(): i = %d, j = %d--------\n", i, j );

    m_pkgInfo.clear();
//    cleanup_dissection();
 //   epan_cleanup();

//    printf("----handle : %p--------\n",handle);
//    if(handle != NULL)
//    {
//        printf("------pcap_close--------\n");
//        pcap_close(handle);
//        handle = NULL;
//    }
   // pcap_t *handle;//会话句柄
}

void AnalysisPcapFile::LoopCallback(u_char *args, const struct pcap_pkthdr *header, const u_char *packet) {
    static int count = 0;                   // 包计数器
    const struct packet_ethernet *ethernet;  /* The ethernet header [1] */

//    const char *payload;                     /* Packet payload */
//   // time_t t;
//    int size_ip;
//    int size_tcp;
//    int size_payload;

    count++;

//printf("-----------------begin-------------------\n");

    /* 以太网头 */
    ethernet = (struct packet_ethernet*)(packet);
    AnalysisPcapFile* pcapFile = (AnalysisPcapFile* )args;

    pcapFile->TryDissect((guchar*)packet,header);

//printf("-----------------end-------------------\n");
}

 void AnalysisPcapFile::ReadPcapFile(QString strFileFullName)
 {
printf("------------ReadPcapFile------------------------\n");
     char error[100];//存储错误信息字符串
     struct pcap_pkthdr header; /* 由pcap.h定义 */
     if((handle=pcap_open_offline(strFileFullName.toStdString().c_str(),error))==NULL)  //打开文件
     {
         printf("%s\n",error);
     }
     pcap_loop( handle, -1, LoopCallback, (u_char*)this);
}


void AnalysisPcapFile::FormatTime(time_t time1, char *szTime)
{
    struct tm tm1;
    localtime_r(&time1, &tm1 );
    sprintf( szTime, "%4.4d-%2.2d-%2.2d %2.2d:%2.2d:%2.2d",
        tm1.tm_year+1900, tm1.tm_mon+1, tm1.tm_mday,
          tm1.tm_hour, tm1.tm_min,tm1.tm_sec);
}

void AnalysisPcapFile::TryDissect(guchar * data, const struct pcap_pkthdr *header)
 {
    frame_data  *fdata = NULL;
    epan_dissect_t  *edt = NULL;
    struct pcap_pkthdr *pheader = new struct pcap_pkthdr;
    memcpy(pheader, header, sizeof(struct pcap_pkthdr));
    union wtap_pseudo_header  pseudo_header;
    pseudo_header.eth.fcs_len = -1;

    fdata = (frame_data*)g_new(frame_data, 1);

    memset(fdata, 0, sizeof(frame_data));
    fdata->pfd  = NULL;
    fdata->num = 1;
    //fdata->interface_id = 0;
    fdata->pkt_len  = header->len;
    fdata->cap_len  = header->caplen;
    fdata->cum_bytes = 0;
    fdata->file_off = 0;
    fdata->subnum = 0;
    fdata->lnk_t = WTAP_ENCAP_ETHERNET;
    fdata->flags.encoding = PACKET_CHAR_ENC_CHAR_ASCII;
    fdata->flags.visited = 0;
    fdata->flags.marked = 0;
    fdata->flags.ref_time = 0;
    fdata->color_filter = NULL;
    fdata->abs_ts.secs = 0;
    fdata->abs_ts.nsecs = 0;
    fdata->opt_comment = NULL;

    edt = epan_dissect_new(TRUE, TRUE);
//   printf("TryDissect-epan_dissect_new--------------------*****-------------edt:%p\n",edt);
    epan_dissect_run(edt, &pseudo_header, data, fdata, NULL);

//    printf("-epan_dissect_new--------------------*****-------------edt:%p\n",edt);
     TableShowInfo *packetHearInfo = new (std::nothrow) TableShowInfo();

     if ( NULL == packetHearInfo )
     {
         g_free( fdata );

         return;
     }
    packetHearInfo->header = pheader;
    char ip[50] = {0};
    memset(ip,0,50);
    memcpy(ip , (char*)edt->pi.net_src.data,edt->pi.net_src.len);
    sprintf(ip,"%d.%d.%d.%d\n", ip[0]& 0xff,ip[1]& 0xff,ip[2]& 0xff,ip[3]& 0xff);
    packetHearInfo->strSrcAddr = QString(ip);
    memset(ip,0,50);
    memcpy(ip , (char*)edt->pi.net_dst.data,edt->pi.net_dst.len);

    sprintf(ip,"%d.%d.%d.%d", ip[0]& 0xff,ip[1]& 0xff,ip[2]& 0xff,ip[3]& 0xff);
    packetHearInfo->strDscAddr = QString(ip);

    packetHearInfo->edt = edt;
    packetHearInfo->data = data;
    packetHearInfo->fdata = fdata;

    memset(ip,0,50);
    time_t itime= header->ts.tv_sec;
    //sprintf(ip,"%s",ctime(&itime));
    FormatTime(itime, ip);
    packetHearInfo->strTime = QString("%1.%2").arg(ip).arg(header->ts.tv_usec);
    packetHearInfo->strPoro = QString("source port:%1 - destination port:%2").arg(edt->pi.srcport).arg(edt->pi.destport);
    packetHearInfo->iLen = header->caplen;

    //printf( "edt->pi.dcetransporttype = '%s'\n", edt->pi.current_proto );

    int RowCont;
    RowCont=m_PcapHreadInfo->rowCount();
    m_PcapHreadInfo->insertRow(RowCont);//增加一行
    m_PcapHreadInfo->setItem(RowCont,0,new QTableWidgetItem(packetHearInfo->strTime));
    m_PcapHreadInfo->setItem(RowCont,1,new QTableWidgetItem(packetHearInfo->strSrcAddr));
    m_PcapHreadInfo->setItem(RowCont,2,new QTableWidgetItem(packetHearInfo->strDscAddr));
    m_PcapHreadInfo->setItem(RowCont,4,new QTableWidgetItem(QString("%1").arg(packetHearInfo->iLen)));
    m_PcapHreadInfo->setItem(RowCont,5,new QTableWidgetItem(QString(packetHearInfo->strPoro)));
    m_PcapHreadInfo->setItem(RowCont,6,new QTableWidgetItem(""));
    m_pkgInfo.insert(RowCont,packetHearInfo);

 //         epan_dissect_free(edt);
          g_free(fdata);

//          if(handle != NULL)
//          {
//              printf("------pcap_close--------\n");
//              pcap_close(handle);
//              //pcap_dump_close
//              //handle = NULL;
//          }

}
QString AnalysisPcapFile:: HexcharToQstring(void* pBuff,  int nLen)
{

    if (NULL == pBuff)
    {
        return "";
    }
    QString strData;
    const int nBytePerLine = nLen;
    unsigned char* p = (unsigned char*)pBuff;
    //char szHex[3*nBytePerLine+1] = {0};
    char *szHex = (char*)malloc(sizeof(char) *(3*nBytePerLine+1));

    for (unsigned int i=0; i<nLen; ++i)
    {
        int idx = 3 * (i % nBytePerLine);
        if (0 == idx)
        {
            memset(szHex, 0, sizeof(szHex));
        }
        snprintf(&szHex[idx], 4, "%02x ", p[i]); // buff长度要多传入1个字节
        // 以16个字节为一行，进行打印
        if (0 == ((i+1) % nBytePerLine))
        {
            strData = QString("%1").arg((char*)szHex);
        }
    }
    return strData;
}


void AnalysisPcapFile::PrintBuffer(void* pBuff, unsigned int nLen)
 {
       m_PcapBinaryInfo->clear();
     if (NULL == pBuff || 0 == nLen)
     {
         return;
     }

     const int nBytePerLine = 16;
     unsigned char* p = (unsigned char*)pBuff;
     char szHex[3*nBytePerLine+1] = {0};

    // printf("-----------------begin-------------------\n");
     for (unsigned int i=0; i<nLen; ++i)
     {
         int idx = 3 * (i % nBytePerLine);
         if (0 == idx)
         {
             memset(szHex, 0, sizeof(szHex));
         }
         snprintf(&szHex[idx], 4, "%02x ", p[i]); // buff长度要多传入1个字节
         // 以16个字节为一行，进行打印
         if (0 == ((i+1) % nBytePerLine))
         {
             QString strData = QString("%1").arg((char*)szHex);
             m_PcapBinaryInfo->append(strData.toUpper());
         }
     }

     // 打印最后一行未满16个字节的内容
     if (0 != (nLen % nBytePerLine))
     {
         QString strData = QString("%1").arg((char*)szHex);
         m_PcapBinaryInfo->append(strData.toUpper());
     }

 //    printf("------------------end-------------------\n");
 }

void AnalysisPcapFile::print_tree(proto_tree* tree, int level)
 {
      if(tree == NULL)
         return;
     gchar field_str[ITEM_LABEL_LENGTH + 1] = {0};
     if(tree->finfo->rep == NULL)
         proto_item_fill_label(tree->finfo, field_str);
     else
        strcpy(field_str, tree->finfo->rep->representation);
     if(!PROTO_ITEM_IS_HIDDEN(tree))
     {
         PackageTreeItemToData info;
         info.iStart = tree->finfo->start;
         info.iLen = tree->finfo->length;


        if(treeItemLevel != level)
         {

             if(level == 0) //Prent Point
             {
                m_TreeWidget.clear();
                treeItemLevel = 0;
                m_TreeWidget.insert(treeItemLevel,(void*)m_PcapTreeinfo);
                PrentTreeItem = NULL;
                PrentTreeItem = new QTreeWidgetItem((QTreeWidget*)m_PcapTreeinfo,QStringList(QString(field_str)));
              //  QVariant v3 = QVariant::fromValue(info);
              //  PrentTreeItem->setData(0, Qt::UserRole,  QVariant(v3));
             }
             else
             {
                 treeItemLevel = level;
                 if(m_TreeWidget[level -1] != m_TreeWidget.end())
                 {
                    m_TreeWidget.insert(treeItemLevel,(void*)PrentTreeItem);
                    PrentTreeItem = new QTreeWidgetItem((QTreeWidgetItem*)PrentTreeItem,QStringList(QString(field_str)));
                   // QVariant v3 = QVariant::fromValue(info);
                   // PrentTreeItem->setData(0, Qt::UserRole,  QVariant(v3));
                 }
             }

         }
         else
         {
           QTreeWidgetItem *Item = new QTreeWidgetItem((QTreeWidgetItem*)PrentTreeItem,QStringList(QString(field_str)));
          // QVariant v3 = QVariant::fromValue(info);
          // Item->setData(0, Qt::UserRole,  QVariant(v3));
         }

//         printf("******************************************\n");
//         printf("%s\n", field_str);
//         printf("******************************************\n");

     }

     print_tree(tree->first_child, level+1);
     print_tree(tree->next, level);
 }

//void AnalysisPcapFile::on_PcapHreadInfo_clicked(const QModelIndex &index)
//{

//}
