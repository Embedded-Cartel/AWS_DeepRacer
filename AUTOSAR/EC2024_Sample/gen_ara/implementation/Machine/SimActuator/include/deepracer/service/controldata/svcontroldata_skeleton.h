///////////////////////////////////////////////////////////////////////////////////////////////////////////
///                                                                                                        
/// Copyright, 2021 PopcornSAR Co., Ltd. All rights reserved.                                              
/// This software is copyright protected and proprietary to PopcornSAR Co., Ltd.                           
/// PopcornSAR Co., Ltd. grants to you only those rights as set out in the license conditions.             
///                                                                                                        
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// AUTOSAR VERSION                   : R20-11
/// GENERATED BY                      : PARA, ARA::COM Generator
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// GENERATED FILE NAME               : svcontroldata_skeleton.h
/// SERVICE INTERFACE NAME            : SvControlData
/// GENERATED DATE                    : 2024-08-14 09:44:02
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///                                                                                                        
/// CAUTION!! AUTOMATICALLY GENERATED FILE - DO NOT EDIT                                                   
///                                                                                                        
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef PARA_COM_GEN_SERVICE_INTERFACE_SVCONTROLDATA_SKELETON_H
#define PARA_COM_GEN_SERVICE_INTERFACE_SVCONTROLDATA_SKELETON_H
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// INCLUSION HEADER FILES
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @uptrace{SWS_CM_01004}
#include "svcontroldata_common.h"
#include "para/com/skeleton/skeleton_interface.h"
/// @uptrace{SWS_CM_01005}
namespace deepracer
{
namespace service
{
namespace controldata
{
/// @uptrace{SWS_CM_01006}
namespace skeleton
{
class SvControlDataSkeleton;
/// @uptrace{SWS_CM_01009}
namespace events
{
/// @uptrace{SWS_CM_00003}
class CEvent
{
public:
    /// @brief Type alias for type of event data
    /// @uptrace{SWS_CM_00162, SWS_CM_90437}
    using SampleType = std::uint32_t;
    /// @brief Constructor
    explicit CEvent(para::com::SkeletonInterface* interface) : mInterface(interface)
    {
    }
    /// @brief Destructor
    virtual ~CEvent() = default;
    /// @brief Delete copy constructor
    CEvent(const CEvent& other) = delete;
    /// @brief Delete copy assignment
    CEvent& operator=(const CEvent& other) = delete;
    /// @brief Move constructor
    CEvent(CEvent&& other) noexcept : mInterface(other.mInterface)
    {
    }
    /// @brief Move assignment
    CEvent& operator=(CEvent&& other) noexcept
    {
        mInterface = other.mInterface;
        return *this;
    }
    /// @brief Send event with data to subscribing service consumers
    /// @uptrace{SWS_CM_90437}
    ara::core::Result<void> Send(const SampleType& data)
    {
        para::serializer::Serializer serializer{};
        serializer.write(data);
        auto payload = serializer.ensure();
        return mInterface->SendEvent(kCallSign, payload);
    }
    /// @brief Returns unique pointer about SampleType
    /// @uptrace{SWS_CM_90438}
    ara::core::Result<ara::com::SampleAllocateePtr<SampleType>> Allocate()
    {
        return std::make_unique<SampleType>();
    }
    
private:
    para::com::SkeletonInterface* mInterface;
    const std::string kCallSign = {"CEvent"};
};
} /// namespace events
/// @uptrace{SWS_CM_01031}
namespace fields
{
} /// namespace fields
/// @uptrace{SWS_CM_00002}
class SvControlDataSkeleton
{
public:
    /// @uptrace{SWS_CM_00191}
    /// @brief Constructor
    /// @uptrace{SWS_CM_00002, SWS_CM_00152}
    SvControlDataSkeleton(ara::core::InstanceSpecifier instanceSpec, ara::com::MethodCallProcessingMode mode = ara::com::MethodCallProcessingMode::kEvent)
        : mInterface(std::make_unique<para::com::SkeletonInterface>(instanceSpec, mode))
        , CEvent(mInterface.get())
    {
        mInterface->SetE2EErrorHandler([this](const ara::com::e2e::E2EErrorDomain& errorCode, ara::com::e2e::DataID dataID, ara::com::e2e::MessageCounter messageCounter) {
            E2EErrorHandler(errorCode, dataID, messageCounter);
        });
    }
    /// @brief Destructor
    virtual ~SvControlDataSkeleton() = default;
    /// @brief Delete copy constructor
    /// @uptrace{SWS_CM_00134}
    SvControlDataSkeleton(const SvControlDataSkeleton& other) = delete;
    /// @brief Delete copy assignment
    /// @uptrace{SWS_CM_00134}
    SvControlDataSkeleton& operator=(const SvControlDataSkeleton& other) = delete;
    /// @brief Move constructor
    /// @uptrace{SWS_CM_00135}
    SvControlDataSkeleton(SvControlDataSkeleton&& other) noexcept
        : mInterface(std::move(other.mInterface))
        , CEvent(std::move(other.CEvent))
    {
        mInterface->SetE2EErrorHandler([this](const ara::com::e2e::E2EErrorDomain& errorCode, ara::com::e2e::DataID dataID, ara::com::e2e::MessageCounter messageCounter) {
            E2EErrorHandler(errorCode, dataID, messageCounter);
        });
        other.mInterface.reset();
    }
    /// @brief Move assignment
    /// @uptrace{SWS_CM_00135}
    SvControlDataSkeleton& operator=(SvControlDataSkeleton&& other) noexcept
    {
        mInterface = std::move(other.mInterface);
        CEvent = std::move(other.CEvent);
        mInterface->SetE2EErrorHandler([this](const ara::com::e2e::E2EErrorDomain& errorCode, ara::com::e2e::DataID dataID, ara::com::e2e::MessageCounter messageCounter) {
            E2EErrorHandler(errorCode, dataID, messageCounter);
        });
        other.mInterface.reset();
        return *this;
    }
    /// @brief Send "OfferService" message to Communication Management
    /// @uptrace{SWS_CM_00101}
    ara::core::Result<void> OfferService()
    {
        return mInterface->OfferService();
    }
    /// @brief Send "StopOfferService" message to Communication Management
    /// @uptrace{SWS_CM_00111}
    void StopOfferService()
    {
        mInterface->StopOfferService();
    }
    /// @brief Allows the implementation providing the service method to trigger the execution of the next service consumer method call at a specific point of time
    ///        if the processing mode is set to kPoll.
    /// @note This function requires that ara::com::MethodCallProcessingMode is configured as kPoll by constructor
    /// @uptrace{SWS_CM_00199}
    ara::core::Future<bool> ProcessNextMethodCall()
    {
        ara::core::Promise<bool> promise{};
        auto ret = mInterface->ProcessNextMethodCall();
        if (ret.HasValue())
        {
            promise.set_value(ret.Value());
        }
        else
        {
            promise.SetError(ret.Error());
        }
        return promise.get_future();
    }
    /// @brief In case the call to E2E_check indicated a failed E2E check of the request message, the server application can get notified via an E2E error handler.
    /// @uptrace{SWS_CM_10470}
    virtual void E2EErrorHandler(const ara::com::e2e::E2EErrorDomain& errorCode, ara::com::e2e::DataID dataID, ara::com::e2e::MessageCounter messageCounter)
    {
    }
    
private:
    std::unique_ptr<para::com::SkeletonInterface> mInterface;
    
public:
    /// @brief Event, CEvent
    events::CEvent CEvent;
    
private:
};
} /// namespace skeleton
} /// namespace controldata
} /// namespace service
} /// namespace deepracer
#endif /// PARA_COM_GEN_SERVICE_INTERFACE_SVCONTROLDATA_SKELETON_H