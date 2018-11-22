#ifndef __STRUCTBASE_H_INCLUDED__
#define __STRUCTBASE_H_INCLUDED__

#include <string>

//程序初始化失败返回值
#define INIT_CONTEXT_ERROR							-1001                       //初始化消息总线上下文失败
#define SET_CONTEXT_SET_ERROR						-1002		                //初始化网络设置失败
#define INIT_REQUEST_RESPONSE_SOCKET_ERROR			-1003		                //创建请求应答socket失败
#define INIT_REQUEST_RESPONSE_CONNECT_ERROR			-1004		                //链接请求应答失败
#define INIT_PUBLISH_SUBSCRIBE_SOCKET_EREOR			-1005		                //创建发布订阅socket失败
#define INIT_PUBLISH_SUBSCRIBE_CONNECT_ERROR		-1006		                //链接发布订阅失败
#define GET_PUBLISH_PORT_ERROR						-1007		                //从服务器中获得发布socket的端口
#define RECV_MESSAGE_EVENT_ERROR					-1008		                //创建接受数据线程共享数据事件
#define INIT_PUBLISH_SUBSCRIBE_BIND_ERROR			-1009		                //订阅发布绑定错误
#define INIT_REQUEST_RESPONSE_BIND_ERROR			-1010		                //请求应答绑定错误
#define FIXED_LENGTH_MESSAGE						1024*60                     //发送和接受固定消息的长度
#define FIXED_LENGTH_MESSAGE_TOP					sizeof(MsgBuffer)			//发送和接受消息的消息头长度
#define FIXED_OVERALL_LENGTH_MESSAGE                FIXED_LENGTH_MESSAGE + FIXED_LENGTH_MESSAGE_TOP

/****************************************
 *此类用于客户端调用和服务器连接
****************************************/
typedef enum ThroughState{
	RealState = 0,		                                        //实时态
	InversionState,		                                        //反演态
	ResearchState,		                                        //研究态
	TestState			                                        //测试态
}ThroughState;

/****************************************
* 消息类型结构
*****************************************/
typedef enum _MessageType
{
	MsgTypeRegisterMsg = 0,		                                //注册消息
	MsgTypeUnregisterMsg,			                            //注销消息
	MsgTypeSubscribeMsg,			                            //订阅消息
	MsgTypeUnSubscribeMsg,			                            //取消订阅
	MsgTypePublishMsg,				                            //发布消息
	MsgTypeReceiveMsg,				                            //接受消息
	MsgTypePublishPort,			                                //获取发布socket的端口号
	MsgTypeControlCmd,				                            //发送控制消息
	MsgTypeRequestRTDase,			                            //实时库数据请求
	MsgTypeControlResult,			                            //返回控制结果
	MsgTypeRealLogResponse,		                                //日志回复,实时日志接收
	MsgTypeProcessData,			                                //配置工具和进程管理通讯
	MsgTypeProcessCtrl,			                                //配置工具对进程进行控制
	MsgTypeRealPointValue,			                            //实时点值更新
	MsgTypeHeartBeatMessage,		                            //心跳的发送 
	MsgTypeAlarmData,				                            //告警
    MsgTypeFile
}MessageType;

/****************************************
* 消息的优先级  1~4级别从高到低
*****************************************/
typedef enum _MessageLevel
{
	MsgLevel1 = 1,
	MsgLevel2,
	MsgLevel3,
	MsgLevel4

}MessageLevel;

/****************************************
* 文件传输，文件信息
*****************************************/
typedef struct _SendFileInfo
{
    char cFileName[256];			                            //文件名称
    int MsgIndex;                                               //文件传输的序号。
    int MsgCount;                                               //文件传输过程总共发送的包数。
}SendFileInfo;

/****************************************
* 存放一次性收到的消息的内容
*****************************************/
typedef struct _MsgBuffer
{
    char PublishType[6];                                        //发布消息的类型
	int MsgLength;			                                    //消息长度
    MessageLevel MsgLevel;	                                    //消息的优先级。目前支持1~4级；
    SendFileInfo FileInfo;                                      //文件传输，文件信息
	MessageType MsgType;                                        //消息的类型
	char *MsgCountext;		                                    //消息内容
}MsgBuffer;

/****************************************
* 服务器信息
*****************************************/
typedef struct _ServerInfo
{
	std::string pServerAddr;	                                //存储要服务器IP
	int    iReqRespPort;                                        //请求应答端口
	int    iPubSubPort;	                                        //发布订阅端口
}ServerInfo;

/****************************************
* 1发送控制消息
*****************************************/
typedef struct _ControlCmd
{
	char channelName[255] ;		                                // 通道名称
	char iedName[255];                                          // 设备名称
	char pointName[255];                                        // 点名称
	float  fValue;                                              // 设置的值
}ControlCmd;

/****************************************
* 2实时库数据请求
*****************************************/
typedef struct _RequestRTDase
{
	char iedName[255];                                          // 设备名称，默认空为所有设备的
	int type;                                                   //类型，备用
}RequestRTDase;

/****************************************
* 3返回控制结果
*****************************************/
typedef struct _ControlResult
{
	char channelName[255];		                               // 通道名称
	char iedName[255];                                         // 设备名称
	char pointName[255];                                       // 点名称
	int    result;                                             // 返回控制的结果
}ControlResult;

/****************************************
* 4日志回复,实时日志接收
*****************************************/
typedef struct _RealLogResponse
{
    char channelName[255]; 	                                   //通道名称
    char devName[255];   		                               //设备名称,没有为空
    char logDetail[255];  		                               //日志详细内容
	int type;    		                                       //类型{0全部, 1普通日志，2报文日志 ,3变位信息, 4授权信息} 
	int level;		                                           //等级{0全部[ALL], 1信息[INFO], 2调试信息[DEBUG]，3错误信息[ERROR]，4告警信息[WARN]}  没有等级，为0全部  
	double logTime; 		                                   //日志产生时间(毫秒秒数)

}RealLogResponse;

/****************************************
* 5配置工具和进程管理通讯 
*****************************************/
typedef struct _ProcessData {
	char process_id[255];	                                   //唯一标识
	char process_path[255];	                                   //进程路径
	char process_name[255];	                                   //进程名称
	int start_time;  	                                       //开始时间   (秒数)
	int process_pid;	                                       //进程ID
	int process_status;	                                       //进程状态   0停止 1运行  
	int heart_beat_time;	                                   //心跳时间    (秒数)
	int restart_num; 	                                       //重启次数
    float cpu_use;		                                       //cpu使用率
    float ram_use;		                                       //内存使用率

}ProcessData;

/****************************************
* 6配置工具对进程进行控制
*****************************************/
typedef struct _ProcessCtrl {
	char process_id[255];	                                   //唯一标识
	int command; 		                                       //控制命令  0停止 1重启
}ProcessCtrl;

/****************************************
* 实时点值类型
*****************************************/
enum PointValueType
{
	VTYPE_RESERVE = 0,                                         //保留
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
* 7实时点值更新 
*****************************************/
typedef struct _RealPointValue
{
	char channelName[255];   		                           //通道名称
	char deviceName[255];     		                           //设备名称
	char pointValue[255];    		                           //实时点值
	char pointAddr[255];			                           //点采集地址
	PointValueType valueType;		                           //点值类型
	int channelType;			                               //通道类型，1-采集  2-数据库，3-网分，4-备用
}RealPointValue;

/****************************************
* 8心跳的发送
*****************************************/
typedef struct _HeartBeatMessage
{
	char channelName[255];   		                           //通道名称
	int time;                 
}HeartBeatMessage;

/****************************************
* 9告警
*****************************************/
typedef struct _AlarmData
{
	char  dev[255];                                            //告警数据来源，或是所属的设备
	char  text[255];                                           //告警内容。可以是点描述，也可以是由报警模块所赋值。
	char  point[255];                                          //告警的点名称 ，资源告警的话，直接填写指标名称
	char  datatime[255];                                       //告警的时间
    int   type;                                                //告警类型，1、资源告警。2越上限，3越下限，4、越上上限，5、越下下限,6
    int   level;                                               //告警级别：1、一般，2、严重，3、致命

}AlarmData;

/****************************************
* 10表达式计算公式
*****************************************/
typedef struct _DesignFormula
{
	char formula[255];                                        //公式内容
}DesignFormula;
#endif 