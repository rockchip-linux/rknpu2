# Aarch64 Linux Demo
## build

modify `GCC_COMPILER` on `build-linux.sh` for target platform, then execute

```
./build-linux.sh
```

## install

connect device and push build output into `/userdata`

```
adb push install/rknn_mobilenet_demo_Linux /userdata/
```

## run

```
adb shell
cd /userdata/rknn_mobilenet_demo_Linux/
```

- rk3566/rk3568
```
./rknn_mobilenet_demo model/mobilenet_v1.rknn model/dog_224x224.jpg
```