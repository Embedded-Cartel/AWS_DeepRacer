cmake_minimum_required(VERSION 3.10)

project(LidarLibrary VERSION 1.0)

add_library(lidar STATIC
    lidar_driver.cpp
)

#add_custom_command(TARGET include/lidar_driver.h POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy include/lidar_driver.h ${PARA_APP_GEN_DIR}/Sensor/include/deepracer/lidar)
target_include_directories(lidar
    PUBLIC
        ${PARA_APP_GEN_DIR}/Sensor/include
        ${PARA_APP_GEN_DIR}/include
        ${PARA_SDK}/include
        ${PARA_SDK}/include/lidar
        ${PARA_SDK}/include/para/swc
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)


# MESSAGE ( ${PARA_SDK} )

# for Opencv2
include_directories(
    ${PARA_SDK}/include/lidar/
)

target_link_libraries(lidar
    PUBLIC
    ${PARA_SDK}/lib/libsl_lidar_sdk.a
    ${PARA_SDK}/lib/libcamera.a
)

install(TARGETS lidar
	#LIBRARY DESTINATION ${PARA_APP_GEN_DIR}/lib
	ARCHIVE DESTINATION ${PARA_SDK}/lib
	)

install(FILES "./include/lidar_driver.h" DESTINATION ${PARA_APP_GEN_DIR}/Sensor/include/deepracer/lidar)
