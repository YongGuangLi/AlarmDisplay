/*
 * HiRedis.h
 *
 *  Created on: Sep 12, 2017
 *      Author: root
 */

#ifndef HIREDIS_H_
#define HIREDIS_H_
#include <string>
#include <string.h>
#include <vector>
#include <map>
#include "hiredis.h"
#include <list>
#include <QMutex>

using namespace std;

class IDB_Oper
{
public:
	IDB_Oper();
	virtual ~IDB_Oper();
public:
	bool 	Connect(int ConnectTpye,const char* host,unsigned short u_port);
	bool 	Subscribe(vector<string> channel);
	/* 名称 ＝》int GetUpdatePoints(char *szSendBuff)
	 * 功能 ＝》获取消息的内容（值的内容）
	 * 参数 ＝》传入char 指针，定义时保证申请的空间足够大，建议大小  2048
	 * 返回 ＝》实际内存的大小
	 */
	int 	GetUpdatePoints(char *szSendBuff);
	/* 名称 ＝》WriteValue(char *szSendBuff,int size,vector<string> posChannel);
	 * 功能 ＝》发送消息
	 * 参数 ＝》1、发送的内容 2、发送的字节大小，3发送的频道；
	 * 返回 ＝》返回执行的结果
	 */
    bool 	WriteValue(char *szSendBuff, int size, vector<string> posChannel, string key="NULL");
	bool    check_connection(int ConnectTpye);
	bool 	DisConnect();
	static void * ThreadRecvData(void * pVoid);

private:
	redisContext * m_pContextSub;
	redisContext * m_pContextPub;

	map<string,string> m_dataMap;
    QMutex  m_dataMutex; //线程锁

	//list<Tag> m_listData;
	bool m_bRunFlag;
};

#endif /* HIREDIS_H_ */
