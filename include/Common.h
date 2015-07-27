#ifndef _COMMON_H_
#define _COMMON_H_

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/contrib/contrib.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/opencv.hpp>


#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <math.h>
#include <fstream>
#include <vector>
#include <string>
#include <iostream> 
#include <sys/time.h>

#include <sys/types.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <arpa/inet.h>  
#include <unistd.h>  
#include <signal.h>
#include <fcntl.h> 

#include <event.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/event.h>

#include <cstdlib>
#include <stack>
#include <queue>
#include <list>
#include <map>

#include <zthread/Runnable.h>  
#include <zthread/Thread.h>  
#include "zthread/PoolExecutor.h"  
#include "zthread/ThreadedExecutor.h"  
#include "zthread/ConcurrentExecutor.h"  
#include "zthread/SynchronousExecutor.h"  
#include "zthread/Runnable.h"  
#include "zthread/PoolExecutor.h"    
 

extern "C"
{
#include <libavutil/opt.h>
#include <libavcodec/avcodec.h>
#include <libavutil/channel_layout.h>
#include <libavutil/common.h>
#include <libavutil/imgutils.h>
#include <libavutil/mathematics.h>
#include <libavutil/samplefmt.h>
#include <libswscale/swscale.h> 
#include <libavformat/avformat.h>

}

using namespace ZThread;  
using namespace std;
using namespace cv;

#define false (0)
#define true (1)

typedef unsigned char byte;
typedef int                    BOOL;

typedef unsigned char  		uint8;
typedef unsigned short 		uint16;
typedef unsigned int       	uint32;

#define DEBUG

#ifdef DEBUG
        #define dbgprint(format,args...) \
        fprintf(stderr, format, ##args)
#else
        #define dbgprint(format,args...)
#endif

#define DEBUGARGS __FILE__,__LINE__

#endif


