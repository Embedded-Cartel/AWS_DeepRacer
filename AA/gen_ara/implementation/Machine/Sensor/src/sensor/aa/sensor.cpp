///////////////////////////////////////////////////////////////////////////////////////////////////////////
///                                                                                                        
/// Copyright, 2021 PopcornSAR Co., Ltd. All rights reserved.                                              
/// This software is copyright protected and proprietary to PopcornSAR Co., Ltd.                           
/// PopcornSAR Co., Ltd. grants to you only those rights as set out in the license conditions.             
///                                                                                                        
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// AUTOSAR VERSION                   : R20-11
/// GENERATED BY                      : PARA, Adaptive Application Generator
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// GENERATED FILE NAME               : sensor.cpp
/// SOFTWARE COMPONENT NAME           : Sensor
/// GENERATED DATE                    : 2024-10-31 15:08:42
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// INCLUSION HEADER FILES
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "sensor/aa/sensor.h"
 
namespace sensor
{
namespace aa
{
 
Sensor::Sensor()
    : m_logger(ara::log::CreateLogger("SENS", "SWC", ara::log::LogLevel::kVerbose))
    , m_workers(2)
{
}
 
Sensor::~Sensor()
{
}
 
bool Sensor::Initialize()
{
    m_logger.LogVerbose() << "Sensor::Initialize";
    
    bool init{true};
    
    m_RawData = std::make_shared<sensor::aa::port::RawData>();
    
    return init;
}
 
void Sensor::Start()
{
    m_logger.LogVerbose() << "Sensor::Start";
    
    m_RawData->Start();
    
    // run software component
    Run();
}
 
void Sensor::Terminate()
{
    m_logger.LogVerbose() << "Sensor::Terminate";
    
    m_RawData->Terminate();
}
 
void Sensor::Run()
{
    m_logger.LogVerbose() << "Sensor::Run";
    
    m_workers.Async([this] { m_RawData->SendEventREventCyclic(); });
    m_workers.Async([this] { m_RawData->NotifyFieldRFieldCyclic(); });
    
    m_workers.Wait();
}
 
} /// namespace aa
} /// namespace sensor