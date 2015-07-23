#ifndef    _CMDDEFINE_H_
#define	_CMDDEFINE_H_

#include "Common.h"

#pragma pack(push) 
#pragma pack(1)

#define T_PACKETHEAD_MAGIC	0xfefefefe

#define SINGLE_URL_LEN_128  128
#define MCU_MAC_LEN_32  32
//包头定义
typedef struct COMMON_PACKET_HEAD 
{
	uint32	magic;					//包头识别码
	uint16      encrypt;				       //加密类型 MsgEncryptType
	uint16	cmd;					//命令ID
	uint32      EncryptLen;				//传输数据包(包体部分)总长度（加密后）
	uint32      UnEncryptLen;			//加密前数据包(包体部分)长度
	uint32	CompressedLen;			//压缩后数据包体长度
	uint32	UnCompressedLen;		//未压缩包体长
	uint16	chksum;					//校验和
	uint16	unused;					//保留

} T_PacketHead,*PT_PacketHead;


////////////////////////////////////////////////////////服务器//////////////////////////////////////////////////////////////////

enum{
	// 分析服务器与处理服务器的通讯命令
	
	SM_ANAY_VDCS_REGISTER = 0x0400,      
	SM_VDCS_ANAY_REGISTER_ACK,  
	
	SM_VDCS_ANAY_DEVICE_CONTROL,        // 控制电控锁 报警器等
	SM_ANAY_VDCS_DEVICE_CONTROL_ACK, 
	SM_ANAY_VDCS_ROUTING_INSPECTION,   
	SM_VDCS_ANAY_ROUTING_INSPECTION_ACK,
	
	/* 设备故障报告 */
	SM_ANAY_VDCS_DEVICE_STATUS =  0x0406,         
	SM_VDCS_ANAY_DEVICE_STATUS_ACK,
	
	SM_ANAY_VDCS_WARN_INFO,              // 分析服务器达到报警最大人数发送
	SM_VDCS_ANAY_WARN_INFO_ACK,
	
	SM_VDCS_MCU_PUSH_MAX_COUNT,   //  no used
	
	SM_VDCS_ANAY_PUSH_CAMERA_PARAM,   
	
	SM_VDCS_ANAY_RENEW_REGISTER_MCU,
	SM_VDCS_ANAY_RENEW_REGISTER_MCU_ACK,
	SM_VDCS_ANAY_SET_CAMERA_PARAM,


	SM_ANAY_HEATBEAT = 0X8003

};

enum DeviceType
{
	DeviceTypeNetCamera = 1,	//网络摄像机
	DeviceTypeWarn,				//报警器
	DeviceTypeLock,				//报警器
	DeviceTypeGuard,			//门禁
	DeviceTypeSpeak,			//对讲
	DeviceTypeWork,				//考勤 
	DeviceTypeYA,				//考勤 
	DeviceTypeMcu,				//MCU
	DeviceTypeNetControl,		//遥控器
	DeviceTypeRecorder,			//遥控器
};


enum WarnType
{
	HumanDetect =0x0001,
	SmokeDetect,
	AlarmRegionDetect,
	FixedObjDetect,
	FireDetect
};

typedef struct _SM_VDCS_VIDEO_DRAW
{
	uint16 	StartX;					//起始X坐标
	uint16 	StartY;					//起始Y坐标
	uint16 	EndX;					//终止X坐标	如果是矩形,则代表宽度
	uint16 	EndY;					//终止Y坐标  如果是矩形,则代表高度
	uint16     Type;					//类型：1监测区，2出入口，3警戒区	(1、3矩形， 2线)

}ST_SM_VDCS_VIDEO_DRAW;


typedef struct _SM_VDCS_VIDEO_ALARM_TIME{

	uint8   Enable;    
	uint8   Day;
	char   StartTime[6];
	char   EndTime[6];
	
}ST_SM_VDCS_VIDEO_ALARM_TIME;

typedef struct _SM_VDCS_VIDEO_FUNC_PARAM{

	char   	CameUrl[SINGLE_URL_LEN_128];
	uint16    	AlarmType;
	uint8     	EnableAlarm;
	uint8     	AutoAlarm;
	ST_SM_VDCS_VIDEO_ALARM_TIME  AlarmTime[7];
	uint16      MaxHumanNum;
	uint16      PkgNum;

	_SM_VDCS_VIDEO_FUNC_PARAM(){
		memset(this, 0, sizeof(_SM_VDCS_VIDEO_FUNC_PARAM));
	}
	
}ST_SM_VDCS_VIDEO_FUNC_PARAM;

typedef struct _SM_ANAY_VDCS_REGISTER
{
	char 	MCUAddr[MCU_MAC_LEN_32];
	
} ST_SM_ANAY_VDCS_REGISTER;



#pragma pack(pop)

#endif

