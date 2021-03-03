# Aarch64 Linux Demo
## build

modify `GCC_COMPILER` on `build-linux.sh` for target platform, then execute

```
./build-linux.sh
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
./rknn_ssd_demo model/ssd_inception_v2.rknn model/road.bmp
```
