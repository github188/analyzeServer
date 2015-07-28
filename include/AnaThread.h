#ifndef _ANA_THREAD_H_
#define _ANA_THREAD_H_

#include "Common.h"

class CAnaThread
{
public:
	CAnaThread(uint8 index);
	
	uint8 m_index;
	Mat   ReadFrame;
 	void run() ;
	void quit()  
	{  
		quitFlag = true;  
	}
private: 
	 bool            quitFlag;     
};

#endif