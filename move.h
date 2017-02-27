#ifndef MOVE_H
#define MOVE_H

#include "distance.h"
#include "network.h"
#include "init.h"
#include <iostream>
#include <pigpio.h>
//#include <stdlib.h>
//#include <unistd.h>
#include <wiringPi.h>


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
	
	pwmWrite(motcont1,powerON);
	pwmWrite(motcont2,powerON);
		
	if(f) { digitalWrite(M1ina,HIGH); }
	else if(b) { digitalWrite(M1inb,HIGH); }
	if(l) { digitalWrite(M2ina,HIGH); }
	else if(r) { digitalWrite(M2inb,HIGH); }
	
	delay(msecs);
	
	digitalWrite(M1ina,LOW);
	digitalWrite(M1inb,LOW);
	digitalWrite(M2ina,LOW);
	digitalWrite(M2inb,LOW);	
	
	pwmWrite(motcont1,powerOFF);
	pwmWrite(motcont2,powerOFF);
	
	delay(50);
}

void TestMotor(){
	printf("fwd\n");
	move(MT_FORWARD, 3000);
	printf("bwd\n");
	move(MT_BACKWARD, 3000);
	printf("rght\n");
		move(MT_RIGHT, 3000);
	printf("left\n");
	move(MT_LEFT, 3000);
	printf("fwd, right\n");
	move(MT_FORWARD | MT_RIGHT, 3000);
	printf("fwd left\n");
	move(MT_FORWARD | MT_LEFT, 3000);
	
	
}








#endif
