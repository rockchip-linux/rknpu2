set -e

TARGET_SOC="rk356x"
export TOOL_CHAIN=~/opt/tool_chain/gcc-9.3.0-x86_64_aarch64-linux-gnu/host

if [ x"${TOOL_CHAIN}" == x"" ]
then
  echo "Please set TOOL_CHAIN!"
  exit 1
fi

# for aarch64
if [ -f "${TOOL_CHAIN}/bin/aarch64-linux-gcc" ]
then
  GCC_COMPILER=${TOOL_CHAIN}/bin/aarch64-linux
else
  GCC_COMPILER=${TOOL_CHAIN}/bin/aarch64-linux-gnu
fi

export LD_LIBRARY_PATH=${TOOL_CHAIN}/lib64:$LD_LIBRARY_PATH
export CC=${GCC_COMPILER}-gcc
export CXX=${GCC_COMPILER}-g++

ROOT_PWD=$( cd "$( dirname $0 )" && cd -P "$( dirname "$SOURCE" )" && pwd )

# build
BUILD_DIR=${ROOT_PWD}/build/build_linux_aarch64

if [[ ! -d "${BUILD_DIR}" ]]; then
  mkdir -p ${BUILD_DIR}
fi

cd ${BUILD_DIR}
cmake ../.. -DCMAKE_SYSTEM_NAME=Linux -DTARGET_SOC=${TARGET_SOC}
make -j4
make install
cd -
