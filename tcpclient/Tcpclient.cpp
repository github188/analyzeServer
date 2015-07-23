
#include "TcpCom.h"
#include  "XmlParser.h"

extern T_ServerParam 	t_ServerParam;  
extern T_ServerMess  	t_ServerMess;

static int AnalyzeRegister(PT_TcpClient pt_TcpClient)
{

	ST_SM_ANAY_VDCS_REGISTER st_AnayReg;
	T_PacketHead             t_PackHeadAnayReg;
	char AnayRegBuff[60] = {0};
	int iRet = -1;
	memset(&t_PackHeadAnayReg,0 ,sizeof(t_PackHeadAnayReg));
	memset(&st_AnayReg,0 ,sizeof(st_AnayReg));
	t_PackHeadAnayReg.magic          = T_PACKETHEAD_MAGIC;
	t_PackHeadAnayReg.cmd	         = SM_ANAY_VDCS_REGISTER;
	t_PackHeadAnayReg.UnEncryptLen   = sizeof(ST_SM_ANAY_VDCS_REGISTER);

	//memcpy(st_AnayReg.MCUAddr,t_McuMess.cMac,MCU_MAC_LEN_20);

	memcpy(AnayRegBuff,&t_PackHeadAnayReg,sizeof(t_PackHeadAnayReg));
	memcpy(AnayRegBuff+sizeof(t_PackHeadAnayReg),&st_AnayReg,sizeof(st_AnayReg));
	iRet = send(t_ServerMess.iClientFd,AnayRegBuff,sizeof(AnayRegBuff),0);
	dbgprint("send server %d \n",iRet);
	t_ServerMess.iAnayRegister = 1;
	return iRet;
}

static int ClientSendHreatbeat(PT_TcpClient pt_TcpClient)
{
	int iRet = -1;
	T_PacketHead heartbeat;
	memset(&heartbeat, 0 ,sizeof(heartbeat));
	heartbeat.magic        	=  T_PACKETHEAD_MAGIC;
	heartbeat.cmd	       		=  SM_ANAY_HEATBEAT;
	heartbeat.UnEncryptLen =  0;
	iRet = send(t_ServerMess.iClientFd,&heartbeat,sizeof(T_PacketHead),0);
	printf("send heartbeat is %d\n",iRet);
	return iRet;
}

static void client_read_callback(struct bufferevent* bev, void* arg)	
{
	char buf[2048];  
	int 	iLen;
	PT_TcpClient instance  =  (PT_TcpClient)arg; 
	
	memset(buf,0,2048);
	struct evbuffer *input = bufferevent_get_input(bev);
	if (evbuffer_get_length(input) == 0) {
		printf("no data read!\n");
		bufferevent_free(bev);
	} 
	if(evbuffer_get_length(input) >2048) return ;
	
	printf("read!\n");
	
	return ;
}

static void client_timer_callbak(int fd, short events, void * arg) 
{
	PT_TcpClient pt_TcpClient  =  (PT_TcpClient)arg; 

	if( 0 == t_ServerMess.ialive )
	{
		bufferevent_socket_connect(pt_TcpClient->pt_PServerBev, (struct sockaddr *)&pt_TcpClient->t_PSeverAddr,  sizeof(pt_TcpClient->t_PSeverAddr));
	}
	if( 1 == t_ServerMess.ialive )
	{	  
		if((!t_ServerMess.iAnayRegister))
		{
			AnalyzeRegister(pt_TcpClient);
		}
		if(t_ServerMess.iAnayRegister){
			ClientSendHreatbeat(pt_TcpClient);
			
		}
	}
}

static void client_event_callbak(struct bufferevent *bev, short event, void *arg)	
{
	PT_TcpClient pt_TcpClient  =  (PT_TcpClient)arg; 
	int fd = -1;
	 
	if( event & BEV_EVENT_CONNECTED)  {  
		printf("--the client has connected to process server--\n");
		t_ServerMess.ialive	 = 1;
        	bufferevent_enable( pt_TcpClient->pt_PServerBev, EV_READ |EV_WRITE );
        	fd  = bufferevent_getfd(pt_TcpClient->pt_PServerBev);
        	t_ServerMess.iClientFd = fd;

	} else{
		printf("process server close connection\n"); 
		t_ServerMess.ialive = 0;
	        fd = bufferevent_getfd(pt_TcpClient->pt_PServerBev);
		if( fd > 0 ){
			evutil_closesocket(fd);
		}
		bufferevent_setfd(pt_TcpClient->pt_PServerBev, -1);
		t_ServerMess.iAnayRegister = 0;
	}
}


void * tcp_client_thread(void *param)
{
	T_TcpClient   t_TcpClient;
	
	struct event eTimeout;
	struct timeval tTimeout = {5, 0};

	memset(&t_TcpClient, 0 ,sizeof(T_TcpClient));
	
	t_TcpClient.pt_PServerBase  = event_base_new();  
	if (!t_TcpClient.pt_PServerBase) {
		printf("Could not initialize libevent!\n");
		return NULL;
	}

	t_TcpClient.pt_PServerBev = bufferevent_socket_new(t_TcpClient.pt_PServerBase, -1, BEV_OPT_CLOSE_ON_FREE);  
	if (!t_TcpClient.pt_PServerBev) {
		printf("Error constructing bufferevent!\n");
		return NULL;
	}
	
	memset(&t_TcpClient.t_PSeverAddr, 0, sizeof(t_TcpClient.t_PSeverAddr) );	
	t_TcpClient.t_PSeverAddr.sin_family	=	AF_INET;  
	t_TcpClient.t_PSeverAddr.sin_port	=	htons(atoi(t_ServerParam.server_port));  
	inet_aton(t_ServerParam.server_ip, &t_TcpClient.t_PSeverAddr.sin_addr);

	bufferevent_socket_connect(t_TcpClient.pt_PServerBev, (struct sockaddr *)&t_TcpClient.t_PSeverAddr,  sizeof(t_TcpClient.t_PSeverAddr));

	bufferevent_setcb(t_TcpClient.pt_PServerBev ,client_read_callback, NULL, client_event_callbak, (void*)&t_TcpClient);  

	event_assign(&eTimeout, t_TcpClient.pt_PServerBase, -1, EV_PERSIST, client_timer_callbak, (void*)&t_TcpClient);
	if (evtimer_add(&eTimeout, &tTimeout)<0) {
		printf("Could not create/add a timer_event!\n");
		return NULL;
	}
	bufferevent_enable(t_TcpClient.pt_PServerBev, EV_READ|EV_WRITE | EV_PERSIST);  

		
	event_base_dispatch (t_TcpClient.pt_PServerBase);	
	// ÊÍ·Å×ÊÔ´
	event_base_free 	(t_TcpClient.pt_PServerBase);
	bufferevent_free  (t_TcpClient.pt_PServerBev); 

	 printf("-- tcp_server_thread is down--\n");
	 pthread_exit(NULL);
	 return NULL;

}



