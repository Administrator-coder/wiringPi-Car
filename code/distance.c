#include <wiringPi.h>
#include <softPwm.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include "distance.h"

float ur_disMeasure() {
	struct timeval ur_tv1;
	struct timeval ur_tv2;
	long ur_time1, ur_time2;
	float ur_dis;
	digitalWrite(makerobo_Trig, LOW);
	delayMicroseconds(2);
	digitalWrite(makerobo_Trig, HIGH);
	delayMicroseconds(10);
	digitalWrite(makerobo_Trig, LOW);
	while(!(digitalRead(makerobo_Echo) == 1));
	gettimeofday(&ur_tv1, NULL);
	while(!(digitalRead(makerobo_Echo) == 0));
	gettimeofday(&ur_tv2, NULL);
	ur_time1 = ur_tv1.tv_sec * 1000000 + ur_tv1.tv_usec;
	ur_time2 = ur_tv2.tv_sec * 1000000 + ur_tv2.tv_usec;
	ur_dis = (float)(ur_time2 - ur_time1) / 1000000 * 34000 / 2;
    printf("Distance = %0.2f cm\n", ur_dis);
	return ur_dis;
}

void distance_init() {
	pinMode(makerobo_Echo, INPUT);
	pinMode(makerobo_Trig, OUTPUT);
}