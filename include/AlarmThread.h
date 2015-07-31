#ifndef _ALARM_THREAD_H_
#define _ALARM_THREAD_H_

#include "Common.h"
#include "CamThread.h"
#include "region.h"
#include "VideoHandler.h"
#include "fire.h"

#define START_FRAME_INDEX     8
#define INTERVAL_FRAME_INDEX  100
#define STOP_FRAME_INDEX      40
enum warnevent{
    alarmOn   = 0x01;
    alarmStop = 0x02;
};

class CAlarmThread
{

public:
  CAlarmThread(CCamThread* &camera,uint8 index);
  ~CAlarmThread();

  CCamThread* m_AlarmCamera;
  bool  m_AlarmFlag;
  bool  m_Status;

  Mat frame1;
  Mat frame2;

  uint16 WarnType;
  uint8  alarm;

  uint32 frame;
  uint32 alarmStartframe;
  uint32 alarmStopframe;
  uint8  startflag;
  uint8  stopflag;
  uint8  intervalflag;

  CRegion* region;
  VideoHandler* videoHandler ;
  CFire*   fire;

  int alarmStrategy();

  int region_detect(Mat &frame);
  int fire_detect(Mat &frame);

  int alarm_run(Mat &frame ,uint8 iType);
  void resource_release();

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
