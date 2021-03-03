#!/bin/bash

set -e

ANDROID_NDK_PATH=~/opts/ndk/android-ndk-r16b

BUILD_TYPE=Debug
#BUILD_TYPE=Release

ROOT_PWD=$( cd "$( dirname $0 )" && cd -P "$( dirname "$SOURCE" )" && pwd )

BUILD_DIR=${ROOT_PWD}/build/build_android_v8a

if [[ ! -d "${BUILD_DIR}" ]]; then
  mkdir -p ${BUILD_DIR}
fi

cd ${BUILD_DIR}
cmake ../.. \
       	-DCMAKE_SYSTEM_NAME=Android \
        -DCMAKE_SYSTEM_VERSION=24 \
        -DCMAKE_ANDROID_ARCH_ABI=arm64-v8a \
        -DCMAKE_ANDROID_STL_TYPE=gnustl_static \
        -DCMAKE_ANDROID_NDK=$ANDROID_NDK_PATH \
        -DCMAKE_BUILD_TYPE=${BUILD_TYPE}
make -j4
make install
cd ..

