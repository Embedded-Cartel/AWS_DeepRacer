#include "include/camera_driver.h"

#include <iostream>
#include <fstream>
#include <string>

using namespace cv;

CameraDriver::CameraDriver(const int left_ch, const int right_ch)
    : m_left_camera(), m_right_camera()
    , m_left_ch(left_ch), m_right_ch(right_ch)
{
}

CameraDriver::~CameraDriver()
{
    EndCamera();
} // 소멸자

bool CameraDriver::CreateCamera() {
    printf("[CreateCamera] Begin l_ch=[%d] r_ch=[%d]\n", m_left_ch, m_right_ch);
//     printf("OpenCV: %s", cv::getBuildInformation().c_str());

    m_left_camera = new VideoCapture(2);
    m_right_camera = new VideoCapture(0);

    if (!m_left_camera->isOpened()) {
        std::cerr << "카메라1을 열 수 없습니다." << std::endl;
        return -1;
    }

    if (!m_right_camera->isOpened()) {
        std::cerr << "카메라2를 열 수 없습니다." << std::endl;
        return -1;
    }

    // Set Camera Settings
    m_left_camera->set(CAP_PROP_FOURCC, VideoWriter::fourcc('M', 'J', 'P', 'G')); // MJPEG
    m_left_camera->set(CAP_PROP_FRAME_WIDTH, FRAME_WIDTH); // width
    m_left_camera->set(CAP_PROP_FRAME_HEIGHT, FRAME_HEIGHT); // height

    m_right_camera->set(CAP_PROP_FOURCC, VideoWriter::fourcc('M', 'J', 'P', 'G')); // MJPEG
    m_right_camera->set(CAP_PROP_FRAME_WIDTH, FRAME_WIDTH); // width
    m_right_camera->set(CAP_PROP_FRAME_HEIGHT, FRAME_HEIGHT); // height
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
    if (m_left_camera->isOpened()) {
        m_left_camera->release();
    }
    if (m_right_camera->isOpened()) {
        m_right_camera->release();
    }
    printf("[EndCamera] End\n");
    return true;
}

bool CameraDriver::GetCameraData(deepracer::service::rawdata::skeleton::events::REvent::SampleType* cameraDatas) {
    printf("[GetCameraData] Begin\n");

    Mat leftFrame, rightFrame;

    std::vector<uchar> buffer3;
    std::vector<uchar> buffer4;

    *m_left_camera >> leftFrame;
    *m_right_camera >> rightFrame;

    cvtColor(leftFrame, leftFrame, COLOR_BGR2GRAY);
    cvtColor(rightFrame, rightFrame, COLOR_BGR2GRAY);

    // imencode(".jpeg", leftFrame, buffer3);
    // imencode(".jpeg", rightFrame, buffer4);
    // Mat decoded_leftFrame = imdecode(buffer3, IMREAD_UNCHANGED);
    // Mat decoded_rightFrame = imdecode(buffer4, IMREAD_UNCHANGED);
    // imwrite("leftFrame.jpg", leftFrame);
    // imwrite("rightFrame.jpg", rightFrame);

    
    std::vector<u_char> left_datas = ConvertFrameToVector(leftFrame);
    std::vector<u_char> right_datas = ConvertFrameToVector(rightFrame);
    cameraDatas->left_camera = left_datas;
    cameraDatas->right_camera = right_datas;

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
