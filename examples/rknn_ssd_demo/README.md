The following <TARGET_PLATFORM> represents RK356X or RK3588.
# Aarch64 Linux Demo
## Build

modify `GCC_COMPILER` on `build-linux_<TARGET_PLATFORM>.sh` for target platform

 then execute

```
./build-linux_<TARGET_PLATFORM>.sh
```

## Install

Copy install/rknn_mobilenet_demo_Linux  to the devices under /userdata/.

- If you use rockchip's evb board, you can use the following way:

Connect device and push the program and rknn model to `/userdata`

```
adb push install/rknn_ssd_demo_Linux /userdata/
```

- If your board has sshd service, you can use scp or other methods to copy the program and rknn model to the board.

## Run

```
adb shell
cd /userdata/rknn_ssd_demo_Linux/
```



```
export LD_LIBRARY_PATH=./lib
./rknn_ssd_demo model/<TARGET_PLATFORM>/ssd_inception_v2.rknn model/bus.jpg
```


# Android Demo
## Build

modify `ANDROID_NDK_PATH` on `build-android_<TARGET_PLATFORM>.sh` for target platform, then execute

```
./build-android_<TARGET_PLATFORM>.sh
```

## Install

connect device and push build output into `/data`

```
adb push install/rknn_ssd_demo_Android /data/
```

## Run

```
adb shell
cd /data/rknn_ssd_demo_Android/
```

```
export LD_LIBRARY_PATH=./lib
./rknn_ssd_demo model/_<TARGET_PLATFORM>/ssd_inception_v2.rknn model/bus.jpg
```