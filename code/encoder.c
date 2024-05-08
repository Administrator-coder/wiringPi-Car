#include <wiringPi.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "encoder.h"

volatile int encoderPos1 = 0;
volatile int encoderPos2 = 0;
volatile int encoderPos3 = 0;
volatile int encoderPos4 = 0;

void encoderA1() {
    (digitalRead(encoderPinA1) == digitalRead(encoderPinB1)) ? encoderPos1++ : encoderPos1--;
}

void encoderB1() {
    (digitalRead(encoderPinA1) != digitalRead(encoderPinB1)) ? encoderPos1++ : encoderPos1--;
}

void encoderA2() {
    (digitalRead(encoderPinA2) == digitalRead(encoderPinB2)) ? encoderPos2++ : encoderPos2--;
}

void encoderB2() {
    (digitalRead(encoderPinA2) != digitalRead(encoderPinB2)) ? encoderPos2++ : encoderPos2--;
}

void encoderA3() {
    (digitalRead(encoderPinA3) == digitalRead(encoderPinB3)) ? encoderPos3++ : encoderPos3--;
}

void encoderB3() {
    (digitalRead(encoderPinA3) != digitalRead(encoderPinB3)) ? encoderPos3++ : encoderPos3--;
}

void encoderA4() {
    (digitalRead(encoderPinA4) == digitalRead(encoderPinB4)) ? encoderPos4++ : encoderPos4--;
}

void encoderB4() {
    (digitalRead(encoderPinA4) != digitalRead(encoderPinB4)) ? encoderPos4++ : encoderPos4--;
}

void Getspeed(int *S1, int *S2, int *S3, int *S4) {
    encoderPos1 = 0;
    encoderPos2 = 0;
    encoderPos3 = 0;
    encoderPos4 = 0;
    delay(100);
    *S1 = encoderPos1;
    *S2 = encoderPos2;
    *S3 = encoderPos3;
    *S4 = encoderPos4;
    printf("S1 = %d, S2 = %d, S3 = %d, S4 = %d\n", *S1, *S2, *S3, *S4);
}

void encoder_init() {
    pinMode(encoderPinA1, INPUT);
    pinMode(encoderPinB1, INPUT);
    pinMode(encoderPinA2, INPUT);
    pinMode(encoderPinB2, INPUT);
    pinMode(encoderPinA3, INPUT);
    pinMode(encoderPinB3, INPUT);
    pinMode(encoderPinA4, INPUT);
    pinMode(encoderPinB4, INPUT);
    wiringPiISR(encoderPinA1, INT_EDGE_RISING, encoderA1);
    wiringPiISR(encoderPinB1, INT_EDGE_RISING, encoderB1);
    wiringPiISR(encoderPinA2, INT_EDGE_RISING, encoderA2);
    wiringPiISR(encoderPinB2, INT_EDGE_RISING, encoderB2);
    wiringPiISR(encoderPinA3, INT_EDGE_RISING, encoderA3);
    wiringPiISR(encoderPinB3, INT_EDGE_RISING, encoderB3);
    wiringPiISR(encoderPinA4, INT_EDGE_RISING, encoderA4);
    wiringPiISR(encoderPinB4, INT_EDGE_RISING, encoderB4);
}

bool checkSpeed() {
    int S1 = 0, S2 = 0, S3 = 0, S4 = 0;
    Getspeed(&S1, &S2, &S3, &S4);
    S1 = abs(S1), S2 = abs(S2), S3 = abs(S3), S4 = abs(S4);
    int average = (S1 + S2 + S3 + S4) / 4;
    return (abs(S1 - average) <= 30 && abs(S2 - average) <= 30 && abs(S3 - average) <= 30 && abs(S4 - average) <= 30);
}