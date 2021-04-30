## RKNPU2
  RKNPU2 provides an advanced interface to access Rockchip NPU.
  
## Support Platform
  - RK3566
  - RK3568
  
Note: The content of this project is not applicable to RK1808/RV1109/RV1126/RK3399Pro.

## ReleaseLog

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


