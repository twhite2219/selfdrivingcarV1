#include <stdio.h>
#include <pigpio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wiringPi.h>
#include <softPwm.h>
#include "network.h"
#include "move.h"
#include "init.h"
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
	

Mat initframe,blurr,B_W,edges,reSized,reShaped,ReadImg,FinalImg;
VideoCapture cap;
Size size(10,10);
cap.open(0);
cap.set(CAP_PROP_FRAME_WIDTH,50);
cap.set(CAP_PROP_FRAME_HEIGHT,50);


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
				
				GaussianBlur(B_W,blurr,Size(9,9),1.5,1.5);//blur applied so edge detction is smoother (less hard edges)
		
				Canny(blurr,edges,0,30,3);//edge detection
				imshow("edges",edges);
				
					if (ObjectDetection() ==0){//within loop where processing can occur
						fflush(stdout);
						printf("\rProcessing available                           ");
						cv::imwrite("currImg.jpg", edges);
				
				
						ReadImg = cv::imread("currImg.jpg",0);
						if (!ReadImg.data)
						cout << "error no file found " << endl;
		
						resize(ReadImg,reSized,size);//resize to 10x10
						cout <<reSized.size()<<endl;
						//cout <<reSized<<endl;
					
					reShaped = reSized.reshape(1,1);//converts image row by row to 1 by x res
					cout <<reShaped.size()<<endl;
					//cout <<reShaped<<endl;
					
					reShaped.convertTo(FinalImg,CV_32F);// 
					cout <<FinalImg.size()<<endl;
					//cout <<FinalImg<<endl;
					
					
					Mat Result;
					Neural_Net->predict(FinalImg,Result);
					cout << Result << endl;	
					cv::Point max_loc;
					cv::minMaxLoc(Result,0,0,0,&max_loc);
					printf("Test Result : %i",max_loc.x);  
					
					switch(max_loc.x)
					{
					
				case 0 :

				printf("fwd left\n");
				move(MT_FORWARD| MT_LEFT, 200);
				break;

			
				case 1 :
				printf("fwd, right\n");
				move(MT_FORWARD | MT_RIGHT, 200);
				break;
			
				case 2 :
				printf("fwd\n");
				move(MT_FORWARD | MT_LEFT, 200);
				break;

				default : printf("no value found");
				break; 	
						
						
					}
					
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
	printf("||5.Auto Drive    || 6.Train+TestNN||\n");
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
TestNetwork();
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
 
