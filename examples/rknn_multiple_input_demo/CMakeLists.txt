cmake_minimum_required(VERSION 3.4.1)

project(rknn_multiple_input_demo)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# rknn api
if(TARGET_SOC STREQUAL "rk356x")
  set(RKNN_API_PATH ${CMAKE_SOURCE_DIR}/../../runtime/RK356X/${CMAKE_SYSTEM_NAME}/librknn_api)
elseif(TARGET_SOC STREQUAL "rk3588")
  set(RKNN_API_PATH ${CMAKE_SOURCE_DIR}/../../runtime/RK3588/${CMAKE_SYSTEM_NAME}/librknn_api)
else()
  message(FATAL_ERROR "TARGET_SOC is not set, ref value: rk356x or rk3588")
endif()

if (CMAKE_SYSTEM_NAME STREQUAL "Android")
  set(RKNN_RT_LIB ${RKNN_API_PATH}/${CMAKE_ANDROID_ARCH_ABI}/librknnrt.so)
else()
  if (CMAKE_C_COMPILER MATCHES "aarch64")
    set(LIB_ARCH aarch64)
  else()
    set(LIB_ARCH armhf)
  endif()
  set(RKNN_RT_LIB ${RKNN_API_PATH}/${LIB_ARCH}/librknnrt.so)
endif()
include_directories(${RKNN_API_PATH}/include)

set(CMAKE_INSTALL_RPATH "lib")

add_executable(rknn_multiple_input_demo
        src/main.cc
)

target_link_libraries(rknn_multiple_input_demo
	${RKNN_RT_LIB}
)

# install target and libraries
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/install/rknn_multiple_input_demo_${CMAKE_SYSTEM_NAME})
install(TARGETS rknn_multiple_input_demo DESTINATION ./)
install(DIRECTORY model DESTINATION ./)
install(PROGRAMS ${RKNN_RT_LIB} DESTINATION lib)
