# Yolo-v5 demo

# 导出rknn模型

1. 进到examples/RV1106_RV1103/rknn_yolov5_demo/convert_rknn_demo/yolov5目录下，执行如下命令，可生成rknn模型:

```sh
   cd examples/RV1106_RV1103/rknn_yolov5_demo/convert_rknn_demo/yolov5
   python onnx2rknn.py
```

## arm Linux Demo

### 编译

RV1106/RV1103编译脚本均为 `build-linux_RV1106.sh`，设置交叉编译器所在目录的路径 `RK_RV1106_TOOLCHAIN`，例如修改成

```sh
export RK_RV1106_TOOLCHAIN=~/opts/toolchain/arm-rockchip830-linux-uclibcgnueabihf/bin/arm-rockchip830-linux-uclibcgnueabihf
```

然后执行：

```sh
./build-linux_RV1106.sh
```

### 推送执行文件到板子

连接板子的usb口到PC,将整个demo目录到 `/userdata`:

```sh
adb push install/rknn_yolov5_demo_Linux /userdata/
```

### 运行

```sh
adb shell
cd /userdata/rknn_yolov5_demo_Linux/

export LD_LIBRARY_PATH=/userdata/rknn_yolov5_demo_Linux/lib
./rknn_yolov5_demo model/RV1106/yolov5s-640-640.rknn model/bus.jpg
```

Note: LD_LIBRARY_PATH 必须采用全路径
