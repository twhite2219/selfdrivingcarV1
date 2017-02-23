#ifndef PICAMF_H
#define PICAMF_H

#include "distance.h"
#include "init.h"
#include <iostream>
#include "distance.h"
#include <linux/videodev2.h>
#include <../include/libv4l2.h>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>  
#include <opencv2/videoio.hpp>
#include "move.h"
using namespace cv;


void TestCamera(){

Mat initframe,blurr,B_W,edges,Image;
VideoCapture cap;
cap.open(0);
cap.set(CAP_PROP_FRAME_WIDTH,500);
cap.set(CAP_PROP_FRAME_HEIGHT,500);

	
 if (cap.isOpened()){
	 
 
for(;;)
{
	cap.read(initframe);
	if (initframe.empty()){	printf("error frame empty/n");	break;}

imshow("Live Stream",initframe);		//normal bgr output

cvtColor(initframe,B_W,CV_BGR2GRAY);//b&W stream
imshow("greyscale",B_W);

GaussianBlur(B_W,blurr,Size(9,9),1.5,1.5);//blur applied so edge detction is smoother (less hard edges)
imshow("blurr",blurr);

Canny(blurr,edges,0,30,3);//edge detection
imshow("edges",edges);

if(waitKey(30)>= 0) break;
}


}else printf("error! unable to use camera\n");
	
	
}

#endif
