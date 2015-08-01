#ifndef _CMDDEFINE_H_
#define	_CMDDEFINE_H_

#include "Common.h"

#pragma pack(push)
#pragma pack(1)

#define T_PACKETHEAD_MAGIC	0xfefefefe

#define  IP_LEN_16        16
#define  USERNAME_LEN_10  10
#define  PASSWORD_LEN_10  10
#define  WEEK_DAY_LEN_7   7

#define  CAM_MAX_LEN   20
#define  SINGLE_URI_LEN_128  128

typedef struct COMMON_PACKET_HEAD
{
	uint32	magic;					   //head
	uint16  encrypt;				   //Encryption type
	uint16	cmd;					     //CMD ID
	uint32  EncryptLen;				 //Data packets Len - encrypted
	uint32  UnEncryptLen;			 //Data packets Len - Before the encryption
	uint32	CompressedLen;		 //The length of the compressed packet
	uint32	UnCompressedLen;	 //UnCompressed packet length
	uint16	chksum;					   //The checksum
	uint16	unused;					   //unused

	COMMON_PACKET_HEAD(){
		memset(this, 0, sizeof(COMMON_PACKET_HEAD));
	}

} T_PacketHead,*PT_PacketHead;


/*********************city Server interaction*******************************/

enum ANAY_VDCS_CMD {  				/* city Server interaction CMD */
	SM_ANAY_VDCS_REGISTER = 0x0400,
	SM_VDCS_ANAY_REGISTER_ACK,

	SM_ANAY_VDCS_DEVICE_STATUS ,
	SM_VDCS_ANAY_DEVICE_STATUS_ACK,

	SM_ANAY_VDCS_WARN_INFO,
	SM_VDCS_ANAY_WARN_INFO_ACK,

	SM_VDCS_ANAY_REGISTER_PUSH_CAMERA,   // REGISTER push camera if none means no camera
	SM_VDCS_ANAY_PUSH_CAMERA_TYPE,       // set type and push
	SM_VDCS_ANAY_PUSH_CAMERA_PARAM,      // set param and push

	SM_ANAY_HEATBEAT = 0X8003
};

enum DeviceType {      			/* DeviceType */
	DeviceTypeNetCamera = 1,	//IPCAMERA
	DeviceTypeWarn,
	DeviceTypeLock,
	DeviceTypeGuard,
	DeviceTypeSpeak,
	DeviceTypeWork,
	DeviceTypeYA,
	DeviceTypeMcu,
	DeviceTypeNetControl,
	DeviceTypeRecorder,
};

#define WarnNumber    7 
enum WarnType  /* WarnType */
{
	HumanDetect 	=	0x0001,
	SmokeDetect 	=	0x0002,
	RegionDetect 	= 0x0004,
	FixedObjDetect= 0x0008,
	FireDetect 		= 0x0010,
	ResidueDetect =	0x0020,
	GenderDetect  = 0x0040  //????????????????????
};

typedef struct _SM_ANAY_VDCS_REGISTER
{
	uint32 Position;  // Analyze Server Position
	_SM_ANAY_VDCS_REGISTER(){
		memset(this, 0, sizeof(_SM_ANAY_VDCS_REGISTER));
	}
} ST_SM_ANAY_VDCS_REGISTER;

typedef struct _SM_VDCS_ANAY_REGISTER_ACK
{
	uint8 	Ack;    // 0 sucess / 1 faild
	_SM_VDCS_ANAY_REGISTER_ACK(){
		memset(this, 0, sizeof(_SM_VDCS_ANAY_REGISTER_ACK));
	}
} ST_SM_VDCS_ANAY_REGISTER_ACK;

typedef struct _SM_ANAY_VDCS_DEVICE_STATUS
{
	char    ip[IP_LEN_16];
	uint8 	DeviceType;
	uint8 	status;             // 0 break 1 reconnect ok

	_SM_ANAY_VDCS_DEVICE_STATUS(){
		memset(this, 0, sizeof(_SM_ANAY_VDCS_DEVICE_STATUS));
	}

}ST_SM_ANAY_VDCS_DEVICE_STATUS;

typedef struct _ALARM_TIME{

	char   StartTime[6];
	char   EndTime[6];

	_ALARM_TIME(){
		memset(this, 0, sizeof(_ALARM_TIME));
	}
}ALARM_TIME;

typedef struct _VIDEO_ALARM_TIME{

	ALARM_TIME time1;
	ALARM_TIME time2;
	ALARM_TIME time3;

	_VIDEO_ALARM_TIME(){
		memset(this, 0, sizeof(_VIDEO_ALARM_TIME));
	}
}VIDEO_ALARM_TIME;

typedef struct _SM_VDCS_VIDEO_ALARM_TIME{

	uint8   Enable;
	uint8   Day;
	VIDEO_ALARM_TIME AlarmTime;

	_SM_VDCS_VIDEO_ALARM_TIME(){
		memset(this, 0, sizeof(_SM_VDCS_VIDEO_ALARM_TIME));
	}
}ST_SM_VDCS_VIDEO_ALARM_TIME;

//followed ST_SM_VDCS_VIDEO_REGISTER_PUSH_CAM / ST_SM_VDCS_VIDEO_FUNC_PARAM
typedef struct _VIDEO_DRAW  // pkg means special parameter
{
	uint16 	StartX;				//StartX
	uint16 	StartY;				//StartY
	uint16 	EndX;					//EndX    if draw is rectangle  means width
	uint16 	EndY;					//EndY    if draw is rectangle  means height
	uint16  Type;					//1 monitor 2 Inward and outward 3 region

	_VIDEO_DRAW(){
		memset(this, 0, sizeof(_VIDEO_DRAW));
	}
}VIDEO_DRAW;

//SM_VDCS_ANAY_REGISTER_PUSH_CAMERA
typedef struct  _SM_VDCS_VIDEO_REGISTER_PUSH_CAM{

		char        ip[IP_LEN_16];
		char        username[USERNAME_LEN_10];
		char        password[PASSWORD_LEN_10];
		char   	    CameUri[SINGLE_URI_LEN_128];
		//uint8       Enable;     /* if disable should not send */
		uint8       AnalyzeNUM;   /* 1/2  other is wrong */
		uint16      AnalyzeType;

		_SM_VDCS_VIDEO_REGISTER_PUSH_CAM(){
			memset(this, 0, sizeof(_SM_VDCS_VIDEO_REGISTER_PUSH_CAM));
		}
}ST_SM_VDCS_VIDEO_REGISTER_PUSH_CAM;

typedef struct  _SM_VDCS_VIDEO_REGISTER_PUSH_CAM_ANALYZE_PARAM{

	uint16      AnalyzeType;
	ST_SM_VDCS_VIDEO_ALARM_TIME  AlarmTime[WEEK_DAY_LEN_7];
	uint16      MaxHumanNum;    /* HumanDetect needs */
	float       ChangRate;      /* RegionDetect needs */
	uint16      PkgNum;         /* structure refer to VIDEO_DRAW */

	_SM_VDCS_VIDEO_REGISTER_PUSH_CAM_ANALYZE_PARAM(){
		memset(this, 0, sizeof(_SM_VDCS_VIDEO_REGISTER_PUSH_CAM_ANALYZE_PARAM));
	}
}ST_SM_VIDEO_PUSH_CAM_ANALYZE_PARAM;


//SM_VDCS_ANAY_PUSH_CAMERA_TYPE
typedef struct  _SM_VDCS_VIDEO_CAM_TYPE{

		char        ip[IP_LEN_16];
		char        username[USERNAME_LEN_10];
		char        password[PASSWORD_LEN_10];
		char   	    CameUri[SINGLE_URI_LEN_128];
		uint8       Enable;   /* 0 disable / 1 enable */
		uint8       AnalyzeNUM;
		uint16      AnalyzeType;

		_SM_VDCS_VIDEO_CAM_TYPE(){
			memset(this, 0, sizeof(_SM_VDCS_VIDEO_CAM_TYPE));
		}
}ST_SM_VDCS_VIDEO_CAM_TYPE;

//SM_VDCS_ANAY_PUSH_CAMERA_PARAM
typedef struct _SM_VDCS_VIDEO_FUNC_PARAM{

	char        ip[IP_LEN_16];
	uint16    	AlarmType;      /* AlarmType refer to enum WarnType */
	//uint8     	AutoAlarm;    /* 0 manual mode  / 1 Automate mode */
	ST_SM_VDCS_VIDEO_ALARM_TIME  AlarmTime[WEEK_DAY_LEN_7];
	uint16      MaxHumanNum;    /* HumanDetect needs */
	float       ChangRate;      /* RegionDetect needs */
	uint16      PkgNum;         /* structure refer to VIDEO_DRAW */

	_SM_VDCS_VIDEO_FUNC_PARAM(){
		memset(this, 0, sizeof(_SM_VDCS_VIDEO_FUNC_PARAM));
	}
}ST_SM_VDCS_VIDEO_FUNC_PARAM;

#pragma pack(pop)

#endif
