#include "AnaThread.h"

CAnaThread::CAnaThread(uint8 index)
	:quitFlag(false) 
{
	m_index =  index;
}

void CAnaThread::run()
{
	while (!quitFlag)  
	{  
	    cout << "Sleep" <<endl;  
	    Thread::sleep(5000);  

	}  
}

