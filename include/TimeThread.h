#ifndef _TIME_THREAD_H_
#define _TIME_THREAD_H_

typedef struct _JUDGE_TYPE_{

  uint8 num;
  uint16 iType1;
  uint16 iType2;

  _JUDGE_TYPE_(){
		memset(this, 0, sizeof(_JUDGE_TYPE_));
	}
}T_JUDGE_TYPE;

#include "Common.h"
class CTime
{
public:
  CTime();
  ~CTime();

  uint8 m_index;
  bool  m_AlarmFlag;
  bool  m_Status;

  uint16 select_type(uint8 i);
  void SetType(uint16 warntype);

  int compare_time(T_AlarmTime & time1 ,T_AlarmTime & time2);
  int change_state(ALARM_DAY * day);
  void change_state_refer_to_time_and_type(uint16 type);
  void change_state_refer_to_time(T_JUDGE_TYPE* pt_JudgeType);

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
