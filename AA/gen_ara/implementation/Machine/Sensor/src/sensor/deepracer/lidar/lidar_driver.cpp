// #include "include/lidar_driver.h"
#include "lidar_driver.h"

#include <iostream>
#include <fstream>
#include <string>

using namespace sl;

static inline void delay(sl_word_size_t ms){
    while (ms>=1000){
        usleep(1000*1000);
        ms-=1000;
    };
    if (ms!=0)
        usleep(ms*1000);
}

LidarDriver::LidarDriver(const char* dev, unsigned int baudrate)
    : m_logger(ara::log::CreateLogger("LIDAR", "SWC", ara::log::LogLevel::kVerbose)) // Logger 객체 초기화
    , m_lidar(nullptr)
    , m_dev(dev), m_baudrate(baudrate)
{
}

bool LidarDriver::CreateLidar() {
    printf("[CreateLidar] Begin\n");
    m_lidar = *createLidarDriver();
    if (!m_lidar) {
        fprintf(stderr, "Fail to create Lidar instance\n");
        return false;
    }
    IChannel* channel = *createSerialPortChannel(m_dev, m_baudrate);
    if (!channel) {
        fprintf(stderr, "Fail to create Channel instance\n");
        fprintf(stderr, "m_dev=[%s]  baudrate=[%d]\n", m_dev.c_str(), m_baudrate);
        return false;
    }

    auto res = (m_lidar)->connect(channel);
    if (SL_IS_OK(res)) {
        // 장치 정보 출력
        sl_lidar_response_device_info_t deviceInfo;
        res = (m_lidar)->getDeviceInfo(deviceInfo);
        if(SL_IS_OK(res)){
            printf("Model: %d, Firmware Version: %d.%d, Hardware Version: %d\n",
            deviceInfo.model,
            deviceInfo.firmware_version >> 8, deviceInfo.firmware_version & 0xffu,
            deviceInfo.hardware_version);

            printf("SLAMTEC LIDAR S/N: ");
            for (int pos = 0; pos < 16 ;++pos) {
                printf("%02X", deviceInfo.serialnum[pos]);
            }

            printf("\n"
            "Firmware Ver: %d.%02d\n"
            "Hardware Rev: %d\n"
            , deviceInfo.firmware_version>>8
            , deviceInfo.firmware_version & 0xFF
            , (int)deviceInfo.hardware_version);
        }else{
            printf("Failed to get device information from LIDAR %08x\r\n", res);
        }
    } else {
        fprintf(stderr, "Fail to connect Lidar\n");
        return false;
    }

    printf("[CreateLidar] End\n");

    return true;
}

bool LidarDriver::StartLidar() {
    printf("[StartLidar] Begin\n");
    if (m_lidar != nullptr) {
        m_lidar->setMotorSpeed();
        m_lidar->startScan(0,1);
    } else {
        fprintf(stderr, "Fail to start Lidar. m_lidar is nullptr\n");
    }
    printf("[StartLidar] End\n");
    return true;
}

bool LidarDriver::EndLidar() {
    printf("[EndLidar] Begin\n");
    if (m_lidar != nullptr) {
        m_lidar->stop();
        delay(200);
        m_lidar->setMotorSpeed(0);
        delete m_lidar;
        m_lidar = NULL;
    }
    printf("[EndLidar] End\n");
    return true;
}

bool LidarDriver::GetLidarData(deepracer::service::rawdata::skeleton::events::REvent::SampleType* lidarDatas) {
    // printf("[GetLidarData] Begin\n");
    bool result = true;
    sl_lidar_response_measurement_node_hq_t nodes[8192];
    size_t   count = sizeof(nodes)/sizeof(sl_lidar_response_measurement_node_hq_t);
    unsigned int op_result;
    op_result = m_lidar->grabScanDataHq(nodes, count);

    if (SL_IS_OK(op_result)) {
        m_lidar->ascendScanData(nodes, count);
        for (int pos = 0; pos < (int)count ; ++pos) {
            deepracer::type::LidarDataForUser temp = {static_cast<uint32_t>(nodes[pos].flag & SL_LIDAR_RESP_HQ_FLAG_SYNCBIT),
                                    (nodes[pos].angle_z_q14 * 90.f) / 16384.f,
                                    (nodes[pos].dist_mm_q2/4.0f),
                                    static_cast<uint32_t>(nodes[pos].quality >> SL_LIDAR_RESP_MEASUREMENT_QUALITY_SHIFT) };
            lidarDatas->lidars.push_back(temp);

           //  printf("%s theta: %03.2f Dist: %08.2f Q: %d \n", 
           //      (nodes[pos].flag & SL_LIDAR_RESP_HQ_FLAG_SYNCBIT) ?"S ":"  ", 
           //      (nodes[pos].angle_z_q14 * 90.f) / 16384.f,
           //      nodes[pos].dist_mm_q2/4.0f,
           //      nodes[pos].quality >> SL_LIDAR_RESP_MEASUREMENT_QUALITY_SHIFT);
        }
    } else {
        result = false;
    }

    // printf("[GetLidarData] End\n");
    return result;
}
