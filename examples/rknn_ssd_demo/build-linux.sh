#!/bin/bash
set -e

# export RK356X_TOOLCHAIN={RK3566_SDK_ROOT}/prebuilts/gcc/linux-x86/aarch64/gcc-buildroot-9.3.0-2020.03-x86_64_aarch64-rockchip-linux-gnu/

if [ x"${RK356X_TOOLCHAIN}" == x"" ]
then
  echo "Please set RK356X_TOOLCHAIN!"
  exit 1
fi

# for aarch64
if [ -f "${RK356X_TOOLCHAIN}/bin/aarch64-linux-gcc" ]
then
  GCC_COMPILER=${RK356X_TOOLCHAIN}/bin/aarch64-linux
else
  GCC_COMPILER=${RK356X_TOOLCHAIN}/bin/aarch64-linux-gnu
fi

ROOT_PWD=$( cd "$( dirname $0 )" && cd -P "$( dirname "$SOURCE" )" && pwd )

# build
BUILD_DIR=${ROOT_PWD}/build/build_linux_aarch64

if [[ ! -d "${BUILD_DIR}" ]]; then
  mkdir -p ${BUILD_DIR}
fi

cd ${BUILD_DIR}
cmake ../.. \
    -DCMAKE_SYSTEM_NAME=Linux \
    -DCMAKE_C_COMPILER=${GCC_COMPILER}-gcc \
    -DCMAKE_CXX_COMPILER=${GCC_COMPILER}-g++
make -j4
make install
cd -
