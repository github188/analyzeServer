#ifndef _TIME_THREAD_H_
#define _TIME_THREAD_H_

#include "Common.h"
class CTime
{
public:
  CTime();
  ~CTime();

  uint8 m_index;
  bool  m_AlarmFlag;
  bool  m_Status;



  void SetType(uint16 warntype);
  void run();
	int  CreateTimeThread();
	static void* RunTimeThread(void*  param){
		CTime* p = (CTime*)param;
		p->run();
		return NULL;
	}
  void thread_resume();
  void thread_pause();
  void quit()
	{
		m_AlarmFlag = false;
	}
private:
  pthread_mutex_t mut;
  pthread_cond_t cond;

  uint16 m_warntype;
};

#endif
