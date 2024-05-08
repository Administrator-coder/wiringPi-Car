#ifndef ENCODER_H
#define ENCODER_H

#define encoderPinA1 8
#define encoderPinB1 9
#define encoderPinA2 16
#define encoderPinB2 15
#define encoderPinA3 13
#define encoderPinB3 12
#define encoderPinA4 5
#define encoderPinB4 6

void encoderA1();
void encoderB1();

void encoderA2();
void encoderB2();

void encoderA3();
void encoderB3();

void encoderA4();
void encoderB4();

void Getspeed(int *S1, int *S2, int *S3, int *S4);
bool checkSpeed();

void encoder_init();

#endif