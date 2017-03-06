
#ifndef DISTANCE_H
#define DISTANCE_H
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


//trigger a
#define TRIGA		13
//trigger b 
#define TRIGB		6
//ECHOA 
#define ECHOA		26 
#define ECHOB   	16


int DistanceSenseA(){
	digitalWrite(TRIGA,HIGH);
	delayMicroseconds(20);
	digitalWrite(TRIGA,LOW);
	long sendA,recvA;
	
	while(digitalRead(ECHOA)== LOW){
	sendA = micros();
	}
	
	while(digitalRead(ECHOA)==HIGH){
	recvA =micros()-sendA;
	}
	
	int distance= recvA/58;
	
	delay(30);
	return distance;
}

int DistanceSenseB(){
	digitalWrite(TRIGB,HIGH);
	delayMicroseconds(20);
	digitalWrite(TRIGB,LOW);
	long sendB,recvB;
	
	while(digitalRead(ECHOB)== LOW){
	sendB = micros();
	}
	
	while(digitalRead(ECHOB)==HIGH){
	recvB =micros()-sendB;
	}
	
	int distance= recvB/58;
	
	delay(30);
	return distance;
}
void TestSensors(){
	
	int A= (DistanceSenseA());
	int B= (DistanceSenseB());
	while(A>=5 && B>=5){
	A= (DistanceSenseA());
	B= (DistanceSenseB());
	printf("\r||DistanceA: %icm ||DistanceB: %icm||     \r",A,B);		
	fflush(stdout);
	delay(800);
}
	printf("\r||DistanceA: %icm  ||DistanceB: %icm ||\n",A,B);	
	printf("||Object Identified within 5cm!    ||\n");
	printf("||     REVERTING TO MAIN MENU      ||\n");
}

int ObjectDetection(){
	int A= (DistanceSenseA());
	int B= (DistanceSenseB());
	if(A<15 || B<15)
	return 1;
	else return 0;
	} 
	



#endif
