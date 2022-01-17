下述<TARGET_PLATFORM>是RK356X或RK3588
# Aarch64 Linux Demo
## build

modify `GCC_COMPILER` on `build-linux_<TARGET_PLATFORM>.sh` for target platform, then execute

```
./build-linux_<TARGET_PLATFORM>.sh
```

## install

connect device and push build output into `/userdata`

```
adb push install/rknn_api_demo_Linux /userdata/
adb push ../rknn_mobilenet_demo/model/ /userdata/rknn_api_demo_Linux
```

## run

```
adb shell
cd /userdata/rknn_api_demo_Linux/
```

```
export LD_LIBRARY_PATH=./lib
./rknn_create_mem_demo model/<TARGET_PLATFORM>/mobilenet_v1.rknn model/dog_224x224.jpg
./rknn_create_mem_with_rga_demo model/<TARGET_PLATFORM>/mobilenet_v1.rknn model/dog_224x224.jpg
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
adb push install/rknn_api_demo_Android /data/
adb push ../rknn_mobilenet_demo/model/ /userdata/rknn_api_demo_Android
```

## run

```
adb shell
cd /data/rknn_api_demo_Android/
```

```
export LD_LIBRARY_PATH=./lib
./rknn_create_mem_demo model/<TARGET_PLATFORM>/mobilenet_v1.rknn model/dog_224x224.jpg
./rknn_create_mem_with_rga_demo model/<TARGET_PLATFORM>/mobilenet_v1.rknn model/dog_224x224.jpg
./rknn_with_mmz_demo model/<TARGET_PLATFORM>/mobilenet_v1.rknn model/dog_224x224.jpg
./rknn_set_internal_mem_from_fd_demo model/<TARGET_PLATFORM>/mobilenet_v1.rknn model/dog_224x224.jpg
./rknn_set_internal_mem_from_phy_demo model/<TARGET_PLATFORM>/mobilenet_v1.rknn model/dog_224x224.jpg
```

# Note
 - You may need to update libmpimmz.so and its header file of this project according to the implementation of MMZ in the system.
 - You may need to update librga.so and its header file of this project according to the implementation of RGA in the system.