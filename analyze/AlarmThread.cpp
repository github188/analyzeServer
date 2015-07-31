#include "Alarmthread.h"

CAlarmThread::CAlarmThread(CCamThread** camera,uint8 cam_index,uint8 alarm_index)
{
  m_AlarmCamera = *camera;
  m_cam_index   = cam_index;
  m_alarm_index = alarm_index;
  m_AlarmFlag   = true;
  m_Status      = false;
  WarnType      = 0;

  mut   = PTHREAD_MUTEX_INITIALIZER;
  cond  = PTHREAD_COND_INITIALIZER;

  region = new CRegion(m_cam_index);
}

CAlarmThread::~CAlarmThread{
    pthread_mutex_destroy(&mut);
    pthread_cond_destroy(&cond);
}

int CAlarmThread::region_detect(Mat &frame)
{

}

int CAlarmThread::alarm_run(Mat &frame ,uint8 iType)
{
  switch (iType) {
    case HumanDetect:
          human_detect(frame);
          break;
    case SmokeDetect:
          smoke_detect(frame);
          break;
    case RegionDetect:
          region_detect(frame);
          break;
    case FixedObjDetect:
          fixobj_detect(frame);
          break;
    case FireDetect:
          fire_detect(frame);
          break;
    case ResidueDetect:
          residue_detect(frame);
          break;
    case GenderDetect:
          gender_detect(frame);
          break;
    default: break;
  }
  return 0;
}

void CAlarmThread::run()
{  // start within pause
  while(m_AlarmFlag){
      pthread_mutex_lock(&mut);
      while (!m_Status)
      {
          pthread_cond_wait(&cond, &mut);
      }
      pthread_mutex_unlock(&mut);

      if(1 == m_alarm_index)
      {
          m_AlarmCamera->Alarmthead1Frame.copyTo(frame1);
          alarm_run(frame1,WarnType);  //alarm_run(Mat & frame);
      }
      else if(2 == m_alarm_index)
      {
          m_AlarmCamera->Alarmthead2Frame.copyTo(frame2);
          alarm_run(frame2,WarnType);
      }
      else
      {
        //dbgprint("%s(%d),cam %d wrong m_alarm_index %d !\n",DEBUGARGS,m_cam_index,m_alarm_index);
        waitKey(40);
      }
  }
}

int CAlarmThread::CreateAlarmThread()
{
	int iRet = -1;
	pthread_t AlarmThread;
	iRet = pthread_create(&AlarmThread,NULL,RunAlarmThread,this);
	if(iRet != 0)
  {
		 dbgprint("%s(%d),cam %d create %d AlarmThread failed!\n",DEBUGARGS,m_cam_index,m_alarm_index);
		 return -1;
	}
	pthread_detach(AlarmThread);
	return 0;
}

void CAlarmThread::thread_resume()
{
    if (m_Status == false)
    {
        pthread_mutex_lock(&mut);
        m_Status = true;
        pthread_cond_signal(&cond);
        dbgprint("%s(%d), cam %d alarm %d pthread run!\n",DEBUGARGS,m_cam_index,m_alarm_index);
        pthread_mutex_unlock(&mut);
    }
    else
    {
        dbgprint("%s(%d), cam %d alarm %d pthread run already!\n",DEBUGARGS,m_cam_index,m_alarm_index);
    }
}


void CAlarmThread::thread_pause()
{
    if (m_Status == true)
    {
        pthread_mutex_lock(&mut);
        m_Status = false;
        dbgprint("%s(%d), cam %d alarm %d pthread stop!\n",DEBUGARGS,m_cam_index,m_alarm_index);
        pthread_mutex_unlock(&mut);
    }
    else
    {
        dbgprint("%s(%d), cam %d alarm %d pthread stop already!\n",DEBUGARGS,m_cam_index,m_alarm_index);
    }
}
