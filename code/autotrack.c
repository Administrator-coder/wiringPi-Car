#include <wiringPi.h>
#include <softPwm.h>
#include <stdio.h>
#include "autotrack.h"

int sensor[8] = {27, 25, 24, 23, 22, 21, 30, 14};

void autotrack_init()
{
    for (int i = 0; i < 8; ++i)
        pinMode(sensor[i], INPUT);
}

void Get_sensor_value(int *sensor_value)
{
    for (int i = 0; i < 8; ++i)
        sensor_value[i] = digitalRead(sensor[i]);
    int sum = 0;
    for (int i = 0; i < 8; ++i)
        sum += sensor_value[i];
    if (sum == 8)
        return;
    for (int i = 0; i < 8; ++i)
        printf("%d ", sensor_value[i]);
    printf("\n");
}