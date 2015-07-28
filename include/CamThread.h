#ifndef _CAM_THREAD_H_
#define _CAM_THREAD_H_

#include "Common.h"

class CCamThread : public  Runnable
{
public:
	CCamThread();

	Mat   ReadFrame;
	VideoCapture  	m_vcap;
	string m_videoStream;
	uint8  m_index;
	uint8  m_CameraFlag;

	int GetCamParam();
 	void run() ;

private:
	int 	 	m_rows; 	 /* high */
	int 	 	m_cols; 	  /* width */
	int	  	m_fps;
};

#endif
