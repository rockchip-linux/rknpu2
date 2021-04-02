## RKNPU2
  RKNPU2 provides an advanced interface to access Rockchip NPU.
  
## Support Platform
  - RK3566
  - RK3568
  
Note: The content of this project is not applicable to RK1808/RV1109/RV1126/RK3399Pro.

## ReleaseLog

### 0.6
   - Initial version

### 0.7
   - Optimize the performance of rknn_inputs_set(), especially for models whose input width is 8-byte aligned.
   - Added new OP support, see OP support list document for details.
   - Bug fix
