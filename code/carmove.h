#ifndef CARMOVE_H
#define CARMOVE_H

#define IN1_12 7
#define IN2_12 0
#define IN1_34 2
#define IN2_34 3
#define PWMr 1
#define PWMl 4

extern int speedl, speedr;

void Move(float l, float r); // 移动
void Move_right_angle(int angle); // 右转
void Move_left_angle(int angle); // 左转
// void Move_forward_left(int angle); // 左前
void Balance(int left, int right); // 平衡

void carmove_init();

#endif