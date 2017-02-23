
#ifndef DISTANCE_H
#define DISTANCE_H

#include "init.h"
#include "move.h"
#include <pigpio.h>
//#include <stdlib.h>
//#include <unistd.h>
#include <wiringPi.h>


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



#endif
