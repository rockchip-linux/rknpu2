# rknn yolov5 demo
Install requirenments
```
sudo apt install gcc cmake git build-essential python3-tk python3.8-dev virtualenv

```
clone rep
```
git clone https://github.com/BelotserkovskiyVA/rknpu2.git
```

Build library
```
cd rknpu2/examples/rknn_yolov5_demo/
./build-linux_3588.sh
chmod 777 build/build_linux_aarch64/rknn_yolov5_demo
export LD_LIBRARY_PATH=./lib
```
Convert your_model.rknn
```
train_model
convert to onnx
best.pt -> best.onnx
convert to rknn (rknn_toolkit2)
copy your_model.rknn to rknpu2/examples/rknn_yolov5_demo/model/RK3588/
```

change "rknpu2/examples/rknn_yolov5_demo/src/postprocess.cc":
```
gedit postprocess.cc
LABEL_NALE_TXT_PATH
```
change "rknpu2/examples/rknn_yolov5_demo/include/postprocess.h":
```
gedit postprocess.h
OBJ_CLASS_NUM
```
Build library
```
cd rknpu2/examples/rknn_yolov5_demo/
./build-linux_3588.sh
```
Run model
```
.build/build_linux_aarch64/rknn_yolov5_demo model/RK3588/your_model.rknn model/bus.jpg
```
