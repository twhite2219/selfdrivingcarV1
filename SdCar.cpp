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
#include "PiCamF.h"
using namespace cv;


int AutoDrive(){
	

Mat initframe,blurr,B_W,edges,image,flat_Img,arr_Img;
VideoCapture cap;
cap.open(0);
cap.set(CAP_PROP_FRAME_WIDTH,20);
cap.set(CAP_PROP_FRAME_HEIGHT,20);

	
 if (cap.isOpened()){ 
	for(;;){
		cap.read(initframe);
		if (initframe.empty()){	printf("error frame empty/n");	break;}
		
			imshow("Live Stream",initframe);		//normal bgr output

			cvtColor(initframe,B_W,CV_BGR2GRAY);//b&W stream
			//imshow("greyscale",B_W);
	
			GaussianBlur(B_W,blurr,Size(9,9),1.5,1.5);//blur applied so edge detction is smoother (less hard edges)
			//imshow("blurr",blurr);

			Canny(blurr,edges,0,30,3);//edge detection
			imshow("edges",edges);
			
			if (ObjectDetection() ==0){//within loop where processing can occur
				fflush(stdout);
				printf("\rProceesing available                           ");
			
				cap.read(edges);//take frame from edges
				flat_Img = edges.reshape(1,1);//converts image row by row to 1 by x res
				flat_Img.convertTo(arr_Img,CV_32FC3,1/255.0); //converts 1 by x imaeg to an array  
				imshow("arr_img",arr_Img);//show array values on screen
				
				
				
	
				if(waitKey(30)>= 0) break;
			}else {fflush(stdout); printf("\rObject Identified within 15cm, Waiting...");}//main object detection loop which will print error until objet is removed
	
	if(waitKey(30)>= 0) break;	
	}//for loop for processing part being run, terminates on keypress
		cap.release();
}else printf("Error! unable to Connect to camera\n");//outer loop for camera being on
	
	return 0;
}
 

void SysMenu(){
	int choice=0;
	while(choice !=5){
	system("clear");
	printf("||Please Choose An Option..........||\n");
	printf("||1.Test Motors   || 2.Test Sensors||\n");
	printf("||3.Test Camera   || 4.Drive Mode  ||\n");
	printf("||5.Exit          ||               ||\n");
	scanf("%i",&choice);
	
	switch(choice)
{
case 1: 
    TestMotor();
	delay(3000);
    break;

case 2 : 
   TestSensors();
    delay(3000);
    break;
    
case 3 :
TestCamera();
	break;

case 4 :
AutoDrive();
break;

case 5 :
break;


default : printf("Input, Option Not available, Please retry");
    break; 
}
}
}
int main(int, char**) {
initialisePins();
SysMenu();
gpioTerminate();
 return 0;
}
 
