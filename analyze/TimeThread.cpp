#include "TimeThread.h"
#include "Analyze.h"

extern T_SINGLE_CAMERA t_camera[CAM_MAX_LEN] ;

CTime::CTime(uint8 index)
{
  m_index = index;
  m_AlarmFlag   = true;
  m_Status      = false;

  mut   = PTHREAD_MUTEX_INITIALIZER;
  cond  = PTHREAD_COND_INITIALIZER;

}

CTime::~CTime()
{
  pthread_mutex_destroy(&mut);
  pthread_cond_destroy(&cond);
}

void CTime::SetType(uint16 warntype)
{
    m_warntype = warntype;
}

void CTime::run()
{  // start within pause
  while(m_AlarmFlag){
      pthread_mutex_lock(&mut);
      while (!m_Status)
      {
          pthread_cond_wait(&cond, &mut);
          //resource_release();
      }
      pthread_mutex_unlock(&mut);




  }
  dbgprint("%s(%d),cam %d TimeThread exit!\n",DEBUGARGS,m_index);
  pthread_exit(NULL);
}

int CTime::CreateTimeThread()
{
	int iRet = -1;
	pthread_t TimeThread;
	iRet = pthread_create(&TimeThread,NULL,RunTimeThread,this);
	if(iRet != 0)
  {
		 dbgprint("%s(%d),cam %d TimeThread failed!\n",DEBUGARGS,m_cam_index);
		 return -1;
	}
	pthread_detach(TimeThread);
	return 0;
}

void CTime::thread_resume()
{
    if (m_Status == false)
    {
        pthread_mutex_lock(&mut);
        m_Status = true;
        pthread_cond_signal(&cond);
        dbgprint("%s(%d), cam %d TimeThread run!\n",DEBUGARGS,m_cam_index);
        pthread_mutex_unlock(&mut);
    }
    else
    {
        dbgprint("%s(%d), cam %d TimeThread run already!\n",DEBUGARGS,m_cam_index);
    }
}


void CTime::thread_pause()
{
    if (m_Status == true)
    {
        pthread_mutex_lock(&mut);
        m_Status = false;
        dbgprint("%s(%d), cam %d TimeThread stop!\n",DEBUGARGS,m_cam_index);
        pthread_mutex_unlock(&mut);
    }
    else
    {
        dbgprint("%s(%d), cam %d TimeThread stop already!\n",DEBUGARGS,m_cam_index);
    }
}
