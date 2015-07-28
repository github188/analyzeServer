
#include  "Common.h"
#include  "TcpCom.h"
#include  "XmlParser.h"
#include  "Analyze.h"
#include  "VideoStream.h"

/* city server parameters */
T_ServerParam 	t_ServerParam;
/* city parameters info */
T_ServerMess  	t_ServerMess;

/* city server config xml file */
char ServerCfg[20] = "Server.xml";

T_SINGLE_CAMERA t_camera[CAM_MAX_LEN];

/*
void * rtsp_thread(void *param)
{
	VideoStream	 stream;
	stream.setUrl("rtsp://admin:123456@192.168.1.252:554/mpeg4cif");
	stream.startStqream();
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
*/
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

/*
	pthread_t rtsp_pthread_ID;
	iRet = pthread_create(&rtsp_pthread_ID,NULL,rtsp_thread,NULL);
		if(iRet != 0){
			 dbgprint("%s(%d),create rtsp_pthread error!\n",DEBUGARGS);
			 return -1;
		}
		pthread_detach(rtsp_pthread_ID);
*/

/*  	//Thread t(new CAnaThread(1));
	try{
        Thread t(new Counter());

  }  catch(Synchronization_Exception& e)
    {
        cerr << e.what() <<endl;
    }

*/

/*
	try{
		CAnaThread *ana =new CAnaThread(1);
		Thread t(ana);
		sleep(50);
		ana->quit();

	}catch (Synchronization_Exception&  e)  {
	        cerr << e.what() <<endl;
	}
*/

	while(1){
		sleep(4);
	}

	//delete  Xmlparser;

	return 0;
}
