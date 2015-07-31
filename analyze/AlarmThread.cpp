#include "Alarmthread.h"

CAlarmThread::CAlarmThread(CCamThread** camera,uint8 cam_index,uint8 alarm_index)
{
  m_AlarmCamera = *camera;
  m_cam_index   = cam_index;
  m_alarm_index = alarm_index;
  m_AlarmFlag   = true;
  m_Status      = false;
  WarnType      = 0;
  alarm         = 0;

  frame = 0;
  startflag         = 0;
  stopflag          = 0;
  intervalflag      = 0;
  alarmStartframe   = 0;
  alarmStopframe    = 0;

  mut   = PTHREAD_MUTEX_INITIALIZER;
  cond  = PTHREAD_COND_INITIALIZER;

  region = new CRegion(m_cam_index);

  videoHandler = NULL;
  fire = new CFire(m_cam_index ,&videoHandler);

}

CAlarmThread::~CAlarmThread()
{
    pthread_mutex_destroy(&mut);
    pthread_cond_destroy(&cond);

    delete region;
    region = NULL;

    videoHandler = NULL;
    delete fire;
    fire = NULL;

}

int CAlarmThread::alarmStrategy()
{
  //alarm strategy 5 frame alarm  and 4s later start another
  if( 1 == alarm && !startflag ){
      alarmStartframe++;
  }else{
    alarmStartframe = 0;
  }

  if(0 == alarm && stopflag){
      alarmStopframe++;
  }else{
      alarmStopframe = 0;
  }

  if(alarmStartframe > START_FRAME_INDEX)
  {
    startflag = 1;
    intervalflag = 1;
    alarmStartframe = 0;
    return alarmOn;
  }

  if(1 == intervalflag)
  {
      frame++;
  }

  if (frame > INTERVAL_FRAME_INDEX)
  {
     stopflag  = 1;
     intervalflag = 0;
     frame = 0;
  }

  if(alarmStopframe >  STOP_FRAME_INDEX)
  {
    stopflag  = 0;
    startflag = 0;
    alarmStopframe = 0;
    return alarmStop;
  }

  return 0;
}

int  CAlarmThread::region_detect(Mat &frame)
{
  int iRet = -1;

  if(!frame.empty())
  {
  		region->alarmRegionDetectRun(frame);
      alarm = region->alarm ;
  		usleep(20*1000);
  }
  else
  {
      alarm = 0;
      usleep(40*1000);
  }
  iRet =  alarmStrategy();

  if(iRet == alarmOn)
  {
    //TODO: notify region alarm start and push rtsp
  }

  if(iRet == alarmStop)
  {
    //TODO: notify region alarm stop
  }
  return 0;
}

int  CAlarmThread::fire_detect(Mat &frame)
{
  int iRet = -1;

  if(!frame.empty())
  {
      fire->frame(tmp,(void *)videoHandler);
      alarm = fire->alarm ;
      usleep(20*1000);
  }
  else
  {
      alarm = 0;
      usleep(40*1000);
  }
  iRet =  alarmStrategy();

  if(iRet == alarmOn)
  {
    //TODO: notify fire alarm start and push rtsp
  }

  if(iRet == alarmStop)
  {
    //TODO: notify fire alarm stop
  }
  return 0;
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
    default:
          dbgprint("%s(%d),cam %d alarmindex  %d wrong WarnType %x !\n",
                                  DEBUGARGS,m_cam_index,m_alarm_index,iType);
          usleep(40*1000);
          break;
  }
  return 0;
}

void CAlarmThread::resource_release()
{
  alarm = 0;

  frame = 0;
  startflag         = 0;
  stopflag          = 0;
  intervalflag      = 0;
  alarmStartframe   = 0;
  alarmStopframe    = 0;

  //release region resource
  region->frameindex = 0;
  region->alarm      = 0;
}

void CAlarmThread::run()
{  // start within pause
  while(m_AlarmFlag){
      pthread_mutex_lock(&mut);
      while (!m_Status)
      {
          pthread_cond_wait(&cond, &mut);
          resource_release();
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
         usleep(40*1000);;
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
