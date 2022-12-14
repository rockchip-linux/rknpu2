# rknn yolov5 demo

Build library
```
cd rknpu2/examples/rknn_yolov5_demo/
./build-linux_3588.sh
chmod 777 build/build_linux_aarch64/rknn_yolov5_demo
export LD_LIBRARY_PATH=./lib
```

```
change "rknpu2/examples/rknn_yolov5_demo/include/postprocess.h":
```
gedit postprocess.h

```
.build/build_linux_aarch64/rknn_yolov5_demo model/RK3588/your_model.rknn model/bus.jpg
```
