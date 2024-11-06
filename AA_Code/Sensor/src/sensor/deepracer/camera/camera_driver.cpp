#include "deepracer/camera/camera_driver.h"

#include <iostream>
#include <fstream>
#include <string>

using namespace cv;

CameraDriver::CameraDriver(const int left_ch, const int right_ch)
    : m_logger(ara::log::CreateLogger("LIDAR", "SWC", ara::log::LogLevel::kVerbose)) // Logger 객체 초기화
    // , m_left_camera(nullptr), m_right_camera(nullptr)
    , m_workers(1) // m_workers.Async에 등록가능한 함수 갯수
    , m_left_ch(left_ch), m_right_ch(right_ch)
{
}

CameraDriver::~CameraDriver()
{
    EndCamera();
} // 소멸자

bool CameraDriver::CreateCamera() {
    printf("[CreateCamera] Begin\n");
    m_left_camera.open(m_left_ch);
    m_right_camera.open(m_right_ch);

    if (!m_left_camera.isOpened() || !m_right_camera.isOpened()) {
        fprintf(stderr, "Fail to create Camera instance\n");
        return false;
    }

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

bool CameraDriver::GetCameraData(deepracer::service::rawdata::skeleton::events::REvent::SampleCameraType* cameraDatas) {
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