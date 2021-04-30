# Aarch64 Linux Demo
## build

modify `GCC_COMPILER` on `build-linux.sh` for target platform, then execute

```
./build-linux.sh
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

- rk3566/rk3568
```
export LD_LIBRARY_PATH=./lib
./rknn_create_mem_demo model/mobilenet_v1.rknn model/dog_224x224.jpg
./rknn_create_mem_with_rga_demo model/mobilenet_v1.rknn model/dog_224x224.jpg
```

# Android Demo
## build

modify `ANDROID_NDK_PATH` on `build-android.sh` for target platform, then execute

```
./build-android.sh
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

- rk3566/rk3568
```
export LD_LIBRARY_PATH=./lib
./rknn_create_mem_demo model/mobilenet_v1.rknn model/dog_224x224.jpg
./rknn_create_mem_with_rga_demo model/mobilenet_v1.rknn model/dog_224x224.jpg
./rknn_with_mmz_demo model/mobilenet_v1.rknn model/dog_224x224.jpg
./rknn_set_internal_mem_from_fd_demo model/mobilenet_v1.rknn model/dog_224x224.jpg
./rknn_set_internal_mem_from_phy_demo model/mobilenet_v1.rknn model/dog_224x224.jpg
```

# Note
 - You may need to update libmpimmz.so and its header file of this project according to the implementation of MMZ in the system.
 - You may need to update librga.so and its header file of this project according to the implementation of RGA in the system.