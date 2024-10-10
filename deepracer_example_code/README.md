# Project Overview & Understand Sample Code

This project contains various C++ files that are responsible for controlling hardware components such as servos, LEDs, and sensors (LIDAR and Camera) through JSON configurations and hardware-specific drivers. The purpose of this project is to provide a comprehensive control and testing environment for hardware components in embedded systems, especially for autonomous vehicles and robotics.

## File Descriptions

### 1. **bios_version.cpp**
- **Purpose**: Manage and compare BIOS versions.
- **Key Features**:
  - The `BiosVersion` class splits a version string into major, minor, and patch numbers.
  - Implements the `>=` operator to compare two BIOS versions.
  - Fallback to default values if version parsing fails.
- **Use Case**: Ensures compatibility by checking if the system BIOS version meets a certain minimum requirement.

### 2. **jsoncpp.cpp**, **json.h**, **json-forwards.h**
- **Purpose**: Provides functionality for parsing and generating JSON data using the JsonCpp library.
- **Key Features**:
  - The `Json::Reader` class reads and parses JSON data from strings or files into `Json::Value` objects.
  - The `Json::Value` class stores and manipulates JSON data.
  - The `Json::StreamWriterBuilder` and `Json::StreamWriter` classes convert `Json::Value` objects into JSON-formatted strings or files.
- **Use Case**: Used for reading and writing configuration or state data in JSON format, which is especially useful for managing device states and settings.

### 3. **led_mgr.cpp**
- **Purpose**: Manage RGB LEDs and control their colors.
- **Key Features**:
  - The `LedMgr` class handles PWM values for three LEDs (Red, Green, Blue).
  - Reads and writes LED states to and from a JSON file.
  - `setLedValue`: Sets the PWM values for each LED color.
  - `getLedValue`: Retrieves the current state of the LEDs.
- **Use Case**: Visually represents system states, such as using color changes in LEDs to indicate different statuses.

### 4. **pwm.cpp**
- **Purpose**: Control servos and other devices using Pulse Width Modulation (PWM).
- **Key Features**:
  - The `Servo` class sets the PWM period and duty cycle (i.e., the amount of movement).
  - Directly reads and writes PWM values through the file system (`/sys/class/pwm/`).
  - `writePWM`: Writes values to a specific path, effectively controlling the device's PWM.
  - `getSysPath`: Dynamically retrieves the system path for PWM devices.
- **Use Case**: Used to control hardware devices like servos, LEDs, and more, through precise PWM control.

### 5. **servo_mgr.cpp**
- **Purpose**: Manages servos and motors, including calibration and PWM control.
- **Key Features**:
  - The `ServoMgr` class manages the period and duty cycle of the servos.
  - `setCalibrationValue` and `getCalibrationValue`: Set and retrieve the min, mid, and max calibration values for servos and motors.
  - `servoSubscriber`: Controls the servos based on input throttle and steering angle.
- **Use Case**: Provides precise control over the servos and motors in applications like steering and throttle control in autonomous vehicles.

### 6. **servo_node_test.cpp**
- **Purpose**: Test the functionality of the `ServoMgr` and `LedMgr` classes.
- **Key Features**:
  - Tests the calibration values of servos and motors by setting and printing them.
  - Repeatedly calls `getCalibrationValue` and `setCalibrationValue` to verify the correct functioning of servos and motors.
- **Use Case**: Ensures that servos and motors are functioning as expected and allows for testing and adjustment of their settings.

### 7. **utility.cpp**
- **Purpose**: Provides utility functions related to file operations.
- **Key Features**:
  - `checkFile`: Checks if a file exists.
  - `writeJSONToFile`: Writes JSON data to a file.
- **Use Case**: Simplifies file I/O operations, especially for reading and writing JSON configuration files.

### 8. **lidar_node_test.cpp**
- **Purpose**: Communicate with and process data from a LIDAR device.
- **Key Features**:
  - Uses the `ILidarDriver` class to connect to and collect data from the LIDAR device.
  - Outputs device information (model, firmware version, hardware version).
  - Prints the scanned data collected from the LIDAR sensor.
- **Use Case**: Collects environmental data from a LIDAR sensor, which is crucial for tasks such as navigation and mapping in autonomous systems.

### 9. **camera_node_test.cpp**
- **Purpose**: Capture images from cameras and process them.
- **Key Features**:
  - Uses the OpenCV library to capture frames from two cameras.
  - Compresses images using the MJPEG codec and saves them to disk.
  - Serializes and deserializes frames for potential transmission.
- **Use Case**: Captures real-time video from cameras for tasks like track recognition or object detection in autonomous driving applications.

## How to Compile and Run

### Compilation Instructions:
Make sure to have all necessary libraries (e.g., OpenCV, JsonCpp, SLAMTEC LIDAR SDK) installed on your system.

```bash
TO BE DONE!