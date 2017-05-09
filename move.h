#ifndef MOVE_H
#define MOVE_H
#include <stdio.h>
#include <pigpio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wiringPi.h>
#include <softPwm.h>
#include "network.h"
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



//EN POWER SUPPLY
#define powerON		1000
//EN Power supply
#define powerOFF	0
//EN1
#define motcont1	2
//IN1...FWD IF HIGH
#define M1ina		3
//IN2...BACK IF HIGH
#define M1inb 		4
//EN2
#define motcont2	14
//IN3... LEFT IF HIGH
#define M2ina 		18
//IN4... RIGHT IF HIGH
#define M2inb 		23  

//trigger a
#define TRIGA		13
//trigger b 
#define TRIGB		6
//ECHOA 
#define ECHOA		26 
#define ECHOB   	16

void initialisePins(){ 
	wiringPiSetupGpio();
	pinMode(motcont1,PWM_OUTPUT);//EN1
	pinMode(M1ina,OUTPUT);//IN1
	pinMode(M1inb,OUTPUT);//IN2
	
	pinMode(motcont2,PWM_OUTPUT);//EN2
	pinMode(M2ina,OUTPUT);//IN3
	pinMode(M2inb,OUTPUT);//IN4
	
	pinMode(TRIGA,OUTPUT);   
	pinMode(TRIGB,OUTPUT);
	pinMode(ECHOA, INPUT);
	pinMode(ECHOB,INPUT);
	digitalWrite(TRIGA,LOW);
	digitalWrite(TRIGB,LOW);
	
	digitalWrite(M1ina,LOW);
	digitalWrite(M1inb,LOW);
	digitalWrite(M2ina,LOW);
	digitalWrite(M2inb,LOW);
	
	delay(100);
}


enum MOVE_TYPE
{
	MT_NONE		=	0,
	MT_RESET	=	0,
	MT_FORWARD	=	1,
	MT_BACKWARD =	2,
	MT_LEFT		=	4,
	MT_RIGHT	=	8,
};


	

int isset(unsigned int flags, MOVE_TYPE bit)
{
	return (flags & bit) > 0 ? 1 : 0;
}



void move(unsigned int flags, int msecs)
{
	int f = isset(flags, MT_FORWARD),
		b = isset(flags, MT_BACKWARD),
		l = isset(flags, MT_LEFT),
		r = isset(flags, MT_RIGHT);
	
	
	digitalWrite(motcont1,HIGH);
	digitalWrite(motcont2,HIGH);
	//pwmWrite(motcont1,powerON);
	//pwmWrite(motcont2,powerON);
		
	if(f) { digitalWrite(M1ina,HIGH); }
	else if(b) { digitalWrite(M1inb,HIGH); }
	if(l) { digitalWrite(M2ina,HIGH); }
	else if(r) { digitalWrite(M2inb,HIGH); }
	delay(msecs);
	
}

void resetMotors(){
	
	
	digitalWrite(M1ina,LOW);
	digitalWrite(M1inb,LOW);
	digitalWrite(M2ina,LOW);
	digitalWrite(M2inb,LOW);	
	digitalWrite(motcont1,LOW);
	digitalWrite(motcont2,LOW);
	
	//pwmWrite(motcont1,powerOFF);
	//pwmWrite(motcont2,powerOFF);
	
	delay(200);
}
void TestMotor(){
	printf("fwd\n");
	move(MT_FORWARD,3000);
	resetMotors();
	printf("bwd\n");
	move(MT_BACKWARD, 3000);
	resetMotors();
	printf("rght\n");
		move(MT_RIGHT, 3000);
	resetMotors();
	printf("left\n");
	move(MT_LEFT, 3000);
	resetMotors();
	printf("fwd, right\n");
	move(MT_FORWARD | MT_RIGHT, 3000);
	resetMotors();
	printf("fwd left\n");
	move(MT_FORWARD | MT_LEFT, 3000);
	resetMotors();
	
}








#endif
