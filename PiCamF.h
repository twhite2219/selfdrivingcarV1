#ifndef PICAMF_H
#define PICAMF_H

#include "distance.h"
#include <iostream>
#include "network.h"
#include <sstream>
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
using namespace std;

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

 void takephotos(){
	 
	 

Mat initframe,blurr,B_W,edges,image,flat_Img,arr_Img;
VideoCapture cap;
cap.open(0);
cap.set(CAP_PROP_FRAME_WIDTH,20);
cap.set(CAP_PROP_FRAME_HEIGHT,20);

	
 if (cap.isOpened()){ 
	 int ct =0;
	for(;;){
		ct++;
		cap.read(initframe);
		if (initframe.empty()){	printf("error frame empty/n");	break;}
		
	imshow("Live Stream",initframe);		//normal bgr output

			cvtColor(initframe,B_W,CV_BGR2GRAY);//b&W stream
			//imshow("greyscale",B_W);
	
			GaussianBlur(B_W,blurr,Size(9,9),1.5,1.5);//blur applied so edge detction is smoother (less hard edges)
			//imshow("blurr",blurr);

			Canny(blurr,edges,0,30,3);//edge detection
		imshow("edges",edges);
		stringstream ss;
		string name = "";
		string type = ".jpg";

		
		
		ss << name << (ct) << type;
		string filename = ss.str();
		ss.str("");
		imwrite(filename, edges);
		delay(200);
			
	if(waitKey(30)>= 0) break;	
}
		cap.release();
}else printf("Error! unable to Connect to camera\n");//outer loop for camera being on
	

 }

#endif
