#ifndef _CAM_THREAD_H_
#define _CAM_THREAD_H_

#include "Common.h"
#include "Analyze.h"

class CCamThread
{
public:
	CCamThread(T_CAM_FIXED_PARAM & t_Camfixedparam);

	Mat   ReadFrame;
	Mat   Alarmthead1Frame;
	Mat   Alarmthead2Frame;

	VideoCapture  	m_vcap;
	string m_videoStream;
	uint8  m_index;
	bool  m_CameraFlag;
	uint32 m_NoFrameNum;
	uint8  m_ReConnect;
	char window[20];

	int GetCamParam();
	int re_connect();

	void run();
	int  CreateCamThread();
	static void* RunCamThread(void*  param){
		CCamThread* p = (CCamThread*)param;
		p->run();
		return NULL;
	}
	
	void quit()
	{
		m_CameraFlag = false;
	}

private:
	int 	 	m_rows; 	 /* high */
	int 	 	m_cols; 	  /* width */
	int	  	m_fps;
};

#endif
