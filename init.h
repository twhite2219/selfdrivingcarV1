#ifndef INIT_H
#define INIT_H
#include "distance.h"
#include "move.h"
#include <pigpio.h>
//#include <stdlib.h>
//#include <unistd.h>
#include <wiringPi.h>


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
#define M2ina 		23
//IN4... RIGHT IF HIGH
#define M2inb 		18  

//trigger a
#define TRIGA		13
//trigger b 
#define TRIGB		6
//ECHOA 
#define ECHOA		26 
//ECHOB
#define ECHOB		16


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
	delay(30);
}

#endif
