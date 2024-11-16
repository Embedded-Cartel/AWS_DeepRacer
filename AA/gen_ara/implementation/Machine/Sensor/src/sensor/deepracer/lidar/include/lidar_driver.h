#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>

#include "sl_lidar.h"

#include "sl_lidar_driver.h"
#include "para/swc/port_pool.h"
#include "sensor/aa/port/rawdata.h"

using namespace sl;

#define LIDAR_DATA_PATH "/home/ksh/Dev/autosar/real/AWS_DeepRacer/sdk/bin/lidar_data_one_cycle.txt"

class LidarDriver 
{ 
public:
struct LidarDataForUser {
    int sync;
    double theta;
    double dist;
    int quality;
};

public:
    LidarDriver(const char* dev, unsigned int baudrate);
    bool CreateLidar();
    bool StartLidar();
    bool EndLidar();
    bool GetLidarData(deepracer::service::rawdata::skeleton::events::REvent::SampleType* lidarDatas);

private:
    /// @brief Logger for software component
    ara::log::Logger& m_logger;
    
    ILidarDriver * m_lidar;
    std::string m_dev;
    unsigned int m_baudrate;
};
