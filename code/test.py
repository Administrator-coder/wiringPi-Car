from picamera2 import Picamera2
import cv2
import numpy as np
import time

# 初始化
picam2 = Picamera2() # 创建摄像头对象开启摄像头，只能创建一次
picam2.start() # 开启摄像头，无需重复开启
time.sleep(1)

# 设定一个阈值，只有当掩码中的像素点数量超过这个阈值时，才认为检测到了大片的颜色
threshold = 100000

while True:
    # 抓拍
    array = picam2.capture_array("main") # 捕获一帧相机数据，输出为numpy.ndarray类型，与opencv无缝连接

    # 转换到 HSV 颜色空间
    hsv = cv2.cvtColor(array, cv2.COLOR_BGR2HSV)

    # 定义红色和绿色的阈值
    # 注意，这些阈值可能需要根据你的摄像头和环境进行调整
    # red_lower = np.array([100, 150, 0])
    # red_upper = np.array([140, 255, 255])
    green_lower = np.array([50, 100, 100])
    green_upper = np.array([70, 255, 255])

    # 创建红色和绿色的掩码
    red_lower = np.array([110, 150, 150])
    red_upper = np.array([130, 255, 255])
    red_mask = cv2.inRange(hsv, red_lower, red_upper)
    green_mask = cv2.inRange(hsv, green_lower, green_upper)

    # 如果掩码中的像素点数量超过阈值，那么就检测到了对应的颜色
    if np.sum(red_mask) > threshold:
        color = "Red"
    elif np.sum(green_mask) > threshold:
        color = "Green"
    else:
        color = "Empty"

    # 将结果写入文件，覆盖旧内容
    with open('res.txt', 'w') as f:
        f.write(color)

    # 按 'q' 键退出循环
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break
    
cv2.destroyAllWindows() # 关闭所有 OpenCV 窗口
picam2.stop() # 关闭摄像头
