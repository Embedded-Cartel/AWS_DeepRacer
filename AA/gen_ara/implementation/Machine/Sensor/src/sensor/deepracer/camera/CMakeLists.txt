cmake_minimum_required(VERSION 3.10)
#project(camera_test)

find_package(OpenCV REQUIRED)

MESSAGE(${OpenCV_LIBS})
add_library(camera STATIC
    camera_driver.cpp
)

#add_custom_command(TARGET include/camera_driver.h POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy include/camera_driver.h ${PARA_APP_GEN_DIR}/Sensor/include/deepracer/camera)
target_include_directories(camera
    PUBLIC
        ${PARA_APP_GEN_DIR}/Sensor/include
        ${PARA_APP_GEN_DIR}/include
        ${PARA_SDK}/include
        ${PARA_SDK}/include/para/swc
	/usr/include/opencv4
        ${CMAKE_CURRENT_SOURCE_DIR}
)


# for Opencv2
include_directories(
    ${OpenCV_INCLUDE_DIRS}
)

target_link_libraries(camera
    PUBLIC
        ${OpenCV_LIBS}
)

install(TARGETS camera
	#LIBRARY DESTINATION ${PARA_APP_GEN_DIR}/lib
	ARCHIVE DESTINATION ${PARA_SDK}/lib
	)

install(FILES "./include/camera_driver.h" DESTINATION ${PARA_APP_GEN_DIR}/Sensor/include/deepracer/camera)
