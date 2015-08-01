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

uint16 CTime::select_type(uint8 i)
{
  uint16 Ret = 0;
  switch (i) {
    case 0:
          Ret = HumanDetect;
          break;
    case 1:
          Ret = SmokeDetect;
          break;
    case 2:
          Ret = RegionDetect;
          break;
    case 3:
          Ret = FixedObjDetect;
          break;
    case 4:
          Ret = FireDetect;
          break;
    case 5:
          Ret = ResidueDetect;
          break;
    case 6:
          Ret = GenderDetect;
          break;
    default : break;
  }
  if(0 == Ret){
    dbgprint("%s(%d),cam %d warntype  wrong!\n",DEBUGARGS,m_index);
  }
  return Ret;
}

T_JUDGE_TYPE & CTime::judgetype()
{
    T_JUDGE_TYPE tmp;
    int i = 0;
    int iNum = 0;
    uint16  ret  = 0x0000;
    uint16  type = 0x0001;
    uint16 type1 = 0;
    uint16 type2 = 0;

    for(i = 0; i< WarnNumber; i++)
    {
        ret = m_warntype & type;
        if(ret)
        {
          iNum++;
          if(1 == iNum){
              type1 = select_type(i);
          }
          if(2 == iNum){
              type2 = select_type(i);
          }
          if(iNum > 2){
              dbgprint("%s(%d),cam %d warntype number wrong!\n",DEBUGARGS,m_index);
              return tmp;
          }
        }

        type << 1;
    }

    if(0 == iNum){
        dbgprint("%s(%d),cam %d warntype number wrong!\n",DEBUGARGS,m_index);
        return tmp;
    }

    if(1 == iNum){
        tmp.num    = iNum;
        tmp.iType1 = type1;
        return tmp;
    }

    if(2 == iNum){
        tmp.num    = iNum;
        tmp.iType1 = type1;
        tmp.iType2 = type2;
        return tmp;
    }

    return tmp;
}

int CTime::compare_time(T_AlarmTime & time1 ,T_AlarmTime & time2)
{
  if(time1.hour < time2.hour)
  {
    return -1;
  }

  if(time1.hour > time2.hour)
  {
    return 1;
  }

  if(time1.hour == time2.hour)
  {
    if(time1.min < time2.min)
    {
      return -1;
    }

    if(time1.min > time2.min)
    {
      return 1;
    }

    if(time1.min == time2.min)
    {
      return 0;
    }
  }
}

int CTime::change_state(ALARM_DAY * day) //?? change state need or not a lock
{
  int iRet = -1;
  time_t timep;
  int iday = -1;
	struct tm * pTM=NULL;
  T_AlarmTime tmp;

  memset(&tmp, 0 , sizeof(T_AlarmTime));
  time (&timep);
  pTM = localtime(&timep);
  if(pTM->tm_wday == 0) pTM->tm_wday =7;
  iday =  pTM->tm_wday -1;
  tmp.hour = pTM->tm_hour;
  tmp.min  = pTM->tm_min;

  if(1 == day[iday].En)
  {
    iRet = compare_time(day[iday].dayTime.time1.Start,day[iday].dayTime.time1.End);
    if(iRet < 0)
    {
       if(( compare_time(tmp,day[iday].dayTime.time1.Start) > 0 ) &&
                  ( compare_time(tmp,day[iday].dayTime.time1.End) < 0 ))
                  {
                      day[iday].detect = 1;
                      return 0;
                  }
    }

    iRet = compare_time(day[iday].dayTime.time2.Start,day[iday].dayTime.time2.End);
    if(iRet < 0)
    {
       if(( compare_time(tmp,day[iday].dayTime.time2.Start) > 0 ) &&
                  ( compare_time(tmp,day[iday].dayTime.time2.End) < 0 ))
                  {
                      day[iday].detect = 1;
                      return 0;
                  }
    }

    iRet = compare_time(day[iday].dayTime.time3.Start,day[iday].dayTime.time3.End);
    if(iRet < 0)
    {
       if(( compare_time(tmp,day[iday].dayTime.time3.Start) > 0 ) &&
                  ( compare_time(tmp,day[iday].dayTime.time3.End) < 0 ))
                  {
                      day[iday].detect = 1;
                      return 0;
                  }
    }
  }
  day[iday].detect = 0;
  return 0;
}

void CTime::change_state_refer_to_time_and_type(uint16 type)
{
  switch (type) {
    case HumanDetect:
          change_state(t_camera[m_index].t_Camvarparam.t_CamHumAlarm.day);
          break;
    case SmokeDetect:
          change_state(t_camera[m_index].t_Camvarparam.t_CamSmkAlarm.day);
          break;
    case RegionDetect:
          change_state(t_camera[m_index].t_Camvarparam.t_CamRegionAlarm.day);
          break;
    case FixedObjDetect:
          change_state(t_camera[m_index].t_Camvarparam.t_CamFixedObjAlarm.day);
          break;
    case FireDetect:
          change_state(t_camera[m_index].t_Camvarparam.t_CamFireAlarm.day);
          break;
    case ResidueDetect:
          change_state(t_camera[m_index].t_Camvarparam.t_CamLostAlarm.day);
          break;
    case GenderDetect:
          //change_state(&t_camera[m_index].t_Camvarparam.t_CamFireAlarm.day);
          break;
    default:
          break;
    };
  }
}

void CTime::change_state_refer_to_time(T_JUDGE_TYPE* pt_JudgeType)
{
  switch (pt_JudgeType->num) {
    case 1:
          change_state_refer_to_time_and_type(pt_JudgeType->iType1);
          break;
    case 2:
          change_state_refer_to_time_and_type(pt_JudgeType->iType1);
          change_state_refer_to_time_and_type(pt_JudgeType->iType2);
          break;
    default: break;
  }
}

void CTime::run()
{
  T_JUDGE_TYPE t_JudgeType;
  while(m_AlarmFlag){
      pthread_mutex_lock(&mut);
      while (!m_Status)
      {
          pthread_cond_wait(&cond, &mut);
          memset(&t_JudgeType, 0 , sizeof(T_JUDGE_TYPE));
          t_JudgeType = judgetype();
      }
      pthread_mutex_unlock(&mut);

      change_state_refer_to_time(&t_JudgeType);
      sleep(5);
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
