cmake_minimum_required(VERSION 3.6)

project(rknn_dual_input_demo C)

set(CMAKE_C_STANDARD 99)

# RKNN API header path (relative to rknpu2 root)
set(RKNN_API_PATH ${CMAKE_SOURCE_DIR}/../../../runtime/RK3588/Linux/librknn_api)
# For RV1106/RV1103, use the armhf-uclibc runtime:
# set(RKNN_API_PATH ${CMAKE_SOURCE_DIR}/../../../runtime/RV1106_RV1103/Linux/librknn_api)

include_directories(${RKNN_API_PATH}/include)

set(SOURCE_FILES src/main.c)

add_executable(rknn_dual_input_demo ${SOURCE_FILES})

if (CMAKE_C_COMPILER MATCHES "aarch64")
    set(LIB_ARCH lib64)
else()
    set(LIB_ARCH lib)
endif()

# link RKNN runtime
find_library(RKNN_API_LIB
    NAMES rknnmrt
    PATHS ${RKNN_API_PATH}/${LIB_ARCH}
    NO_DEFAULT_PATH)

target_link_libraries(rknn_dual_input_demo ${RKNN_API_LIB} m pthread)

# install
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/install/rknn_dual_input_demo_Linux)
install(TARGETS rknn_dual_input_demo DESTINATION .)
install(DIRECTORY model DESTINATION .)
install(FILES ${RKNN_API_PATH}/${LIB_ARCH}/librknnmrt.so DESTINATION lib)
