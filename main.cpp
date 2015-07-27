/*********************************************************************
* ��Ȩ���� (C)2015, �Ͼ�����ﰲ���Ƽ����޹�˾��

* �ļ����ƣ� main.cpp
* ����ժҪ�� �����������̺߳����߳�
* ��ǰ�汾�� 1.0
* ��    �ߣ� �����-��ƽ
* ������ڣ�
**********************************************************************/

#include  "Common.h"
#include "TcpCom.h"
#include  "XmlParser.h"

#include "VideoStream.h"


/* ������������� */
T_ServerParam 	t_ServerParam;  
 /* ��������Ϣ */
T_ServerMess  	t_ServerMess;

/* �����������ļ��� */
char ServerCfg[20] = "Server.xml";


void * rtsp_thread(void *param)
{
	VideoStream	 stream;
	stream.setUrl("rtsp://admin:123456@192.168.1.252:554/mpeg4cif");
	stream.startStream();
	stream.Init();
	Mat dispalyFrame;
	while(1){
		stream.play();
		stream.GetReadFrame().copyTo(dispalyFrame);
		if(dispalyFrame.empty()) continue;
		imshow("dispalyFrame",dispalyFrame);
		waitKey(1);
	}
}

int main(int argc,char**argv)
{

	int iRet = -1;
/*	
	pthread_t client_pthread_ID;
	
	memset(&t_ServerParam,0,sizeof(T_ServerParam));
	memset(&t_ServerMess,0,sizeof(T_ServerMess));


	CXmlParser* Xmlparser  = new CXmlParser(ServerCfg);
	
	Xmlparser->GetServerParam(t_ServerParam);
	
	iRet = pthread_create(&client_pthread_ID,NULL,tcp_client_thread,NULL);  
	if(iRet != 0){
		 dbgprint("%s(%d),create client_thread error!\n",DEBUGARGS);
		 return -1;
	} 
	pthread_detach(client_pthread_ID);
	
*/	
	pthread_t rtsp_pthread_ID;

	iRet = pthread_create(&rtsp_pthread_ID,NULL,rtsp_thread,NULL);	
		if(iRet != 0){
			 dbgprint("%s(%d),create rtsp_pthread error!\n",DEBUGARGS);
			 return -1;
		} 
		pthread_detach(rtsp_pthread_ID);

	while(1){	
		sleep(5);
	}	
	
//	delete  Xmlparser;
	
	return 0;
}


