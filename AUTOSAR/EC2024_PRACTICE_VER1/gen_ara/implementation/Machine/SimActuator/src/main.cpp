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
/// GENERATED FILE NAME               : main.cpp
/// EXECUTABLE NAME                   : SimActuator
/// GENERATED DATE                    : 2024-10-12 01:33:36
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// INCLUSION HEADER FILES
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "ara/core/initialization.h"
#include "ara/exec/execution_client.h"
#include "ara/log/logger.h"
 
#include "simactuator/aa/simactuator.h"
 
#include <csignal>
 
simactuator::aa::SimActuator* g_swcSimActuator{nullptr};
 
static void SignalHandler(std::int32_t signal)
{
    if (signal == SIGTERM || signal == SIGINT)
    {
        g_swcSimActuator->Terminate();
    }
}
 
int main(int argc, char *argv[], char* envp[])
{
    bool proceed{true};
    bool araInitialized{true};
    
    // initialize AUTOSAR adaptive application
    auto appInit = ara::core::Initialize();
    if (!appInit.HasValue())
    {
        proceed = false;
        araInitialized = false;
    }
    
    if (araInitialized)
    {
        ara::log::Logger& appLogger{ara::log::CreateLogger("DFLT", "SimActuator's main function")};
        
        // regist signals
        std::signal(SIGTERM, SignalHandler);
        std::signal(SIGINT, SignalHandler);
        
        // declaration of software components
        simactuator::aa::SimActuator swcSimActuator;
        g_swcSimActuator = &swcSimActuator;
        
        // initialize software component
        proceed = swcSimActuator.Initialize();
        
        if (proceed)
        {
            // report execution state
            ara::exec::ExecutionClient executionClient;
            auto exec = executionClient.ReportExecutionState(ara::exec::ExecutionState::kRunning);
            if (exec.HasValue())
            {
                appLogger.LogVerbose() << "Running adaptive application";
            }
            else
            {
                appLogger.LogError() << "Unable to report execution state";
                araInitialized = false;
            }
            // start software component
            swcSimActuator.Start();
        }
        else
        {
            appLogger.LogError() << "Unable to start application";
        }
        
        // de-initialize AUTOSAR adaptive application
        auto appDeinit = ara::core::Deinitialize();
        if (!appDeinit.HasValue())
        {
            araInitialized = false;
        }
    }
    
    return (araInitialized && proceed) ? EXIT_SUCCESS : EXIT_FAILURE;
}
 