#ifndef    _CMDDEFINE_H_
#define	_CMDDEFINE_H_

#include "Common.h"

#pragma pack(push) 
#pragma pack(1)

#define T_PACKETHEAD_MAGIC	0xfefefefe

#define SINGLE_URL_LEN_128  128
#define MCU_MAC_LEN_32  32
//��ͷ����
typedef struct COMMON_PACKET_HEAD 
{
	uint32	magic;					//��ͷʶ����
	uint16      encrypt;				       //�������� MsgEncryptType
	uint16	cmd;					//����ID
	uint32      EncryptLen;				//�������ݰ�(���岿��)�ܳ��ȣ����ܺ�
	uint32      UnEncryptLen;			//����ǰ���ݰ�(���岿��)����
	uint32	CompressedLen;			//ѹ�������ݰ��峤��
	uint32	UnCompressedLen;		//δѹ�����峤
	uint16	chksum;					//У���
	uint16	unused;					//����

} T_PacketHead,*PT_PacketHead;


////////////////////////////////////////////////////////������//////////////////////////////////////////////////////////////////

enum{
	// �����������봦���������ͨѶ����
	
	SM_ANAY_VDCS_REGISTER = 0x0400,      
	SM_VDCS_ANAY_REGISTER_ACK,  
	
	SM_VDCS_ANAY_DEVICE_CONTROL,        // ���Ƶ���� ��������
	SM_ANAY_VDCS_DEVICE_CONTROL_ACK, 
	SM_ANAY_VDCS_ROUTING_INSPECTION,   
	SM_VDCS_ANAY_ROUTING_INSPECTION_ACK,
	
	/* �豸���ϱ��� */
	SM_ANAY_VDCS_DEVICE_STATUS =  0x0406,         
	SM_VDCS_ANAY_DEVICE_STATUS_ACK,
	
	SM_ANAY_VDCS_WARN_INFO,              // �����������ﵽ���������������
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
	DeviceTypeNetCamera = 1,	//���������
	DeviceTypeWarn,				//������
	DeviceTypeLock,				//������
	DeviceTypeGuard,			//�Ž�
	DeviceTypeSpeak,			//�Խ�
	DeviceTypeWork,				//���� 
	DeviceTypeYA,				//���� 
	DeviceTypeMcu,				//MCU
	DeviceTypeNetControl,		//ң����
	DeviceTypeRecorder,			//ң����
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
	uint16 	StartX;					//��ʼX����
	uint16 	StartY;					//��ʼY����
	uint16 	EndX;					//��ֹX����	����Ǿ���,�������
	uint16 	EndY;					//��ֹY����  ����Ǿ���,�����߶�
	uint16     Type;					//���ͣ�1�������2����ڣ�3������	(1��3���Σ� 2��)

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

