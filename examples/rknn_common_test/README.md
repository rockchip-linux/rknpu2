下述<TARGET_PLATFORM>是RK356X或RK3588
# Aarch64 Linux Demo
## build

modify `GCC_COMPILER` on `build-linux_<TARGET_PLATFORM>.sh` for target platform,

then execute

```
./build-linux_<TARGET_PLATFORM>.sh
```

## install

connect device and push build output into `/userdata`

```
adb push install/rknn_common_test_Linux /userdata/
```

## run

```
adb shell
cd /userdata/rknn_common_test_Linux/
```

```
export LD_LIBRARY_PATH=./lib
./rknn_common_test model/<TARGET_PLATFORM>/mobilenet_v1.rknn model/dog_224x224.jpg
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
adb push install/rknn_common_test_Android /data/
```

## run

```
adb shell
cd /data/rknn_common_test_Android/
```

```
export LD_LIBRARY_PATH=./lib
./rknn_common_test model/<TARGET_PLATFORM>/mobilenet_v1.rknn model/dog_224x224.jpg
```
