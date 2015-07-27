
#include "VideoStream.h"

VideoStream::VideoStream()
{
	 m_i_frameFinished  = 0;
	 BuffMutex = PTHREAD_MUTEX_INITIALIZER;
}

VideoStream::~VideoStream()
{
	pthread_mutex_destroy (&BuffMutex);
}

void VideoStream::setUrl(String url)
{
	m_str_url = url;
}

void VideoStream::startStream()
{
	videoStreamIndex=-1;
	av_register_all();              
	avformat_network_init();
	pAVFormatContext = avformat_alloc_context();
	pAVFrame=av_frame_alloc();

}

void VideoStream::stopStream()
{
	avformat_free_context(pAVFormatContext);
	av_frame_free(&pAVFrame);
	sws_freeContext(pSwsContext);
}

bool VideoStream::Init()
{
	int iRet = -1;
	
	if(m_str_url.empty())
	    		return false;
	iRet=avformat_open_input(&pAVFormatContext, m_str_url.c_str(),NULL,NULL);
	if (iRet<0){
		dbgprint("%s(%d),open videostream error!\n",DEBUGARGS);
		return false;
	}

	iRet=avformat_find_stream_info(pAVFormatContext,NULL);
	if (iRet<0){
		dbgprint("%s(%d),get  videostream message error!\n",DEBUGARGS);
		return false;
	}

	videoStreamIndex = -1;
	for (uint i = 0; i < pAVFormatContext->nb_streams; i++) {
	    if (pAVFormatContext->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
	        videoStreamIndex = i;
	        break;
	    }
	}

	if (videoStreamIndex==-1){
		dbgprint("%s(%d),get  videoStreamIndex error!\n",DEBUGARGS);
		return false;
	}

	pAVCodecContext = pAVFormatContext->streams[videoStreamIndex]->codec;
	videoWidth=pAVCodecContext->width;
	videoHeight=pAVCodecContext->height;

	avpicture_alloc(&pAVPicture,PIX_FMT_RGB24,videoWidth,videoHeight);

	AVCodec *pAVCodec;

	pAVCodec = avcodec_find_decoder(pAVCodecContext->codec_id);
	pSwsContext = sws_getContext(videoWidth,videoHeight,PIX_FMT_YUV420P,videoWidth,videoHeight,PIX_FMT_RGB24,SWS_BICUBIC,0,0,0);

	iRet=avcodec_open2(pAVCodecContext,pAVCodec,NULL);
	if (iRet<0){
		dbgprint("%s(%d),open decoder error!\n",DEBUGARGS);
		return false;
	}
	ReadFrame.create(videoHeight,videoWidth,CV_8UC3);

	dbgprint("%s(%d),initalize videostream sucess!\n",DEBUGARGS);
	return true;
	
}

void VideoStream::play()
{
    if (av_read_frame(pAVFormatContext, &pAVPacket) >= 0){
        if(pAVPacket.stream_index==videoStreamIndex){

            avcodec_decode_video2(pAVCodecContext, pAVFrame, &m_i_frameFinished, &pAVPacket);
            if (m_i_frameFinished){
                 pthread_mutex_lock(&BuffMutex);
                //pSwsContext = sws_getContext(videoWidth,videoHeight,PIX_FMT_YUV420P,videoWidth,videoHeight,PIX_FMT_RGB24,SWS_BICUBIC,0,0,0);
                 sws_scale(pSwsContext,(const uint8_t* const *)pAVFrame->data,pAVFrame->linesize,0,videoHeight,pAVPicture.data,pAVPicture.linesize);
                 ReadFrame.data =pAVPicture.data[0];
                 //sws_freeContext(pSwsContext);
                 pthread_mutex_unlock(&BuffMutex);
            }
        }
    }
 
    av_free_packet(&pAVPacket);
}

