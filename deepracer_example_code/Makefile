# Makefile for Camera, Lidar, and Servo Projects

# Compiler Setting
CXX = g++
CXXFLAGS = -std=c++17 -I/usr/include/opencv4/ -L/usr/lib -lopencv_core -lopencv_imgproc -lopencv_videoio -lopencv_imgcodecs -lpthread -lstdc++fs

# path setting
OPENCV_INC = -I/usr/include/opencv4/
OPENCV_LIB = -L/usr/lib -lopencv_core -lopencv_imgproc -lopencv_videoio -lopencv_imgcodecs

LIDAR_INC = -I./rplidar_sdk/sdk/include/
LIDAR_LIB = -L./rplidar_sdk/output/Linux/Release -lsl_lidar_sdk -lpthread

# Each projet file
CAMERA_SRC = camera_node_test.cpp
LIDAR_SRC = lidar_node_test.cpp
SERVO_SRC = bios_version.cpp led_mgr.cpp pwm.cpp servo_mgr.cpp servo_node_test.cpp utility.cpp jsoncpp.cpp

# Output File
CAMERA_OUT = camera_node_test
LIDAR_OUT = lidar_node_test
SERVO_OUT = servo_node_test

# Basic Target
all: camera lidar servo

# Camera file build and run
camera: $(CAMERA_OUT)

$(CAMERA_OUT): $(CAMERA_SRC)
	$(CXX) $(CXXFLAGS) $(CAMERA_SRC) -o $(CAMERA_OUT) $(OPENCV_LIB)

camera_run: $(CAMERA_OUT)
	sudo ./$(CAMERA_OUT)

# Lidar project build and run
lidar: $(LIDAR_OUT)

$(LIDAR_OUT): $(LIDAR_SRC)
	$(CXX) $(CXXFLAGS) $(LIDAR_SRC) -o $(LIDAR_OUT) $(LIDAR_INC) $(LIDAR_LIB)

lidar_run: $(LIDAR_OUT)
	sudo ./$(LIDAR_OUT)

# Servo project build and run
servo: $(SERVO_OUT)

$(SERVO_OUT): $(SERVO_SRC)
	$(CXX) $(CXXFLAGS) $(SERVO_SRC) -o $(SERVO_OUT)

servo_run: $(SERVO_OUT)
	sudo ./$(SERVO_OUT)

# Clean Set
clean:
	rm -f $(CAMERA_OUT) $(LIDAR_OUT) $(SERVO_OUT)

