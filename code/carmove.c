#include <wiringPi.h>
#include <softPwm.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include "carmove.h"

void Move(float l, float r)
{
    if (l > 0) digitalWrite(IN1_34, LOW), digitalWrite(IN2_34, HIGH);
    else if (l < 0) digitalWrite(IN1_34, HIGH), digitalWrite(IN2_34, LOW);
    else digitalWrite(IN1_34, LOW), digitalWrite(IN2_34, LOW);
    if (r > 0) digitalWrite(IN1_12, HIGH), digitalWrite(IN2_12, LOW);
    else if (r < 0) digitalWrite(IN1_12, LOW), digitalWrite(IN2_12, HIGH);
    else digitalWrite(IN1_12, LOW), digitalWrite(IN2_12, LOW);
    softPwmWrite(PWMr, speedr * fabs(r) > 100 ? 100 : speedr * fabs(r));
    softPwmWrite(PWMl, speedl * fabs(l) > 100 ? 100 : speedl * fabs(l));
    // printf("left = %f, right = %f\n", speedr * fabs(l) > 100 ? 100 : speedr * l, speedr * fabs(r) > 100 ? 100 : speedr * r);
}

void Move_left_angle(int angle) // 左转
{
    Move(-1, 1);
    delay(angle * 550 / 90);
    Move(0, 0);
    delay(100);
}

void Move_right_angle(int angle) // 右转
{
    Move(1, -1);
    delay(angle * 550 / 90);
    Move(0, 0);
    delay(100);
}

void Balance(int left, int right) 
{
    if (abs(left - right) <= 2) return;
    if (left < right) {
        softPwmWrite(PWMr, --speedr);
        softPwmWrite(PWMl, ++speedl);
    }
    else {
        softPwmWrite(PWMr, ++speedr);
        softPwmWrite(PWMl, --speedl);
    }
    printf("left = %d, right = %d\n", speedl, speedr);
}

void carmove_init()
{
    pinMode(IN1_34, OUTPUT);
    pinMode(IN2_34, OUTPUT);
    pinMode(IN1_12, OUTPUT);
    pinMode(IN2_12, OUTPUT);
    softPwmCreate(PWMl, 0, 100);  
    softPwmCreate(PWMr, 0, 100);
    digitalWrite(IN1_34, LOW);
    digitalWrite(IN2_34, LOW);
    digitalWrite(IN1_12, LOW);
    digitalWrite(IN2_12, LOW);
}