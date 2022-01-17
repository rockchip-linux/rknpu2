## RKNPU2
  RKNPU2 provides an advanced interface to access Rockchip NPU.

## Support Platform
  - RK3566
  - RK3568
  - RK3588
  - RK3588S

Note: The content of this project is not applicable to RK1808/RV1109/RV1126/RK3399Pro.
      The rknn model must be generated using RKNN Toolkit 2 instead of RKNN Toolkit.

## ReleaseLog

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

