
#include  "Common.h"
#include  "TcpCom.h"
#include  "XmlParser.h"
#include  "Analyze.h"
#include  "VideoStream.h"

/* city server parameters */
T_ServerParam 	t_ServerParam;
/* city parameters info */
T_ServerMess  	t_ServerMess;
/* Analyze position */
uint32 Position = 0;
/* city server config xml file */
char ServerCfg[20] = "Server.xml";

T_SINGLE_CAMERA t_camera[CAM_MAX_LEN] ;

int main(int argc,char**argv)
{
	int iRet = -1;
	uint8 iNum = 0;

	pthread_t client_pthread_ID;

	memset(&t_ServerParam,0,sizeof(T_ServerParam));
	memset(&t_ServerMess,0,sizeof(T_ServerMess));
	for(iNum = 0; iNum < CAM_MAX_LEN; iNum++){
		memset(&t_camera[iNum], 0 ,sizeof(T_SINGLE_CAMERA));
	}

	CXmlParser* Xmlparser  = new CXmlParser(ServerCfg);
	Xmlparser->GetServerParam(t_ServerParam);
	Xmlparser->GetAnalyzeParam(Position);

	iRet = pthread_create(&client_pthread_ID,NULL,tcp_client_thread,NULL);
	if(iRet != 0){
		 dbgprint("%s(%d),create client_thread error!\n",DEBUGARGS);
		 return -1;
	}
	pthread_detach(client_pthread_ID);

	while(1){
		sleep(5);
	}

	delete  Xmlparser;
	return 0;
}
