下述<TARGET_PLATFORM>是RK356X或RK3588
# Aarch64 Linux Demo
## build

modify `GCC_COMPILER` on `build-linux_<TARGET_PLATFORM>.sh` for target platform

 then execute

```
./build-linux_<TARGET_PLATFORM>.sh
```

## install

connect device and push build output into `/userdata`

```
adb push install/rknn_ssd_demo_Linux /userdata/
```

## run

```
adb shell
cd /userdata/rknn_ssd_demo_Linux/
```

- rk3566/rk3568
```
export LD_LIBRARY_PATH=./lib
./rknn_ssd_demo model/<TARGET_PLATFORM>/ssd_inception_v2.rknn model/road.bmp
```


# Android Demo
## build

modify `ANDROID_NDK_PATH` on `build-android_<TARGET_PLATFORM>.sh` for target platform, then execute

```
./build-android_<TARGET_PLATFORM>.sh
```

## install

connect device and push build output into `/data`

```
adb push install/rknn_ssd_demo_Android /data/
```

## run

```
adb shell
cd /data/rknn_ssd_demo_Android/
```

```
export LD_LIBRARY_PATH=./lib
./rknn_ssd_demo model/_<TARGET_PLATFORM>/ssd_inception_v2.rknn model/road.bmp
```