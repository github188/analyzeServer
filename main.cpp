/*********************************************************************
* 版权所有 (C)2015, 南京亨润达安防科技有限公司。

* 文件名称： main.cpp
* 内容摘要： 创建服务器线程和主线程
* 当前版本： 1.0
* 作    者： 亨润达-陈平
* 完成日期：
**********************************************************************/

#include  "Common.h"
#include "TcpCom.h"
#include  "XmlParser.h"


/* 服务器网络参数 */
T_ServerParam 	t_ServerParam;  
 /* 服务器信息 */
T_ServerMess  	t_ServerMess;

/* 服务器参数文件名 */
char ServerCfg[20] = "Server.xml";

int main(int argc,char**argv)
{

	int iRet = -1;
	
	pthread_t client_pthread_ID;
	
	memset(&t_ServerParam,0,sizeof(T_ServerParam));
	memset(&t_ServerMess,0,sizeof(T_ServerMess));

	CXmlParser* Xmlparser  = new CXmlParser(ServerCfg);
	Xmlparser->GetServerParam(t_ServerParam);

	iRet = pthread_create(&client_pthread_ID,NULL,tcp_client_thread,NULL);
	if(iRet != 0){
		 printf("create client_thread error!\n");
		 return -1;
	} 
	pthread_detach(client_pthread_ID);

	while(1){	
		sleep(5);
	}	
	
	delete  Xmlparser;
	
	return 0;
}


