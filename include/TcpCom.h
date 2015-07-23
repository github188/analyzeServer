#ifndef    _TCPCOM_H_
#define    _TCPCOM_H_

#include 	"Common.h"
#include 	"CmdDefine.h"

typedef struct _server_mess{
	
	    int    iClientFd;
	    int    ialive;
	    int    iConnectFlag;
	    int    iAnayRegister;
		
}T_ServerMess,*PT_ServerMess;


enum {	

	SERVER_DATA_ERR =0x001,
};

typedef struct _tcp_client{
	
	struct sockaddr_in       t_PSeverAddr;		   
	struct event_base *     pt_PServerBase;		   			  				  
	struct bufferevent*       pt_PServerBev;

}T_TcpClient,*PT_TcpClient;

void * tcp_client_thread(void *param);

#endif

