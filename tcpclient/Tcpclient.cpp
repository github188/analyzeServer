
#include  "TcpCom.h"
#include  "XmlParser.h"
#include  "SerData.h"

extern T_ServerParam 	t_ServerParam;
extern T_ServerMess  	t_ServerMess;
extern uint32 Position;
/**********************************************************************
* function name: client_read_callback
* Description  : libevent read callback
* Author       : 2015/7/29   by nikola
***********************************************************************/
static void client_read_callback(struct bufferevent* bev, void* arg)
{
	char buf[2048];
	int 	iLen;
	PT_TcpClient pt_TcpClient  =  (PT_TcpClient)arg;

	memset(buf,0,2048);
	struct evbuffer *input = bufferevent_get_input(bev);
	if (evbuffer_get_length(input) == 0) {
		bufferevent_free(bev);
	}
	if(evbuffer_get_length(input) >2048) return ;

	iLen = evbuffer_remove(input, buf, sizeof(buf));

	server_data_analyse(buf,iLen,pt_TcpClient);
}
/**********************************************************************
* function name: AnalyzeServerRegister
* Description  : send Register to city server
* Author       : 2015/7/29   by nikola
***********************************************************************/
static int AnalyzeServerRegister(PT_TcpClient pt_TcpClient)
{
	int iRet = -1;
	T_PacketHead			 				t_PackHeadAnayReg;
	ST_SM_ANAY_VDCS_REGISTER 	t_AnayReg;

	char AnayRegBuff[28 + 4] = {0};

	t_PackHeadAnayReg.magic         = T_PACKETHEAD_MAGIC;
	t_PackHeadAnayReg.cmd	         	= SM_ANAY_VDCS_REGISTER;
	t_PackHeadAnayReg.UnEncryptLen  = sizeof(ST_SM_ANAY_VDCS_REGISTER);

	t_AnayReg.Position = Position;

	memcpy(AnayRegBuff,&t_PackHeadAnayReg,sizeof(T_PacketHead));
	memcpy(AnayRegBuff+sizeof(T_PacketHead),&t_AnayReg,sizeof(ST_SM_ANAY_VDCS_REGISTER));
	iRet = send(t_ServerMess.iClientFd,AnayRegBuff,sizeof(AnayRegBuff),0);
	if(iRet < 0){
		dbgprint("%s(%d),send register to server error!\n",DEBUGARGS);
		return -1;
	}
	dbgprint("%s(%d),send register to server ,total %d bytes!\n",DEBUGARGS,iRet);

	//t_ServerMess.iAnayRegister = 1;  /* test used / should be in ack function file */
	return 0;
}
/**********************************************************************
* function name: ClientSendHreatbeat
* Description  : send heartbeat to city server while connecting per 5S
* Author       : 2015/7/29   by nikola
***********************************************************************/
static int ClientSendHreatbeat(PT_TcpClient pt_TcpClient)
{
	int iRet = -1;
	T_PacketHead t_HeartBeat;
	memset(&t_HeartBeat, 0 ,sizeof(t_HeartBeat));
	t_HeartBeat.magic        	=  T_PACKETHEAD_MAGIC;
	t_HeartBeat.cmd	       		=  SM_ANAY_HEATBEAT;
	t_HeartBeat.UnEncryptLen	=  0;
	iRet = send(t_ServerMess.iClientFd,&t_HeartBeat,sizeof(T_PacketHead),0);
	if(iRet < 0){
		dbgprint("%s(%d),send heartbeat error!\n",DEBUGARGS);
		return -1;
	}
	dbgprint("%s(%d),send heartbeat,total %d bytes!\n",DEBUGARGS,iRet);
	return 0;
}
/**********************************************************************
* function name: client_timer_callbak
* Description  : libevent time callback per 5s
									1) if connected , and registered  send heartbeat
									2) if connected , and not registered  send  register
									3) if not connected, connect city server
* Author       : 2015/7/29   by nikola
***********************************************************************/
static void client_timer_callbak(int fd, short events, void * arg)
{
	PT_TcpClient pt_TcpClient  =  (PT_TcpClient)arg;

	if( 0 == t_ServerMess.ialive )
	{
		bufferevent_socket_connect(pt_TcpClient->pt_PServerBev,
			(struct sockaddr *)&pt_TcpClient->t_PSeverAddr,  sizeof(pt_TcpClient->t_PSeverAddr));
	}

	if( 1 == t_ServerMess.ialive )
	{
		if((0 == t_ServerMess.iAnayRegister)){
			AnalyzeServerRegister(pt_TcpClient);
		}
		if(1 == t_ServerMess.iAnayRegister){
			ClientSendHreatbeat(pt_TcpClient);
		}
	}
}
/**********************************************************************
	* function name: client_event_callbak
  * Description  : connect to city server state
										1) if connected , get fd and set server state
										2) if not connected, close old fd,and set connecting
  * Author       : 2015/7/29   by nikola
 ***********************************************************************/
static void client_event_callbak(struct bufferevent *bev, short event, void *arg)
{
	PT_TcpClient pt_TcpClient  =  (PT_TcpClient)arg;
	int iFd = -1;

	if( event & BEV_EVENT_CONNECTED)  {
		dbgprint("%s(%d),connect to city server sucess!\n",DEBUGARGS);
		iFd  = bufferevent_getfd(pt_TcpClient->pt_PServerBev);
		t_ServerMess.iClientFd = iFd; /* got fd */
		t_ServerMess.ialive	   = 1;
		bufferevent_enable( pt_TcpClient->pt_PServerBev, EV_READ |EV_WRITE );
	}else{
		dbgprint("%s(%d),city server close connection!\n",DEBUGARGS);
		t_ServerMess.ialive    = 0;
	  iFd = bufferevent_getfd(pt_TcpClient->pt_PServerBev);
		if( iFd> 0 ){
			evutil_closesocket(iFd);
		}
		bufferevent_setfd(pt_TcpClient->pt_PServerBev, -1);
		t_ServerMess.iAnayRegister = 0;  /* change registe state */
	}
}
/**********************************************************************
 * function name: tcp_client_thread
 * Description  : connect to city server thread
 * Author       : 2015/7/29   by nikola
 ***********************************************************************/
void * tcp_client_thread(void *param)
{
	T_TcpClient   t_TcpClient;

	struct event eTimeout;
	struct timeval tTimeout = {5, 0};

	memset(&t_TcpClient, 0 ,sizeof(T_TcpClient));

	t_TcpClient.pt_PServerBase  = event_base_new();
	if (!t_TcpClient.pt_PServerBase) {
		dbgprint("%s(%d),Could not initialize libevent!\n",DEBUGARGS);
		return NULL;
	}

	t_TcpClient.pt_PServerBev = bufferevent_socket_new(t_TcpClient.pt_PServerBase, -1, BEV_OPT_CLOSE_ON_FREE);
	if (!t_TcpClient.pt_PServerBev) {
		dbgprint("%s(%d),Error constructing bufferevent!\n",DEBUGARGS);
		return NULL;
	}

	memset(&t_TcpClient.t_PSeverAddr, 0, sizeof(t_TcpClient.t_PSeverAddr) );
	t_TcpClient.t_PSeverAddr.sin_family	=	AF_INET;
	t_TcpClient.t_PSeverAddr.sin_port	=	htons(atoi(t_ServerParam.server_port));
	inet_aton(t_ServerParam.server_ip, &t_TcpClient.t_PSeverAddr.sin_addr);

	bufferevent_socket_connect(t_TcpClient.pt_PServerBev,
		(struct sockaddr *)&t_TcpClient.t_PSeverAddr,  sizeof(t_TcpClient.t_PSeverAddr));

	bufferevent_setcb(t_TcpClient.pt_PServerBev ,
		client_read_callback, NULL, client_event_callbak, (void*)&t_TcpClient);

	event_assign(&eTimeout, t_TcpClient.pt_PServerBase, -1, EV_PERSIST,
		 client_timer_callbak, (void*)&t_TcpClient);
	if (evtimer_add(&eTimeout, &tTimeout)<0) {
		dbgprint("%s(%d),Could not create/add a timer_event!\n",DEBUGARGS);
		return NULL;
	}
	bufferevent_enable(t_TcpClient.pt_PServerBev, EV_READ|EV_WRITE | EV_PERSIST);

	event_base_dispatch (t_TcpClient.pt_PServerBase);  /* goto while */
	/* free resource */
	event_base_free 	(t_TcpClient.pt_PServerBase);
	bufferevent_free  (t_TcpClient.pt_PServerBev);

	dbgprint("%s(%d), tcp_server_thread is down!\n",DEBUGARGS);
	pthread_exit(NULL);
	return NULL;
}
