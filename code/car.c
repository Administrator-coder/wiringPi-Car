#include <wiringPi.h>
#include <softPwm.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "carmove.h"
#include "distance.h"
#include "encoder.h"
#include "autotrack.h"
#include <string.h>

int speedl = 50, speedr = 50;

bool kbhit()  //判断键盘是否有输入
{
    struct timeval tv;
    fd_set fds;
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    FD_ZERO(&fds);
    FD_SET(0, &fds);
    select(1, &fds, NULL, NULL, &tv);
    return FD_ISSET(0, &fds);
}

void GPIO_init()
{
    if (wiringPiSetup() == -1) exit(0);
    distance_init();
    carmove_init();
    encoder_init();
}

void Manual_mode()  //手动模式
{
    speedl = 30, speedr = 30;
    printf("\033[2J\033[1;1H");
    printf("Manual mode\n");
    printf("w: Move forward\n");
    printf("s: Move backward\n");
    printf("a: Move left\n");
    printf("d: Move right\n");
    printf("q: Stop\n");
    printf("o: Exit\n");

    unsigned char lastch = ' ', curch = ' ';
    int l = 0, r = 0;
    while (1)
    {
        if (kbhit()) curch = getchar();
        if (curch == 'w' && lastch != 'w') Move(l = 1, r = 1);
        else if (curch == 'x' && lastch != 'x') Move(l = -1, r = -1);
        else if (curch == 'a' && lastch != 'a') Move(l = -1, r = 1);
        else if (curch == 'd' && lastch != 'd') Move(l = 1, r = -1);
        else if (curch == 'q' && lastch != 'q') {Move(0, 0); delay(500); Move(l = 0, r = 1);}
        else if (curch == 'e' && lastch != 'e') {Move(0, 0); delay(500); Move(l = 1, r = 0);}
        else if (curch == 'z' && lastch != 'z') {Move(0, 0); delay(500); Move(l = 0, r = -1);}
        else if (curch == 'c' && lastch != 'c') {Move(0, 0); delay(500); Move(l = -1, r = 0);}
        else if (curch == 's' && lastch != 's') Move(l = 0, r = 0);
        else if (curch == 'o') {
            Move(l = 0, r = 0);
            return;
        }
        // printf("l = %d, r = %d\n", l, r);
        if (curch == 'w'|| curch == 'x') {
            int S1 = 0, S2 = 0, S3 = 0, S4 = 0;
            Getspeed(&S1, &S2, &S3, &S4);
            int right_speed = abs((S1 + S2) / 2);
            int left_speed = abs((S3 + S4) / 2);
            printf("left_speed = %d, right_speed = %d\n", left_speed, right_speed);
            Balance(left_speed, right_speed);
        }
        if (curch == 'q' || curch == 'e' || curch == 'z' || curch == 'c') {
            int S1 = 0, S2 = 0, S3 = 0, S4 = 0;
            Getspeed(&S1, &S2, &S3, &S4);
            int right_speed = abs((S1 + S2) / 2);
            int left_speed = abs((S3 + S4) / 2);
            printf("left_speed = %d, right_speed = %d\n", left_speed, right_speed);
            if (l == 0 && left_speed != 0) 
            {
                if (left_speed > 0) digitalWrite(IN1_34, HIGH), digitalWrite(IN2_34, LOW), softPwmWrite(PWMl, left_speed * 0.1);
                else if (left_speed < 0) digitalWrite(IN1_34, LOW), digitalWrite(IN2_34, HIGH), softPwmWrite(PWMl, left_speed * -0.1);
                else softPwmWrite(PWMl, 0);
            }
            if (r == 0 && right_speed != 0)
            {
                if (right_speed > 0) digitalWrite(IN1_12, HIGH), digitalWrite(IN2_12, LOW), softPwmWrite(PWMr, right_speed * 0.1);
                else if (right_speed < 0) digitalWrite(IN1_12, LOW), digitalWrite(IN2_12, HIGH), softPwmWrite(PWMr, right_speed * -0.1);
                else softPwmWrite(PWMr, 0);
            }
        }
        lastch = curch;
    }
    
}

void Ultrasonic_obstacle_avoidance_mode()  //超声波避障模式
{
    speedl = 40, speedr = 40;
    //清屏 并给出提示
    printf("\033[2J\033[1;1H");
    printf("Ultrasonic obstacle avoidance mode\n");
    printf("w: Move forward\n");
    printf("s: Stop\n");
    printf("o: Exit\n");
    unsigned char lastch = ' ', curch = ' ';
    while (1)
    {
        curch = getchar();
        if (curch == 'w' && lastch != 'w') {
            Move(1, 1);
            unsigned long long presec = 0;
            float predis = 0;
            while (1)
            {
                if (kbhit())
                {
                    curch = getchar();
                    if (curch == 's') { Move(0, 0); break; }
                    else if (curch == 'o') { Move(0, 0); return; }
                }
                float dis = ur_disMeasure();
                struct timeval tv;
                gettimeofday(&tv, NULL);
                unsigned long long sec = (unsigned long long)(tv.tv_sec) * 1000 + (unsigned long long)(tv.tv_usec) / 1000;
                if (sec - presec > 1500) presec = sec, predis = dis;
                if (dis < 30 /*|| sec - presec > 2000 && abs(dis - predis) < 2*/ || !checkSpeed()) {
                    // printf("Obstacle detected! %lld %lld\n", sec, presec);
                    Move(-1, -1);
                    delay(1000);
                    Move_left_angle(45);
                    Move(1, 1);
                }
                delay(100);
            }
        }
        else if (curch == 's') { Move(0, 0); }
        else if (curch == 'o') { Move(0, 0); return; }
        lastch = curch;
    }

}

// PID constants
#define Kp 1.5    
#define Ki 0   
#define Kd 0.1   

float er[8] = {3.5, 1.2, 0.7, 0.4, -0.4, -0.7, -1.2, -3.5};

float calculateError(int *s) {
    float error = 0;
    float sum = 0;
    for (int i = 0; i < 8; i++) 
        if (s[i] == 0) error += er[i], sum++;
    
    if (sum == 0) return -100;
    for (int i = 0; i < 8; ++i)
        printf("%d ", s[i]);
    printf("\n");
    return error / sum;
}

void Auto_tracking_mode()  //自动循迹模式
{
    // PID variables
    float lastError = 0;
    float integral = 0;
    speedl = 70, speedr = 70;
    int presec = 0;
    int fx = 0;
    while (1)
    {
        if (kbhit()) {Move(0, 0); break;}
        int sensorValues[8];
        Get_sensor_value(sensorValues);
        float error = calculateError(sensorValues);
        if (error == -100)
        {
            while (1)
            {
                if (kbhit()) {Move(0, 0); break;}
                Get_sensor_value(sensorValues);
                float error = calculateError(sensorValues);
                if (error != -100) break;
                Move(fx, -fx);
            }
            continue;
        }
        if (!(sensorValues[0] | sensorValues[1])) fx = -1;
        if (!(sensorValues[6] | sensorValues[7])) fx = 1;
        float derivative = error - lastError;
        integral += error;
        float speedDifference = Kp * error + Ki * integral + Kd * derivative;
        // printf("error = %f, speedDifference = %f\n", error, speedDifference);
        float leftSpeed = 1.0 - speedDifference;
        float rightSpeed = 1.0 + speedDifference;
        if (leftSpeed > 1) leftSpeed = 1;
        if (rightSpeed > 1) rightSpeed = 1;
        if (leftSpeed < -1) leftSpeed = -1;
        if (rightSpeed < -1) rightSpeed = -1;
        Move(leftSpeed, rightSpeed);
        lastError = error;
        //  delay(5);
    }
}

void RedGreenLight() {
    int ls = -1;
    speedl = 30, speedr = 30;
    while (1) {
        if (kbhit()) {Move(0, 0); break;}
        char color[20];  // 创建一个足够大的字符串来存储颜色
        FILE *file = fopen("res.txt", "r");  // 打开文件
        // 读取文件内容到字符串中
        if (fgets(color, sizeof(color), file) != NULL) {
            printf("Detected color: %s\n", color);
        } else {
            printf("Could not read from file\n");
        }
        fclose(file);  // 关闭文件
        if (color[0] == 'R') {
            printf("Red light\n");
            Move(0, 0);
        }
        else if (color[0] == 'G' || color[0] == 'E') {
            printf("Green light\n");
            Move(1, 1);
            // printf("%d\n", ls);
        }
        delay(100);
        // if (ls == 0) Move(1, 1);
        // else if (ls == 1) Move(0, 0);
        // printf("%d\n", ls);
    }

}

int main()
{
    struct termios old_tio, new_tio;
    tcgetattr(STDIN_FILENO, &old_tio);  // 获取终端参数
    new_tio = old_tio;  // 设置新的终端参数
    new_tio.c_lflag &=(~ICANON & ~ECHO);  // 关闭ICANON（规范模式）和ECHO
    tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);  // 设置新的终端参数
    GPIO_init();
    unsigned char option;
    do
    {
        printf("\033[2J\033[1;1H");
        printf("Please select the mode:\n");
        printf("1. Manual mode\n");
        printf("2. Ultrasonic obstacle avoidance mode\n");
        printf("3. Automatic tracking mode\n");
        printf("4. RedGreenLight\n");
        printf("5. Exit\n");

        option = getchar();
        switch (option)
        {
            case '1': Manual_mode(); break;
            case '2': Ultrasonic_obstacle_avoidance_mode(); break;
            case '3': Auto_tracking_mode(); break;
            case '5': tcsetattr(STDIN_FILENO, TCSANOW, &old_tio); return 0; // 恢复原来的终端参数
            case '4': RedGreenLight(); break;
            default : printf("Invalid option!\n"); delay(2000); break;
        }
    } while (1);
    return 0;
}