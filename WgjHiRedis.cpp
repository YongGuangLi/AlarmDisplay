/*
 * HiRedis.cpp
 *
 *  Created on: Sep 12, 2017
 *      Author: root
 */

#include "WgjHiRedis.h"

IDB_Oper::IDB_Oper() {
	// TODO Auto-generated constructor stub
	m_pContextPub = NULL;
	m_pContextSub = NULL;
	m_bRunFlag = true;
}

IDB_Oper::~IDB_Oper() {
	redisFree(m_pContextPub);
	redisFree(m_pContextSub);
	m_bRunFlag = false;
	m_pContextPub = NULL;
	m_pContextSub = NULL;
//	pthread_mutex_destroy(&m_dataMutex);
}

bool IDB_Oper::Connect(int ConnectTpye,const char* host,unsigned short u_port  ){


    struct timeval timeout = { 1, 500000 };
	m_pContextSub = redisConnectWithTimeout(host,u_port,timeout);
	if ( NULL == m_pContextSub || m_pContextSub->err  )
	{
		printf( "%s\n", m_pContextSub->errstr );
		return false;
	}
	m_pContextPub = redisConnectWithTimeout(host,u_port,timeout);
	if ( NULL == m_pContextPub || m_pContextPub->err == 1 )
	{
		printf( "%s\n", m_pContextPub->errstr );
		return false;
	}
	//连接成功后，创建一个线程用于数据接收的
//	pthread_mutex_init(&m_dataMutex,NULL);
	//pthread_t id;
	//int ret = pthread_create(&id, NULL, ThreadRecvData, this);
	return true;
}

bool IDB_Oper::Subscribe(vector<string> channel)
{
	bool bresult = true;
	for(int i =0; i<channel.size();i++)
	{
		 redisReply * r = (redisReply *)redisCommand( this->m_pContextSub, "SUBSCRIBE %s", channel[i].c_str() );
		 if (r ==NULL )
		 {
			 bresult = false;
		 }
		 freeReplyObject(r);
	}
	return bresult;
}
bool   IDB_Oper::DisConnect()
{
	//m_pContextPub=NULL;
	redisFree(m_pContextPub);
	//m_pContextSub=NULL;
	redisFree(m_pContextSub);
	m_pContextPub = NULL;
	m_pContextSub = NULL;
	return true;
}
bool   IDB_Oper::WriteValue(char *szSendBuff, int size, vector<string> posChannel, string key)
{
	if(!(size > 0)) return false;

	redisReply * r = NULL;

//	pthread_mutex_lock(&m_dataMutex);
    m_dataMutex.lock();
	for(int number =0 ;number < posChannel.size() ;number++)
	{
		r = (redisReply *)redisCommand( this->m_pContextPub, "PUBLISH %s %b" ,
				posChannel[number].c_str(),
				szSendBuff,
				(size_t) size);

        freeReplyObject(r);
	}



    redisReply * r1 = NULL;
    r1 = (redisReply *)redisCommand( this->m_pContextPub, "set %s %b" ,
                                     key.c_str(),
                                     szSendBuff,
                                     (size_t) size);
    freeReplyObject(r1);

//	pthread_mutex_unlock(&m_dataMutex);
     m_dataMutex.unlock();
	return true;
}

int IDB_Oper::GetUpdatePoints(char *szSendBuff)
{

	redisReply * pReply = NULL;
	int len = 0;
	if ( redisGetReply( m_pContextSub, (void **)&pReply ) == REDIS_OK )
	{
		redisReply * pSubReply = NULL;
		if ( pReply != NULL && pReply->elements == 3 )
		{
			pSubReply = pReply->element[2];
			memcpy(szSendBuff,pSubReply->str,pSubReply->len);
			len = pSubReply->len;
		}
		freeReplyObject( pReply );
	}
	return len;
	//	int num =0 ;
	//	for(int i = 0 ; i < tagNum ;i++)
	//	{
	//		if(i < this->m_listData.size())
	//		{
	//			//pthread_mutex_lock(&m_dataMutex);
	//			strcpy(m_tag[i].pName , this->m_listData.begin()->pName);
	//			memcpy(m_tag[i].pdata,  this->m_listData.begin()->pdata,this->m_listData.begin()->nLen);
	//			m_tag[i].nLen = this->m_listData.begin()->nLen;
	//			m_listData.pop_front();
	//			//pthread_mutex_unlock(&m_dataMutex);
	//			num ++;
	//		}
	//	}
	//	return num;
}
bool  IDB_Oper::check_connection(int ConnectTpye)
{
	char szMsgCmd[128]={0};
	bool result = true;
	sprintf(szMsgCmd,"ping");
//	pthread_mutex_lock(&m_dataMutex);
    m_dataMutex.lock();
	if(m_pContextPub == NULL)
	{
    //	pthread_mutex_unlock(&m_dataMutex);
        m_dataMutex.unlock();
		return false;
	}
	redisReply * r = (redisReply *)redisCommand( this->m_pContextPub, szMsgCmd);
	if(r == NULL)
		result = false;
    m_dataMutex.unlock();

	freeReplyObject(r);
	return result;
}

void * IDB_Oper::ThreadRecvData(void * pVoid)
{
	IDB_Oper *wgjHiRedis = (IDB_Oper *)pVoid;
	redisContext *pRedisContext = wgjHiRedis->m_pContextSub;
	while(wgjHiRedis->m_bRunFlag)
	{
		redisReply * pReply;
		if(!wgjHiRedis->check_connection(1))
		{
			wgjHiRedis->DisConnect();
		//	m_pContextSub = redisConnectWithTimeout(host,u_port,timeout);
			continue;
		}
		while ( redisGetReply( pRedisContext, (void **)&pReply ) == REDIS_OK )
		{
			redisReply * pSubReply = NULL;
			if ( pReply != NULL && pReply->elements == 3 )
			{
				pSubReply = pReply->element[2];
				string strkey = pSubReply->str;
				//printf("key = %s\n" ,strkey.c_str());
				string strValue = "";
//				pthread_mutex_lock(& wgjHiRedis->m_dataMutex);
                wgjHiRedis->m_dataMutex.lock();
				int pos = strkey.find(',');
				if(pos >0)
					strkey = strkey.substr(0,pos);
				redisReply *r = (redisReply*)redisCommand(wgjHiRedis->m_pContextPub, "get %s",strkey.c_str());
				if ( r->type == REDIS_REPLY_STRING || r->type == REDIS_REPLY_ARRAY)
				{
					strValue = r->str;
//					Tag tag;
//					strcpy(tag.pName,strkey.c_str());
//					memcpy(tag.pdata,r->str,r->len);
//					tag.nLen = r->len;
//					wgjHiRedis->m_listData.push_back(tag);
				}
//				pthread_mutex_unlock(& wgjHiRedis->m_dataMutex);
                wgjHiRedis->m_dataMutex.unlock();
				freeReplyObject(r);
			}
			freeReplyObject( pReply );
		}
	}
	return NULL;
}
