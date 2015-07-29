#ifndef _ALARM_THREAD_H_
#define _ALARM_THREAD_H_

#include "Common.h"
#include "CamThread.h"

class CAlarmThread
{

public:
  CAlarmThread(CCamThread* &camera,uint8 index);
  CCamThread* m_AlarmCamera;
  bool  m_AlarmFlag;
  bool  m_Status;

  Mat frame1;
  Mat frame2;
  
  void run();
	int  CreateAlarmThread();
	static void* RunAlarmThread(void*  param){
		CAlarmThread* p = (CAlarmThread*)param;
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
  uint8 m_cam_index;
  uint8 m_alarm_index;

  pthread_mutex_t mut;
  pthread_cond_t cond;
};

#endif
