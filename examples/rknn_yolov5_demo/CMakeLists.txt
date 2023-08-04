cmake_minimum_required(VERSION 3.4.1)

project(rknn_yolov5_demo)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--allow-shlib-undefined")
set(CMAKE_SKIP_INSTALL_RPATH FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)

# settings
set(LIB_ARCH aarch64)
include_directories(${CMAKE_SOURCE_DIR})

set(RKNN_API_PATH ${CMAKE_SOURCE_DIR}/../../runtime/RK3588/Linux/librknn_api)
set(RKNN_RT_LIB ${RKNN_API_PATH}/${LIB_ARCH}/librknnrt.so)
include_directories(${RKNN_API_PATH}/include)
include_directories(${CMAKE_SOURCE_DIR}/../3rdparty)

# set(OpenCV_DIR ${CMAKE_SOURCE_DIR}/../3rdparty/opencv/opencv-linux-aarch64/share/OpenCV)
find_package(OpenCV REQUIRED)
message(STATUS "OpenCV library status:")
message(STATUS "    version: ${OpenCV_VERSION}")
message(STATUS "    libraries: ${OpenCV_LIBS}")
message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")

#test
#set(My_OpenCV_LIBS ${CMAKE_SOURCE_DIR}/../3rdparty/opencv/opencv-linux-aarch64/lib)

set(RGA_PATH ${CMAKE_SOURCE_DIR}/../3rdparty/rga/RK3588)
set(RGA_LIB ${RGA_PATH}/lib/Linux//${LIB_ARCH}/librga.so)
include_directories( ${RGA_PATH}/include)

set(MPP_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/mpp)
set(MPP_LIBS ${MPP_PATH}/Linux/${LIB_ARCH}/librockchip_mpp.so)
include_directories(${MPP_PATH}/include)

set(ZLMEDIAKIT_PATH ${CMAKE_SOURCE_DIR}/../3rdparty/zlmediakit)
include_directories(${ZLMEDIAKIT_PATH}/include)
set(ZLMEDIAKIT_LIBS ${ZLMEDIAKIT_PATH}/${LIB_ARCH}/libmk_api.so)

add_definitions(-DBUILD_VIDEO_RTSP)

set(CMAKE_INSTALL_RPATH "lib")
# rknn_yolov5_demo
include_directories( ${CMAKE_SOURCE_DIR}/include)
add_executable(rknn_yolov5_demo
        src/main.cc
        src/postprocess.cc
)
target_link_libraries(rknn_yolov5_demo
  ${RKNN_RT_LIB}
  ${RGA_LIB}
  ${OpenCV_LIBS}
)
#
add_executable(rknn_yolov5_camera
        src/main_camera.cc
        src/postprocess.cc
)
target_link_libraries(rknn_yolov5_camera
  ${RKNN_RT_LIB}
  ${RGA_LIB}
  ${OpenCV_LIBS}
)

add_executable(rknn_yolov5_video_demo
        src/main_video.cc
        src/postprocess.cc
        utils/mpp_decoder.cpp
        utils/mpp_encoder.cpp
        utils/drawing.cpp
)
target_link_libraries(rknn_yolov5_video_demo
  ${RKNN_RT_LIB}
  ${RGA_LIB}
  ${OpenCV_LIBS}
  ${MPP_LIBS}
  ${ZLMEDIAKIT_LIBS}
)

set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/install/)
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

install(TARGETS rknn_yolov5_demo DESTINATION ./)
install(PROGRAMS ${RKNN_RT_LIB} DESTINATION lib)
install(PROGRAMS ${RGA_LIB} DESTINATION lib)
install(DIRECTORY model DESTINATION ./)

install(TARGETS rknn_yolov5_camera DESTINATION ./)
#install(PROGRAMS ${RKNN_RT_LIB} DESTINATION lib)
#install(PROGRAMS ${RGA_LIB} DESTINATION lib)
#install(DIRECTORY model DESTINATION ./)

install(TARGETS rknn_yolov5_video_demo DESTINATION ./)
install(PROGRAMS ${MPP_LIBS} DESTINATION lib)

install(PROGRAMS ${ZLMEDIAKIT_LIBS} DESTINATION lib)
