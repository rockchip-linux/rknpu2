# onnx模型

* onnx_models/yolov5s_rm_transpose.onnx
模型基于官方yolov5s基础魔改而来，与rknn-toolkit2工具SDK中examples/onnx/yolov5/yolov5s.onnx相同，用于rknpu2的examples/rknn_yolov5_demo

# 转换rknn模型

1. 将onnx2rknn.py以下的参数修改成对应的平台，例如RK3566_RK3568模型，修改为：

```C++
platform="rk3566"
```

2. 执行python onnx2rknn.py

3. rknn模型生成在rknn_models目录
