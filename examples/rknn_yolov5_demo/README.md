# rknn yolov5 demo
Git clone
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
Get your_model.rknn 
```
convert best.pt model to onnx format
download onnx model from yolov5_bot
...
```
Run your_model.rknn 
```
add your_model.rknn to rknpu2/examples/rknn_yolov5_demo/model/RK3588/
```
change "rknpu2/examples/rknn_yolov5_demo/src/postprocess.cc":
```
gedit postprocess.cc
LABEL_NALE_TXT_PATH = 'path to your labels'
```
change "rknpu2/examples/rknn_yolov5_demo/include/postprocess.h":
```
gedit postprocess.h
OBJ_CLASS_NUM = 'class number of your model'
```
build 
```
cd ~/rknpu2/examples/rknn_yolov5_demo/
./build-linux_3588.sh
```
run
```
.build/build_linux_aarch64/rknn_yolov5_demo model/RK3588/your_model.rknn model/bus.jpg
```
