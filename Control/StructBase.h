#ifndef __STRUCTBASE_H_INCLUDED__
#define __STRUCTBASE_H_INCLUDED__

#include <string>

//�����ʼ��ʧ�ܷ���ֵ
#define INIT_CONTEXT_ERROR							-1001                       //��ʼ����Ϣ����������ʧ��
#define SET_CONTEXT_SET_ERROR						-1002		                //��ʼ����������ʧ��
#define INIT_REQUEST_RESPONSE_SOCKET_ERROR			-1003		                //��������Ӧ��socketʧ��
#define INIT_REQUEST_RESPONSE_CONNECT_ERROR			-1004		                //��������Ӧ��ʧ��
#define INIT_PUBLISH_SUBSCRIBE_SOCKET_EREOR			-1005		                //������������socketʧ��
#define INIT_PUBLISH_SUBSCRIBE_CONNECT_ERROR		-1006		                //���ӷ�������ʧ��
#define GET_PUBLISH_PORT_ERROR						-1007		                //�ӷ������л�÷���socket�Ķ˿�
#define RECV_MESSAGE_EVENT_ERROR					-1008		                //�������������̹߳��������¼�
#define INIT_PUBLISH_SUBSCRIBE_BIND_ERROR			-1009		                //���ķ����󶨴���
#define INIT_REQUEST_RESPONSE_BIND_ERROR			-1010		                //����Ӧ��󶨴���
#define FIXED_LENGTH_MESSAGE						1024*60                     //���ͺͽ��̶ܹ���Ϣ�ĳ���
#define FIXED_LENGTH_MESSAGE_TOP					sizeof(MsgBuffer)			//���ͺͽ�����Ϣ����Ϣͷ����
#define FIXED_OVERALL_LENGTH_MESSAGE                FIXED_LENGTH_MESSAGE + FIXED_LENGTH_MESSAGE_TOP

/****************************************
 *�������ڿͻ��˵��úͷ���������
****************************************/
typedef enum ThroughState{
	RealState = 0,		                                        //ʵʱ̬
	InversionState,		                                        //����̬
	ResearchState,		                                        //�о�̬
	TestState			                                        //����̬
}ThroughState;

/****************************************
* ��Ϣ���ͽṹ
*****************************************/
typedef enum _MessageType
{
	MsgTypeRegisterMsg = 0,		                                //ע����Ϣ
	MsgTypeUnregisterMsg,			                            //ע����Ϣ
	MsgTypeSubscribeMsg,			                            //������Ϣ
	MsgTypeUnSubscribeMsg,			                            //ȡ������
	MsgTypePublishMsg,				                            //������Ϣ
	MsgTypeReceiveMsg,				                            //������Ϣ
	MsgTypePublishPort,			                                //��ȡ����socket�Ķ˿ں�
	MsgTypeControlCmd,				                            //���Ϳ�����Ϣ
	MsgTypeRequestRTDase,			                            //ʵʱ����������
	MsgTypeControlResult,			                            //���ؿ��ƽ��
	MsgTypeRealLogResponse,		                                //��־�ظ�,ʵʱ��־����
	MsgTypeProcessData,			                                //���ù��ߺͽ��̹���ͨѶ
	MsgTypeProcessCtrl,			                                //���ù��߶Խ��̽��п���
	MsgTypeRealPointValue,			                            //ʵʱ��ֵ����
	MsgTypeHeartBeatMessage,		                            //�����ķ��� 
	MsgTypeAlarmData,				                            //�澯
    MsgTypeFile
}MessageType;

/****************************************
* ��Ϣ�����ȼ�  1~4����Ӹߵ���
*****************************************/
typedef enum _MessageLevel
{
	MsgLevel1 = 1,
	MsgLevel2,
	MsgLevel3,
	MsgLevel4

}MessageLevel;

/****************************************
* �ļ����䣬�ļ���Ϣ
*****************************************/
typedef struct _SendFileInfo
{
    char cFileName[256];			                            //�ļ�����
    int MsgIndex;                                               //�ļ��������š�
    int MsgCount;                                               //�ļ���������ܹ����͵İ�����
}SendFileInfo;

/****************************************
* ���һ�����յ�����Ϣ������
*****************************************/
typedef struct _MsgBuffer
{
    char PublishType[6];                                        //������Ϣ������
	int MsgLength;			                                    //��Ϣ����
    MessageLevel MsgLevel;	                                    //��Ϣ�����ȼ���Ŀǰ֧��1~4����
    SendFileInfo FileInfo;                                      //�ļ����䣬�ļ���Ϣ
	MessageType MsgType;                                        //��Ϣ������
	char *MsgCountext;		                                    //��Ϣ����
}MsgBuffer;

/****************************************
* ��������Ϣ
*****************************************/
typedef struct _ServerInfo
{
	std::string pServerAddr;	                                //�洢Ҫ������IP
	int    iReqRespPort;                                        //����Ӧ��˿�
	int    iPubSubPort;	                                        //�������Ķ˿�
}ServerInfo;

/****************************************
* 1���Ϳ�����Ϣ
*****************************************/
typedef struct _ControlCmd
{
	char channelName[255] ;		                                // ͨ������
	char iedName[255];                                          // �豸����
	char pointName[255];                                        // ������
	float  fValue;                                              // ���õ�ֵ
}ControlCmd;

/****************************************
* 2ʵʱ����������
*****************************************/
typedef struct _RequestRTDase
{
	char iedName[255];                                          // �豸���ƣ�Ĭ�Ͽ�Ϊ�����豸��
	int type;                                                   //���ͣ�����
}RequestRTDase;

/****************************************
* 3���ؿ��ƽ��
*****************************************/
typedef struct _ControlResult
{
	char channelName[255];		                               // ͨ������
	char iedName[255];                                         // �豸����
	char pointName[255];                                       // ������
	int    result;                                             // ���ؿ��ƵĽ��
}ControlResult;

/****************************************
* 4��־�ظ�,ʵʱ��־����
*****************************************/
typedef struct _RealLogResponse
{
    char channelName[255]; 	                                   //ͨ������
    char devName[255];   		                               //�豸����,û��Ϊ��
    char logDetail[255];  		                               //��־��ϸ����
	int type;    		                                       //����{0ȫ��, 1��ͨ��־��2������־ ,3��λ��Ϣ, 4��Ȩ��Ϣ} 
	int level;		                                           //�ȼ�{0ȫ��[ALL], 1��Ϣ[INFO], 2������Ϣ[DEBUG]��3������Ϣ[ERROR]��4�澯��Ϣ[WARN]}  û�еȼ���Ϊ0ȫ��  
	double logTime; 		                                   //��־����ʱ��(��������)

}RealLogResponse;

/****************************************
* 5���ù��ߺͽ��̹���ͨѶ 
*****************************************/
typedef struct _ProcessData {
	char process_id[255];	                                   //Ψһ��ʶ
	char process_path[255];	                                   //����·��
	char process_name[255];	                                   //��������
	int start_time;  	                                       //��ʼʱ��   (����)
	int process_pid;	                                       //����ID
	int process_status;	                                       //����״̬   0ֹͣ 1����  
	int heart_beat_time;	                                   //����ʱ��    (����)
	int restart_num; 	                                       //��������
    float cpu_use;		                                       //cpuʹ����
    float ram_use;		                                       //�ڴ�ʹ����

}ProcessData;

/****************************************
* 6���ù��߶Խ��̽��п���
*****************************************/
typedef struct _ProcessCtrl {
	char process_id[255];	                                   //Ψһ��ʶ
	int command; 		                                       //��������  0ֹͣ 1����
}ProcessCtrl;

/****************************************
* ʵʱ��ֵ����
*****************************************/
enum PointValueType
{
	VTYPE_RESERVE = 0,                                         //����
	VTYPE_STRING = 1,                                          //string
	VTYPE_INT32 = 2,                                           //int
	VTYPE_FLOAT = 3,                                           //float
	VTYPE_LONG = 4,                                            //long
	VTYPE_BOOL = 5,                                            //bool
	VTYPE_DOUBLE = 6,                                          //double
	VTYPE_QUALITY = 7,                                         //quality
	VTYPE_TIMESTAMP = 8,                                       //timestamp
	VTYPE_UNKOWN = 9                                           //unkown
};

/****************************************
* 7ʵʱ��ֵ���� 
*****************************************/
typedef struct _RealPointValue
{
	char channelName[255];   		                           //ͨ������
	char deviceName[255];     		                           //�豸����
	char pointValue[255];    		                           //ʵʱ��ֵ
	char pointAddr[255];			                           //��ɼ���ַ
	PointValueType valueType;		                           //��ֵ����
	int channelType;			                               //ͨ�����ͣ�1-�ɼ�  2-���ݿ⣬3-���֣�4-����
}RealPointValue;

/****************************************
* 8�����ķ���
*****************************************/
typedef struct _HeartBeatMessage
{
	char channelName[255];   		                           //ͨ������
	int time;                 
}HeartBeatMessage;

/****************************************
* 9�澯
*****************************************/
typedef struct _AlarmData
{
	char  dev[255];                                            //�澯������Դ�������������豸
	char  text[255];                                           //�澯���ݡ������ǵ�������Ҳ�������ɱ���ģ������ֵ��
	char  point[255];                                          //�澯�ĵ����� ����Դ�澯�Ļ���ֱ����дָ������
	char  datatime[255];                                       //�澯��ʱ��
    int   type;                                                //�澯���ͣ�1����Դ�澯��2Խ���ޣ�3Խ���ޣ�4��Խ�����ޣ�5��Խ������,6
    int   level;                                               //�澯����1��һ�㣬2�����أ�3������

}AlarmData;

/****************************************
* 10���ʽ���㹫ʽ
*****************************************/
typedef struct _DesignFormula
{
	char formula[255];                                        //��ʽ����
}DesignFormula;
#endif 