
#ifndef _XMLPARSER_H_
#define _XMLPARSER_H_

#include "Common.h"

#include "tinyxml.h"  
#include "tinystr.h"  
/* 
 *	���з���������
 */
typedef struct   _SERVER_PARAM{

	char		server_ip[20];
	char		server_port[8];
	
}T_ServerParam;

// XML������
class CXmlParser
{

public:
	CXmlParser(char* infile);
	~CXmlParser();

	void GetServerParam(T_ServerParam &t_ServerParam);
	
protected:
	
private:
	
	TiXmlDocument *	m_ConfigDocument;
	TiXmlElement *	m_RootElement;
	
	TiXmlElement * 	m_ServerElement;
	TiXmlElement *	m_ServerIpElement;
	TiXmlElement *	m_ServerPortElement ;

	
};

#endif

