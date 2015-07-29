#include "SerData.h"

extern T_ServerMess  	t_ServerMess;
extern T_SINGLE_CAMERA t_camera[CAM_MAX_LEN] ;

int report_cam_break(uint8 index);
{
  T_PacketHead			 				    t_PackHeadRepStatus;
  ST_SM_ANAY_VDCS_DEVICE_STATUS t_DeviceStatus;
  char RepStatusBuff[28 + 18] = {0};
  t_PackHeadAnayReg.magic         = T_PACKETHEAD_MAGIC;
	t_PackHeadAnayReg.cmd	         	= SM_ANAY_VDCS_DEVICE_STATUS;
	t_PackHeadAnayReg.UnEncryptLen  = sizeof(ST_SM_ANAY_VDCS_DEVICE_STATUS);

  memcpy(t_DeviceStatus.ip,t_camera[index].t_Camfixedparam.ip,IP_LEN_16);
  t_DeviceStatus.DeviceType = DeviceTypeNetCamera;
  t_DeviceStatus.status     = 0;

  memcpy(RepStatusBuff,&t_PackHeadRepStatus,sizeof(T_PacketHead));
	memcpy(RepStatusBuff+sizeof(T_PacketHead),&t_DeviceStatus,sizeof(ST_SM_ANAY_VDCS_DEVICE_STATUS));
	iRet = send(t_ServerMess.iClientFd,RepStatusBuff,sizeof(RepStatusBuff),0);
	if(iRet < 0){
		dbgprint("%s(%d),report cam break to server error!\n",DEBUGARGS);
		return -1;
	}
	dbgprint("%s(%d),report cam break to server ,total %d bytes!\n",DEBUGARGS,iRet);
	return 0;
}

int report_cam_reconnect(uint8 index)
{
  T_PacketHead			 				    t_PackHeadRepStatus;
  ST_SM_ANAY_VDCS_DEVICE_STATUS t_DeviceStatus;
  char RepStatusBuff[28 + 18] = {0};
  t_PackHeadAnayReg.magic         = T_PACKETHEAD_MAGIC;
  t_PackHeadAnayReg.cmd	         	= SM_ANAY_VDCS_DEVICE_STATUS;
  t_PackHeadAnayReg.UnEncryptLen  = sizeof(ST_SM_ANAY_VDCS_DEVICE_STATUS);

  memcpy(t_DeviceStatus.ip,t_camera[index].t_Camfixedparam.ip,IP_LEN_16);
  t_DeviceStatus.DeviceType = DeviceTypeNetCamera;
  t_DeviceStatus.status     = 1;

  memcpy(RepStatusBuff,&t_PackHeadRepStatus,sizeof(T_PacketHead));
  memcpy(RepStatusBuff+sizeof(T_PacketHead),&t_DeviceStatus,sizeof(ST_SM_ANAY_VDCS_DEVICE_STATUS));
  iRet = send(t_ServerMess.iClientFd,RepStatusBuff,sizeof(RepStatusBuff),0);
  if(iRet < 0){
    dbgprint("%s(%d),report cam re connect to server error!\n",DEBUGARGS);
    return -1;
  }
  dbgprint("%s(%d),report cam re connect to server ,total %d bytes!\n",DEBUGARGS,iRet);
  return 0;
}

static int push_camera_process(char* buffer,int len,PT_TcpClient pt_TcpClient,
  ST_SM_VDCS_VIDEO_REGISTER_PUSH_CAM* pt_PushCam,uint8 iNum)
{
  int iRet = -1;

  memset(&t_camera[iNum].t_Camfixedparam, 0 ,sizeof(T_CAM_FIXED_PARAM));
  memcpy(t_camera[iNum].t_Camfixedparam.username,pt_PushCam->username,IP_LEN_16);
  memcpy(t_camera[iNum].t_Camfixedparam.password,pt_PushCam->password,PASSWORD_LEN_10);
  memcpy(t_camera[iNum].t_Camfixedparam.CameUri,pt_PushCam->CameUri,SINGLE_URI_LEN_128);
  t_camera[iNum].t_Camfixedparam.index = iNum;

  t_camera[iNum].CamAnalyze = new Canalyze(t_camera[iNum].t_Camfixedparam);
  iRet = t_camera[iNum].CamAnalyze->m_Camera->GetCamParam();  // try to open camera
  if(iRet < 0) {
      delete  t_camera[iNum].CamAnalyze;
      return -1;
  }
  // open camera sucess
  t_camera[iNum].CamAnalyze->m_Camera->run(); // open camera read thread
  //open 2 alarm  / 1 time thread and hang on
  return 0;
}

static int camera_process(char* buffer,int len,PT_TcpClient pt_TcpClient)
{
  uint8 iNum = 0;
  int iRet = -1;
  ST_SM_VDCS_VIDEO_REGISTER_PUSH_CAM   t_PushCam;
  memcpy(&t_PushCam,buffer,sizeof(ST_SM_VDCS_VIDEO_REGISTER_PUSH_CAM));

  if( 1 == t_PushCam.AnalyzeNUM | 2 == t_PushCam.AnalyzeNUM){
      for(iNum = 0; iNum < CAM_MAX_LEN; iNum++){
          if(0 == t_camera[iNum].Enable) break;
      }
      iRet = push_camera_process(buffer,len,pt_TcpClient,&t_PushCam,iNum);
      if (iRet < 0) return -1;
      //  read thread is on

  }else{
      dbgprint("%s(%d),city server push camera AnalyzeNUM wrong!\n",DEBUGARGS);
      return -1;
  }
  return 0;
}

static int anay_re_register_ack(char* buffer,PT_TcpClient pt_TcpClient)
{
	ST_SM_VDCS_ANAY_REGISTER_ACK  t_AnayReRegAck;
	uint8 ack;

	memcpy(&t_AnayReRegAck,buffer,sizeof(ST_SM_VDCS_ANAY_REGISTER_ACK));

	ack = t_AnayReRegAck.Ack;
	if(ack == 0){
      dbgprint("%s(%d),anay register sucess!\n",DEBUGARGS);
			t_ServerMess.iAnayRegister = 1;
			return 0;
	}else{
      dbgprint("%s(%d),anay register failed!\n",DEBUGARGS);
      t_ServerMess.iAnayRegister = 0;
			return -1;
	}
	return 0;
}
/**********************************************************************
* function name: server_data_analyse
* Description  : analyze city server data
* Author       : 2015/7/29   by nikola
***********************************************************************/
int server_data_analyse(char* buffer,int len,PT_TcpClient pt_TcpClient)
{
    uint16 cmd;
  	T_PacketHead   t_packet_head;

  	if(len < PACKET_HEAD_LEN){
       dbgprint("%s(%d),receive error!\n",DEBUGARGS);
  		 return -1;
  	}

  	memcpy(&t_packet_head,buffer,PACKET_HEAD_LEN);
  	cmd = t_packet_head.cmd;

  	switch (cmd ){
  		case SM_VDCS_ANAY_REGISTER_PUSH_CAMERA:
  		     camera_process(buffer+PACKET_HEAD_LEN,len-PACKET_HEAD_LEN,pt_TcpClient);
  			   break;
  		case SM_VDCS_ANAY_REGISTER_ACK:  // anay register ack
  			   anay_register_ack(buffer+PACKET_HEAD_LEN,pt_TcpClient);
  		     break;

  		case  SM_ANAY_HEATBEAT:
  			//process_heart_beat(instance);
  			break;
  		default: break;
  	}
  	dbgprint("server cmd = %x\n", cmd);
  	return 0;
}
