
#include "Analyze.h"
#include "Alarmthread.h"

Canalyze::Canalyze(T_CAM_FIXED_PARAM & t_Camfixedparam)
{
	m_index        = t_Camfixedparam.index;
	m_Camera 			 = new CCamThread(t_Camfixedparam);
	m_Alarmthread1 = new CAlarmThread(&m_Camera,m_index,1);
	m_Alarmthread2 = new CAlarmThread(&m_Camera,m_index,2);
}

Canalyze::~Canalyze()
{
	delete m_Camera ;
	delete m_Alarmthread1;
	delete m_Alarmthread2;
}
