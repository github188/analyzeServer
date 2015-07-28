#include "CamThread.h"


CCamThread::CCamThread(string videostream,uint8 index)
	:m_videoStream(videostream),m_index(index)
{
	m_CameraFlag = 1;
}

int CCamThread::GetCamParam()
{
	Mat tmpframe;
	uint32  num  = 0;
	m_vcap.open(CV_CAP_FIREWARE);
	if(!m_vcap.open(m_videoStream)) {
		cout << "Error opening video stream or file!" << endl;
		return -1;
   	 }

   m_cols    =	m_vcap.get(CV_CAP_PROP_FRAME_WIDTH);
	 m_rows   = 	m_vcap.get(CV_CAP_PROP_FRAME_HEIGHT);
	 m_fps      =     m_vcap.get(CV_CAP_PROP_FPS);

	 while(!(m_vcap.read(tmpframe))) {
	 	num++;
	 	if(num > 500)
	 		return -1;
	 }
	 ReadFrame .create(m_rows,m_cols,tmpframe.type());
	 return 0;
}

void CCamThread::run()
{
	int iRet = -1;
	iRet = GetCamParam();
	if(iRet  <  0){
		cout<<"report_camera_break"<<endl;
		return;//report_camera_break(num);
	}
	while(m_CameraFlag){
		if(!(m_vcap.read(ReadFrame))) {
			cout<<"no frame"<<endl;
			waitKey(1);
			continue;
		}
		imshow("ReadFrame",ReadFrame);
		waitKey(10);
	}
}
