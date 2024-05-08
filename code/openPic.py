import wiringpi
import time

# 定义GPIO引脚
IN1_12 = 7
IN2_12 = 0
IN1_34 = 2
IN2_34 = 3
PWMr = 1
PWMl = 4

# 初始化WiringPi
wiringpi.wiringPiSetup()

# 设置GPIO引脚为输出模式
wiringpi.pinMode(IN1_12, 1)
wiringpi.pinMode(IN2_12, 1)
wiringpi.pinMode(IN1_34, 1)
wiringpi.pinMode(IN2_34, 1)

# 设置PWM引脚为PWM输出模式
wiringpi.pinMode(PWMr, wiringpi.PWM_OUTPUT)
wiringpi.pinMode(PWMl, wiringpi.PWM_OUTPUT)

# 设置PWM模式为MS模式
wiringpi.pwmSetMode(wiringpi.PWM_MODE_MS)

# 设置PWM的范围
wiringpi.pwmSetRange(100)

def forward():
    wiringpi.digitalWrite(IN1_12, 1)
    wiringpi.digitalWrite(IN2_12, 0)
    wiringpi.digitalWrite(IN1_34, 1)
    wiringpi.digitalWrite(IN2_34, 0)
    wiringpi.pwmWrite(PWMr, 75)  # 75% duty cycle
    wiringpi.pwmWrite(PWMl, 75)  # 75% duty cycle

def backward():
    wiringpi.digitalWrite(IN1_12, 0)
    wiringpi.digitalWrite(IN2_12, 1)
    wiringpi.digitalWrite(IN1_34, 0)
    wiringpi.digitalWrite(IN2_34, 1)
    wiringpi.pwmWrite(PWMr, 75)  # 75% duty cycle
    wiringpi.pwmWrite(PWMl, 75)  # 75% duty cycle

def left():
    wiringpi.digitalWrite(IN1_12, 0)
    wiringpi.digitalWrite(IN2_12, 1)
    wiringpi.digitalWrite(IN1_34, 1)
    wiringpi.digitalWrite(IN2_34, 0)
    wiringpi.pwmWrite(PWMr, 75)  # 75% duty cycle
    wiringpi.pwmWrite(PWMl, 75)  # 75% duty cycle

def right():
    wiringpi.digitalWrite(IN1_12, 1)
    wiringpi.digitalWrite(IN2_12, 0)
    wiringpi.digitalWrite(IN1_34, 0)
    wiringpi.digitalWrite(IN2_34, 1)
    wiringpi.pwmWrite(PWMr, 75)  # 75% duty cycle
    wiringpi.pwmWrite(PWMl, 75)  # 75% duty cycle

def stop():
    wiringpi.digitalWrite(IN1_12, 0)
    wiringpi.digitalWrite(IN2_12, 0)
    wiringpi.digitalWrite(IN1_34, 0)
    wiringpi.digitalWrite(IN2_34, 0)
    wiringpi.pwmWrite(PWMr, 0)  # 0% duty cycle
    wiringpi.pwmWrite(PWMl, 0)  # 0% duty cycle

# 测试小车的移动
forward()
time.sleep(2)
backward()
time.sleep(2)
left()
time.sleep(2)
right()
time.sleep(2)
stop()
