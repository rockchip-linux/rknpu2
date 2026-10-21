# RKNN 双输入模型推理示例

本示例演示如何在 RV1106/RV1103 上运行**双输入** RKNN 模型。

现有示例（mobilenet、yolov5）仅覆盖单输入模型。双输入模型（如图像 + 辅助特征）需要特殊处理：

- **输入 0（图像）**：直接喂 `uint8` 原始 RGB 像素——NPU 内部已融合归一化，**禁止手动量化**。
- **输入 1（特征）**：手动量化为 `int8` 后再喂入：`q = round(x / scale + zero_point)`。
- **API 选择**：使用标准 `rknn_input` 结构体 API。轻量 `rknn_inputs_set` 调用对双输入模型恒返回错误 `-5`。
- **输出**：`int8`，需手动反量化 `float = (int8 - zp) * scale`。

## 模型文件

将你的双输入 `.rknn` 模型放入 `model/` 目录。

本示例默认配置：
- 图像输入：64×64×3 uint8 RGB（NHWC 布局）
- 特征输入：16 维 float32（板端量化为 int8）
- 输出：8 类 logits

根据你的模型修改 `src/main.c` 中的 `IMG_H`、`IMG_W`、`IMG_C`、`FEAT_DIM`、`NUM_CLASS` 以及量化参数（`FEAT_SCALE`、`FEAT_ZP`、`OUT_SCALE`、`OUT_ZP`）。

可通过 `rknn_query(ctx, RKNN_QUERY_INPUT_ATTR, ...)` 和 `rknn_query(ctx, RKNN_QUERY_OUTPUT_ATTR, ...)` 查询模型的量化参数。

## 编译

设置交叉编译器路径，然后执行构建脚本：

```bash
export RK_RV1106_TOOLCHAIN=<path-to>/arm-rockchip830-linux-uclibcgnueabihf
./build-linux_RV1106.sh
```

> **重要**：必须使用 **uclibc** 版本交叉编译器（`arm-rockchip830-linux-uclibcgnueabihf`），不能用 glibc 版本。RV1106 Buildroot 使用 uclibc，glibc 编译的程序会段错误。

## 安装到板子

```bash
adb push install/rknn_dual_input_demo_Linux /userdata/
```

## 运行

```bash
adb shell
cd /userdata/rknn_dual_input_demo_Linux/
export LD_LIBRARY_PATH=./lib
./rknn_dual_input_demo model/your_model.rknn image.bin features.bin
```

- `image.bin`：12288 字节，uint8 RGB NHWC 原始像素
- `features.bin`：64 字节，16 × float32（示例内部会量化为 int8）

## 预期输出

```
=== inference result ===
  class 0: logit=1.2345
  class 1: logit=-0.6789
  ...
predicted class: 3
```

## 关键踩坑记录

1. **图像输入不可手动量化**。如果 RKNN 转换时设置了 `mean_values`/`std_values`，归一化已融合进模型。双重量化会导致精度从 ~90% 暴跌到 ~30%。
2. **轻量 API 对双输入模型返回 -5**。必须使用 `rknn_input` 结构体，显式设置 `index`、`type`、`size`、`fmt`、`buf`、`pass_through` 字段。
3. **输出类型在转换时固定**。int8 量化模型的输出不能在运行时改为 float32，必须手动反量化。
4. **uclibc 与 glibc 的 ABI 差异**。RV1106 Buildroot 使用 uclibc，交叉编译器和 `librknnmrt.so` 都必须是 uclibc（armhf-uclibc）版本。
