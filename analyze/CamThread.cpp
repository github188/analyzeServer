#include "CamThread.h"
#include "SerData.h"

CCamThread::CCamThread(T_CAM_FIXED_PARAM & t_Camfixedparam)
	:m_videoStream(t_Camfixedparam.CameUri),m_index(t_Camfixedparam.index)
{
	m_CameraFlag = true;
	m_NoFrameNum = 0;
	m_ReConnect  = 0;
	sprintf(window,"%s-%d",Cam,m_index);
}

CCamThread::~CCamThread()
{
	m_CameraFlag = false;
}

int CCamThread::GetCamParam()
{
	Mat tmpframe;
	uint32  num  = 0;
	m_vcap.open(CV_CAP_FIREWARE);
	if(!m_vcap.open(m_videoStream)) {
		report_cam_break(m_index);
		dbgprint("%s(%d),%d CAM opening video stream failed!\n",DEBUGARGS,m_index);
		return -1;
	}

	m_cols    =	 m_vcap.get(CV_CAP_PROP_FRAME_WIDTH);
	m_rows    =  m_vcap.get(CV_CAP_PROP_FRAME_HEIGHT);
	m_fps     =  m_vcap.get(CV_CAP_PROP_FPS);

	while(!(m_vcap.read(tmpframe))) {
		num++;
		if(num > 500){
				report_cam_break(m_index);
				dbgprint("%s(%d),%d CAM read video stream failed!\n",DEBUGARGS,m_index);
				return -1;
		}

	}

	ReadFrame.create(m_rows,m_cols,tmpframe.type());
	return 0;
}

int CCamThread::re_connect(){
	ReadFrame.release();
	while (m_ReConnect&&m_CameraFlag) {
		if(!m_vcap.open(m_videoStream)) {
			sleep(10);
			continue;
		}

		m_cols    =	 m_vcap.get(CV_CAP_PROP_FRAME_WIDTH);
		m_rows    =  m_vcap.get(CV_CAP_PROP_FRAME_HEIGHT);
		m_fps     =  m_vcap.get(CV_CAP_PROP_FPS);
		if(!(m_vcap.read(tmpframe))) {
			sleep(10);
			continue;
		}
		m_ReConnect = 0;
		ReadFrame.create(m_rows,m_cols,tmpframe.type());
		report_cam_reconnect(m_index);
		//notify_go_on();
	}
}

void CCamThread::run()
{
	int iRet = -1;
	while(m_CameraFlag){
		if(!(m_vcap.read(ReadFrame))) {
			dbgprint("%s(%d),%d CAM no frame!\n",DEBUGARGS,m_index);
			m_NoFrameNum++;
			if(m_NoFrameNum >= 250){
					m_ReConnect = 1;
					report_cam_break(m_index);
					//notify_hang_up();
					re_connect();
			}
			waitKey(40);
			continue;
		}
		m_NoFrameNum =0;
		ReadFrame.copyTo(Alarmthead1Frame);
		ReadFrame.copyTo(Alarmthead2Frame);
		imshow(window,ReadFrame);
		waitKey(5);
	}

	dbgprint("%s(%d),%d CamThread exit!\n",DEBUGARGS,m_index);
	pthread_exit(NULL);
}

int CCamThread::CreateCamThread()
{
	int iRet = -1;
	pthread_t CamThread;
	iRet = pthread_create(&CamThread,NULL,RunCamThread,this);
	if(iRet != 0){
		 dbgprint("%s(%d),create %d CamThread failed!\n",DEBUGARGS,m_index);
		 return -1;
	}
	pthread_detach(CamThread);
	return 0;
}
