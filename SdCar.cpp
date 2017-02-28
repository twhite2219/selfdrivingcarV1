#include <stdio.h>
#include <pigpio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wiringPi.h>
#include <softPwm.h>
#include "network.h"
#include "move.h"
#include <iostream>
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
#include <opencv2/ml.hpp>
#include "PiCamF.h"
using namespace cv;
using namespace std;

 
 
 
int AutoDrive(){
	

Mat initframe,blurr,B_W,edges,image,flat_Img,arr_Img;
VideoCapture cap;
cap.open(0);
cap.set(CAP_PROP_FRAME_WIDTH,10);
cap.set(CAP_PROP_FRAME_HEIGHT,10);


FileStorage fs("NNPARAMS.xml",FileStorage::READ);
Ptr<ml::ANN_MLP> Neural_Net = cv::Algorithm::read<ml::ANN_MLP>(fs.root());
	if (!Neural_Net->isTrained()) printf("network not trained \n");
	else {
		
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
					printf("\rProcessing available                           ");
				
					cap.read(edges);//take frame from edges
					flat_Img = edges.reshape(1,1);//converts image row by row to 1 by x res
					
					
					flat_Img.convertTo(arr_Img,CV_32F);//,1/255.0); //converts 1 by x imaeg to an array  
					imshow("arr_img",arr_Img);//show array values on screen
					
					//std::vector<uchar> imageArr;//20x20 resolution = 400 bits 
					//if(arr_Img.isContinuous())
						//imageArr.assign(arr_Img.datastart, arr_Img.dataend);
					//else {
						//imageArr.insert(imageArr.end(),arr_Img.ptr<uchar>(0),arr_Img.ptr<uchar> (0)+arr_Img.cols);
					
						//}
						
						Mat Result;
						Mat CurrentImg = Mat::ones(arr_Img.rows,arr_Img.cols, CV_32FC1);
						Neural_Net->predict(CurrentImg,Result);
						cout << Result << endl;
					
	
					if(waitKey(30)>= 0) break;
				}else {fflush(stdout); printf("\rObject Identified within 15cm, Waiting...");}//main object detection loop which will print error until objet is removed
	
	if(waitKey(30)>= 0) break;	
	}//for loop for processing part being run, terminates on keypress
		cap.release();
}else printf("Error! unable to Connect to camera\n");//outer loop for camera being on
}
	return 0;
}
 
void SysMenu(){
	int choice=0;
	while(choice !=8){
	system("clear");
	printf("||Please Choose An Option..........||\n");
	printf("||1.Test Motors   || 2.Test Sensors||\n");
	printf("||3.Test Camera   || 4.Take tImages||\n");
	printf("||5.Auto Drive    || 6.Test NN     ||\n");
	printf("||7.XML.store     || 8.Exit        ||\n");
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
takephotos();
break;

case 5 :
AutoDrive();
break;

case 6 :
trainNetwork();
printf("end");
break;

case 7 :
readScanStore();
break;

case 8 :
break;

default : printf("Input, Option Not available, Please retry");
    break; 
}
}
}

int main(int argc,char** argv) {
initialisePins();
SysMenu();
gpioTerminate();


 return 0;
}
 
