#include "include/camera_driver.h"

#include <iostream>
#include <fstream>
#include <string>

using namespace cv;

CameraDriver::CameraDriver(const int left_ch, const int right_ch)
    : m_left_ch(left_ch), m_right_ch(right_ch)
{
	printf("ksh_@@@ zzzzz\n");
}

CameraDriver::~CameraDriver()
{
    EndCamera();
} // 소멸자

// bool CameraDriver::CreateCamera(const int left_ch, const int right_ch) {
bool CameraDriver::CreateCamera() {
    printf("[CreateCamera] Begin l_ch=[%d] r_ch=[%d]\n", m_left_ch, m_right_ch);
//     printf("OpenCV: %s", cv::getBuildInformation().c_str());

    //VideoCapture cap1(CAP_DSHOW);
   // VideoCapture cap1(CAP_V4L2);
//    VideoCapture cap1(CAP_ANY);
    VideoCapture cap1;
    printf("ksh_@@@ 1\n");
//    cap1.open(0,CAP_ANY);
//    cap1.open(0,CAP_DSHOW);
    cap1.open(0);
    printf("ksh-@@@@asdadasd\n");

    if (!cap1.isOpened()) {
        std::cerr << "카메라1을 열 수 없습니다." << std::endl;
        return -1;
    }

    printf("ksh_@@@ cam 0\n");
    m_left_camera.open(m_left_ch);
    printf("ksh_@@@ cam 1\n");
    m_right_camera.open(m_right_ch);
    printf("ksh_@@@ cam 2\n");

    if (!m_left_camera.isOpened() || !m_right_camera.isOpened()) {
        fprintf(stderr, "Fail to create Camera instance\n");
	printf("ksh_@@@ faillllllllllllllll\n");
        return false;
    }

    printf("ksh_@@@ cam 3\n");
    // Set Camera Settings
    m_left_camera.set(CAP_PROP_FOURCC, VideoWriter::fourcc('M', 'J', 'P', 'G')); // MJPEG
    m_left_camera.set(CAP_PROP_FRAME_WIDTH, FRAME_WIDTH); // width
    m_left_camera.set(CAP_PROP_FRAME_HEIGHT, FRAME_HEIGHT); // height

    m_right_camera.set(CAP_PROP_FOURCC, VideoWriter::fourcc('M', 'J', 'P', 'G')); // MJPEG
    m_right_camera.set(CAP_PROP_FRAME_WIDTH, FRAME_WIDTH); // width
    m_right_camera.set(CAP_PROP_FRAME_HEIGHT, FRAME_HEIGHT); // height
    printf("[CreateCamera] End\n");
    return true;
}

bool CameraDriver::StartCamera() {
    printf("[StartCamera] Begin\n");
    printf("[StartCamera] End\n");
    return true;
}

bool CameraDriver::EndCamera() {
    printf("[EndCamera] Begin\n");
    if (m_left_camera.isOpened()) {
        m_left_camera.release();
    }
    if (m_right_camera.isOpened()) {
        m_right_camera.release();
    }
    printf("[EndCamera] End\n");
    return true;
}

bool CameraDriver::GetCameraData(deepracer::service::rawdata::skeleton::events::REvent::SampleType* cameraDatas) {
    printf("[GetCameraData] Begin\n");
    Mat leftFrame, rightFrame;

    if (!m_left_camera.read(leftFrame)) {
        printf("Fail to read left camera data\n");
        return false;
    }

    if (!m_right_camera.read(rightFrame)) {
        printf("Fail to read right camera data\n");
        return false;
    }

    cvtColor(leftFrame, leftFrame, COLOR_BGR2GRAY);
    cvtColor(rightFrame, rightFrame, COLOR_BGR2GRAY);

    std::vector<u_char> left_datas = ConvertFrameToVector(leftFrame);
    std::vector<u_char> right_datas = ConvertFrameToVector(rightFrame);

    if (left_datas.size() != TOTOAL_FIXEL || right_datas.size() != TOTOAL_FIXEL) {
        printf("Fail to Convert Camera data to Vector\n");
        return false;
    }
    printf("[GetCameraData] End\n");
    return true;
}

std::vector<u_char> CameraDriver::ConvertFrameToVector(const Mat& frame) {
    // printf("[ConvertFrameToVector] Start\n");
    std::vector<u_char> result;
    Mat image = frame.clone();

    for (int y = 0; y < image.rows; ++y) {
        u_char* ptr = image.ptr<u_char>(y);
        for (int x = 0; x < image.cols; ++x) {
            result.push_back(ptr[x]);
        }
    }
    // printf("[ConvertFrameToVector] End\n");
    return result;
}
