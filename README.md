## RKNPU2
  RKNPU2 provides an advanced interface to access Rockchip NPU.

## Support Platform
  - RK3566/RK3568
  - RK3588/RK3588S
  - RV1103/RV1106
  - RK3562

Note:
      The rknn model must be generated using RKNN Toolkit 2:  https://github.com/rockchip-linux/rknn-toolkit2

​      **For RK1808/RV1109/RV1126/RK3399Pro, please use:**

​          https://github.com/rockchip-linux/rknn-toolkit

​          https://github.com/rockchip-linux/rknpu

​          https://github.com/airockchip/RK3399Pro_npu

## ReleaseLog

### 1.5.2
- Improved dynamic shape support
- Improved matmul api support
- Add GPU back-end implementations for some operators such as matmul
- Improve transformer support
- Reduce rknn_init memory usage
- Optimize rknn_init time-consuming

### 1.5.0

- Support RK3562
- Support more NPU operator fuse, such as Conv-Silu/Conv-Swish/Conv-Hardswish/Conv-sigmoid/Conv-HardSwish/Conv-Gelu ..
- Improve support for  NHWC output layout
- RK3568/RK3588：The maximum input resolution up to 8192
- Improve support for Swish/DataConvert/Softmax/Lstm/LayerNorm/Gather/Transpose/Mul/Maxpool/Sigmoid/Pad
- Improve support for CPU operators (Cast, Sin, Cos, RMSNorm, ScalerND, GRU)
- Limited support for dynamic resolution
- Provide MATMUL API
- Add RV1103/RV1106 rknn_server application as proxy between PC and board
- Add more examples such as rknn_dynamic_shape_input_demo and video demo for yolov5
- Bug fix



### 1.4.0

- Support more NPU operators, such as Reshape、Transpose、MatMul、 Max、Min、exGelu、exSoftmax13、Resize etc.
- Add **Weight Share**  function, reduce memory usage.
- Add **Weight Compression** function, reduce memory and bandwidth usage.(RK3588/RV1103/RV1106)
- RK3588 supports storing weights or feature maps on SRAM, reducing system bandwidth consumption.
- RK3588 adds the function of running a single model on multiple cores at the same time.
- Add new output layout NHWC (C has alignment restrictions) .
- Improve support for non-4D input.
- Add more examples such as rknn_yolov5_android_apk_demo and rknn_internal_mem_reuse_demo.
- Bug fix.

### 1.3.0

- Support RV1103/RV1106（Beta SDK）
- rknn_tensor_attr support w_stride(rename from stride) and h_stride
- Rename rknn_destroy_mem()
- Support more NPU operators, such as Where, Resize, Pad, Reshape, Transpose etc.
- RK3588 support multi-batch multi-core mode
- When RKNN_LOG_LEVEL=4, it supports to display the MACs utilization and bandwidth occupation of each layer.
- Bug fix

### 1.2.0

- Support RK3588
- Support more operators, such as GRU、Swish、LayerNorm etc.
- Reduce memory usage
- Improve zero-copy interface implementation
- Bug fix

### 1.1.0

   - Support INT8+FP16 mixed quantization to improve model accuracy
   - Support specifying input and output dtype, which can be solidified into the model
   - Support multiple inputs of the model with different channel mean/std
   - Improve the stability of multi-thread + multi-process runtime
   - Support flashing cache for fd pointed to internal tensor memory which are allocated by users
   - Improve dumping internal layer results of the model
   - Add rknn_server application as proxy between PC and board
   - Support more operators, such as HardSigmoid、HardSwish、Gather、ReduceMax、Elu
   - Add LSTM support (structure cifg and peephole are not supported, function: layernormal, clip is not supported)
   - Bug fix


### 1.0
   - Optimize the performance of rknn_inputs_set()
   - Add more functions for zero-copy
   - Add new OP support, see OP support list document for details.
   - Add multi-process support
   - Support per-channel quantitative model
   - Bug fix


### 0.7
   - Optimize the performance of rknn_inputs_set(), especially for models whose input width is 8-byte aligned.
   - Add new OP support, see OP support list document for details.
   - Bug fix

### 0.6
   - Initial version

