#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <opencv2/opencv.hpp>

#include "sensor/aa/port/rawdata.h"

#define FRAME_WIDTH 160
#define FRAME_HEIGHT 120
#define TOTOAL_FIXEL FRAME_WIDTH * FRAME_HEIGHT

using namespace cv;

class CameraDriver 
{ 
public:
struct CameraDataForUser {
    int sync;
    double theta;
    double dist;
    int quality;
};

public:
    CameraDriver(const int left_ch, const int right_ch);
    ~CameraDriver();
    bool CreateCamera();
    bool StartCamera();
    bool EndCamera();
    bool GetCameraData(deepracer::service::rawdata::skeleton::events::REvent::SampleType* cameraDatas);


private:
    std::vector<u_char> ConvertFrameToVector(const Mat& frame);
    

    VideoCapture m_left_camera;
    VideoCapture m_right_camera;
    int m_left_ch;
    int m_right_ch;
};
