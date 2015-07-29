
#ifndef _XMLPARSER_H_
#define _XMLPARSER_H_

#include "Common.h"

#include "tinyxml.h"
#include "tinystr.h"

/*
 *	city server parameters include ip / port
 */
typedef struct   _SERVER_PARAM{

	char		server_ip[20];
	char		server_port[8];

}T_ServerParam;

// XML parser class
class CXmlParser
{

public:
	CXmlParser(char* infile);
	~CXmlParser();

	void GetServerParam(T_ServerParam &t_ServerParam);
  void GetAnalyzeParam(uint32 &Position);

protected:

private:

	TiXmlDocument *	m_ConfigDocument;
	TiXmlElement *	m_RootElement;

	TiXmlElement * 	m_ServerElement;
	TiXmlElement *  m_AnalyzeElement;

	TiXmlElement *	m_ServerIpElement;
	TiXmlElement *	m_ServerPortElement ;

  TiXmlElement *  m_AnalyzePositionElement;
};

#endif
