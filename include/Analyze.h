
#ifndef _ANALYZE_H_
#define _ANALYZE_H_

#include "Common.h"

typedef struct _T_CAMERA_FIXED_PARAM{

  char        ip[IP_LEN_16];
  char        username[USERNAME_LEN_10];
	char        password[PASSWORD_LEN_10];
	char        uri[SINGLE_URI_LEN_128];
	uint8       CameStatus;
	uint8       index;

}T_CAM_FIXED_PARAM; /* camera Fixed parameters */

typedef struct _ALARM_TIME_{

	uint8 hour;
	uint8 min;

}T_AlarmTime;  /* time structure h-m*/

typedef struct _ALARM_TIME_INT{

	T_AlarmTime Start;
	T_AlarmTime End;

}ALARM_TIME_INT;

typedef struct _ALARM_DAY_INT{

    ALARM_TIME_INT time1;
    ALARM_TIME_INT time2;
    ALARM_TIME_INT time3;

}ALARM_DAY_INT;

typedef struct _ALARM_DAY{

  uint8 En;
  ALARM_DAY_INT dayTime;

}ALARM_DAY;

typedef struct _T_CAMERA_FUNC_HUMAN_ALARM{

	uint16    	AlarmType;
	uint8  	    AutoWarn;
	uint16      MaxNum;
	uint8       HumanDetectEn;
	uint8       Flag;                  /*  0 none  1 monitor  2 door 3 ALL  */
	uint16      Reserve1;
	uint16      Reserve2;

	vector< Rect >        MonitorZoneRects;
	vector< Line >        DirectionLines;

  ALARM_DAY day[WEEK_DAY_LEN_7];

}T_CAM_HUM_ALARM;


typedef struct _T_CAMERA_FUNC_RGN_ALARM{

	uint16    	AlarmType;
	uint8  	    AutoWarn;
	uint8       DetectEn;
	float       ChangRate;                   /* rate of change */

	vector< Rect >        Rects;

  ALARM_DAY day[WEEK_DAY_LEN_7];

}T_CAM_RGN_ALARM;


typedef struct _T_CAMERA_FUNC_OTH_ALARM{

	uint16    	AlarmType;
	uint8  	    AutoWarn;
	uint8       DetectEn;

	vector< Rect >        Rects;

  ALARM_DAY day[WEEK_DAY_LEN_7];

}T_CAM_OTH_ALARM;

typedef struct _T_CAMERA_VAR_PARAM{  // six alarm

  T_CAM_HUM_ALARM    t_CamHumAlarm;                 /*  human detect alarm parameters   */
	T_CAM_OTH_ALARM    t_CamSmkAlarm;                 /*  Smoke alarm parameters */
	T_CAM_OTH_ALARM    t_CamFireAlarm;                /*  Fire alarm parameters */
	T_CAM_OTH_ALARM    t_CamFixedObjAlarm;            /*  Fixed object alarm parameters */
  T_CAM_OTH_ALARM    t_CamLostAlarm;                /*  Residual alarm parameters */
	T_CAM_RGN_ALARM    t_CamRegionAlarm;              /*  region alarm parameters  */

}T_CAM_VAR_PARAM;

class Canalyze
{

public:

	CCamThread m_Camera;

private:

}; /* Camera analyze class */


typedef struct _T_SINGLE_CAMERA{

	T_CAM_FIXED_PARAM    t_Camfixedparam;   /* camera Fixed parameters */

 	T_CAM_VAR_PARAM      t_Camvarparam;     /* camera Variable parameter */

	Canalyze   CamAnalyze;                  /* camera analyze class */

  uint8      Enable;

}T_SINGLE_CAMERA;  /* single camera param */

#endif
