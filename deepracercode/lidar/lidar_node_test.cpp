#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>

#include "sl_lidar.h" 
#include "sl_lidar_driver.h"

using namespace sl;

static inline void delay(sl_word_size_t ms){
    while (ms>=1000){
        usleep(1000*1000);
        ms-=1000;
    };
    if (ms!=0)
        usleep(ms*1000);
}

int main(int argc, const char * argv[]) {
    const char * dev = "/dev/ttyUSB0";
    unsigned int baudrate = 115200;
    unsigned int op_result;
    IChannel* channel;
    
    // 드라이버 인스턴스 생성
	ILidarDriver * drv = *createLidarDriver();
    if (!drv) {
        fprintf(stderr, "insufficent memory, exit\n");
        exit(-2);
    }

    // 장치 연결(/dev/ttyUSB0, 115200)
    channel = *createSerialPortChannel(dev, baudrate);
    auto res = (drv)->connect(channel);
    if (SL_IS_OK(res)) {
        // 장치 정보 출력
        sl_lidar_response_device_info_t deviceInfo;
        res = (drv)->getDeviceInfo(deviceInfo);
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
    }
    
    // 스캔 시작
    drv->setMotorSpeed();
    drv->startScan(0,1);
    
    while (1) {
        // 스캔 정보 출력
        sl_lidar_response_measurement_node_hq_t nodes[8192];
        size_t   count = sizeof(nodes)/sizeof(sl_lidar_response_measurement_node_hq_t);

        op_result = drv->grabScanDataHq(nodes, count);

        if (SL_IS_OK(op_result)) {
            drv->ascendScanData(nodes, count);
            for (int pos = 0; pos < (int)count ; ++pos) {
                printf("%s theta: %03.2f Dist: %08.2f Q: %d \n", 
                    (nodes[pos].flag & SL_LIDAR_RESP_HQ_FLAG_SYNCBIT) ?"S ":"  ", 
                    (nodes[pos].angle_z_q14 * 90.f) / 16384.f,
                    nodes[pos].dist_mm_q2/4.0f,
                    nodes[pos].quality >> SL_LIDAR_RESP_MEASUREMENT_QUALITY_SHIFT);
            }
        }
    }
    // 스캔 종료
    drv->stop();
	delay(200);
    drv->setMotorSpeed(0);
    delete drv;
    drv = NULL;

    return 0;
}
