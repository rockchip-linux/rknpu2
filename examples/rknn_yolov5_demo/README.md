# Yolo-v5 demo

## 导出rknn模型

请参考 https://github.com/airockchip/rknn_model_zoo/tree/main/models/vision/object_detection/yolov5-pytorch



## 注意事项

1. 使用rknn-toolkit2版本大于等于1.1.2。
2. 切换成自己训练的模型时，请注意对齐anchor等后处理参数，否则会导致后处理解析出错。
3. 官网和rk预训练模型都是检测80类的目标，如果自己训练的模型,需要更改include/postprocess.h中的OBJ_CLASS_NUM以及NMS_THRESH,BOX_THRESH后处理参数。
5. demo需要librga.so的支持,编译使用请参考https://github.com/rockchip-linux/linux-rga
5. 由于硬件限制，该demo的模型默认把 yolov5 模型的后处理部分，移至cpu实现。本demo附带的模型均使用relu为激活函数，相比silu激活函数精度略微下降，性能大幅上升。



## Android Demo

### 编译

根据指定平台修改 `build-android_<TARGET_PLATFORM>.sh`中的Android NDK的路径 `ANDROID_NDK_PATH`，<TARGET_PLATFORM>可以是RK356X或RK3588 例如修改成：

```sh
ANDROID_NDK_PATH=~/opt/tool_chain/android-ndk-r17
```

然后执行：

```sh
./build-android_<TARGET_PLATFORM>.sh
```

### 推送执行文件到板子

连接板子的usb口到PC,将整个demo目录到 `/data`:

```sh
adb root
adb remount
adb push install/rknn_yolov5_demo /data/
```

### 运行

```sh
adb shell
cd /data/rknn_yolov5_demo/

export LD_LIBRARY_PATH=./lib
./rknn_yolov5_demo model/<TARGET_PLATFORM>/yolov5s-640-640.rknn model/bus.jpg
```

## Aarch64 Linux Demo

### 编译

根据指定平台修改 `build-linux_<TARGET_PLATFORM>.sh`中的交叉编译器所在目录的路径 `TOOL_CHAIN`，例如修改成

```sh
export TOOL_CHAIN=~/opt/tool_chain/gcc-9.3.0-x86_64_aarch64-linux-gnu/host
```

然后执行：

```sh
./build-linux_<TARGET_PLATFORM>.sh
```

### 推送执行文件到板子


将 install/rknn_yolov5_demo_Linux 拷贝到板子的/userdata/目录.

- 如果使用rockchip的EVB板子，可以使用adb将文件推到板子上：

```
adb push install/rknn_yolov5_demo_Linux /userdata/
```

- 如果使用其他板子，可以使用scp等方式将install/rknn_yolov5_demo_Linux拷贝到板子的/userdata/目录

### 运行

```sh
adb shell
cd /userdata/rknn_yolov5_demo_Linux/

export LD_LIBRARY_PATH=./lib
./rknn_yolov5_demo model/<TARGET_PLATFORM>/yolov5s-640-640.rknn model/bus.jpg
```

Note: Try searching the location of librga.so and add it to LD_LIBRARY_PATH if the librga.so is not found on the lib folder.
Using the following commands to add to LD_LIBRARY_PATH.

```sh
export LD_LIBRARY_PATH=./lib:<LOCATION_LIBRGA.SO>
```



### 注意

- 需要根据系统的rga驱动选择正确的librga库，具体依赖请参考： https://github.com/airockchip/librga
