#include "servo/servo_mgr.hpp"
#include "servo/led_mgr.hpp"
#include <opencv2/opencv.hpp>

int main() {
    cv::VideoCapture cap(0); 
    if (!cap.isOpened()) {
        std::cerr << "Can't open Camera" << std::endl;
        return -1;
    }

    PWM::ServoMgr servoMgr;
    
    // Motor speed and angle init
    float motorSpeed = 0.1;  // operate speed
    float steeringAngle = 0;  // init angle value
    
    /**** Main Loop ****/ 
    while (true) {
        cv::Mat frame;
        cap >> frame;
        
        cv::Mat gray, binary;
        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
        cv::threshold(gray, binary, 100, 255, cv::THRESH_BINARY);
        
        int deviation = calculateDeviationFromCenter(binary);
        
        if (deviation > 10) {
            steeringAngle = adjustSteering(deviation);
            servoMgr.servoSubscriber(motorSpeed, steeringAngle);
        }

        // Set wheel angle and go ahead
        servoMgr.servoSubscriber(motorSpeed, 0);  // No angle

        // ESC : ENDKEY
        if (cv::waitKey(30) == 27) break;
    }
    
    cap.release();
    return 0;
}

/* Function : find center */
int calculateDeviationFromCenter(const cv::Mat& binaryImage) {
    // Find track center in bottom area
    int height = binaryImage.rows;
    int width = binaryImage.cols;

    // Usage Image 1/4 Area
    cv::Mat bottomPart = binaryImage(cv::Rect(0, height * 3 / 4, width, height / 4));

    // Find while pixel (track color should be set)
    std::vector<cv::Point> points;
    cv::findNonZero(bottomPart, points);

    // Error : When fail to find center of track -> return 0 for no angle set
    if (points.empty()) {
        return 0;
    }

    // Find track center by white pixel x-coordinate average
    int trackCenter = 0;
    for (const auto& point : points) {
        trackCenter += point.x;
    }
    trackCenter /= points.size();

    // Image center
    int vehicleCenter = width / 2;

    // Find Track (deepracer) center and image center difference
    int deviation = trackCenter - vehicleCenter;
    
    return deviation;
}

// Adjust angle based by difference
float adjustSteering(int deviation) {

    float maxSteeringAngle = 30.0f;  // Max angle value
    
    // Calculate angle (@TODO : ratio change)
    float steeringAngle = (deviation / 100.0f) * maxSteeringAngle;
    
    // Limit angle
    if (steeringAngle > maxSteeringAngle) {
        steeringAngle = maxSteeringAngle;
    } else if (steeringAngle < -maxSteeringAngle) {
        steeringAngle = -maxSteeringAngle;
    }
    
    return steeringAngle;
}