#include <stdio.h>
#include <pigpio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wiringPi.h>
#include <softPwm.h>
#include "move.h"
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

void AutoDrive(){
	
}

void SysMenu(){
	system("clear");
int choice;
	printf("||Please Choose An Option..........||\n");
	printf("||1.Test Motors   || 2.Test Sensors||\n");
	printf("||3.Test Camera   || 4.Drive Mode  ||\n");
	printf("||5.Exit          ||               ||\n");
	scanf("%i",&choice);
	
	switch(choice)
{
case 1: 
    move(MT_FORWARD, 3000);
	printf("fwd\n");
	move(MT_BACKWARD, 3000);
	printf("bwd\n");
	move(MT_RIGHT, 3000);
	printf("rght\n");
	move(MT_LEFT, 3000);
	printf("left\n");
	move(MT_FORWARD | MT_RIGHT, 3000);
	printf("fwd, right\n");
	move(MT_FORWARD | MT_LEFT, 3000);
	printf("fwd left\n");
	delay(3000);
	SysMenu();
    break;

case 2 : 
   TestSensors();
    delay(3000);
    SysMenu();
    break;
    
case 3 :
TestCamera();
	SysMenu();
	break;

case 4 :
AutoDrive();
break;

case 5 :
break;


default : printf("Input, Option Not available, Please retry");
    SysMenu();
    break;  
}
}


int main(int, char**) {
initialisePins();
SysMenu();
gpioTerminate();
 return 0;
}
 
