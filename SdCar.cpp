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
    TestMotor();
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
 
