
#include "XmlParser.h"

CXmlParser::CXmlParser(char* infile)
{
	m_ConfigDocument= new TiXmlDocument(infile);

	m_ConfigDocument->LoadFile();
	m_RootElement  	 	   = m_ConfigDocument->RootElement();

	m_ServerElement 		 = m_RootElement->FirstChildElement();
	m_AnalyzeElement	   = m_ServerElement->NextSiblingElement();

	m_ServerIpElement 	 = m_ServerElement->FirstChildElement();
	m_ServerPortElement  = m_ServerIpElement->NextSiblingElement();

	m_AnalyzePositionElement = m_AnalyzeElement->FirstChildElement();
}

CXmlParser::~CXmlParser()
{	delete m_ConfigDocument;
}

void CXmlParser::GetServerParam(T_ServerParam &t_ServerParam)
{
	sprintf(t_ServerParam.server_ip,"%s", 	m_ServerIpElement->FirstChild()->Value());
	sprintf(t_ServerParam.server_port,"%s", m_ServerPortElement->FirstChild()->Value());
}

void CXmlParser::GetAnalyzeParam(uint32 &Position)
{
	Position = (uint32)m_AnalyzePositionElement->FirstChild()->Value();
}
