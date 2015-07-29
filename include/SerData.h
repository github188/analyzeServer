#ifndef _SERVER_DATA_H_
#define _SERVER_DATA_H_

#include  "TcpCom.h"
#include 	"CmdDefine.h"

int report_cam_break(uint8 index);
int report_cam_reconnect(uint8 index);
int server_data_analyse(char * buffer,int len,PT_TcpClient pt_TcpClient);

#endif
