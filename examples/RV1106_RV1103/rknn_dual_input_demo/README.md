# RKNN Dual-Input Model Inference Demo

This example demonstrates how to run a **dual-input** RKNN model on RV1106/RV1103.

The existing examples (mobilenet, yolov5) only cover single-input models. Dual-input models (e.g., image + auxiliary features) require special handling:

- **Input 0 (image)**: feed raw `uint8` RGB pixels — the NPU fuses normalization internally. Do NOT manually quantize.
- **Input 1 (features)**: manually quantize to `int8` before feeding: `q = round(x / scale + zero_point)`.
- **API**: use the standard `rknn_input` struct API. The lightweight `rknn_inputs_set` call returns error `-5` for dual-input models.
- **Output**: `int8`, dequantize with `float = (int8 - zp) * scale`.

## Model

Place your dual-input `.rknn` model in the `model/` directory.

This demo is configured for:
- image input: 64×64×3 uint8 RGB (NHWC)
- feature input: 16-dimensional float32 (quantized to int8 on-device)
- output: 8-class logits

Adjust `IMG_H`, `IMG_W`, `IMG_C`, `FEAT_DIM`, `NUM_CLASS`, and the quantization constants (`FEAT_SCALE`, `FEAT_ZP`, `OUT_SCALE`, `OUT_ZP`) in `src/main.c` to match your model.

You can query your model's quantization parameters with `rknn_query(ctx, RKNN_QUERY_INPUT_ATTR, ...)` and `rknn_query(ctx, RKNN_QUERY_OUTPUT_ATTR, ...)`.

## Build

Set the toolchain path, then run the build script:

```bash
export RK_RV1106_TOOLCHAIN=<path-to>/arm-rockchip830-linux-uclibcgnueabihf
./build-linux_RV1106.sh
```

> **Important**: Use the **uclibc** toolchain (`arm-rockchip830-linux-uclibcgnueabihf`), not the glibc version. RV1106 Buildroot uses uclibc; glibc binaries will segfault.

## Install

```bash
adb push install/rknn_dual_input_demo_Linux /userdata/
```

## Run

```bash
adb shell
cd /userdata/rknn_dual_input_demo_Linux/
export LD_LIBRARY_PATH=./lib
./rknn_dual_input_demo model/your_model.rknn image.bin features.bin
```

- `image.bin`: 12288 bytes, uint8 RGB NHWC raw pixels
- `features.bin`: 64 bytes, 16 × float32 (the demo quantizes them to int8 internally)

## Expected Output

```
=== inference result ===
  class 0: logit=1.2345
  class 1: logit=-0.6789
  ...
predicted class: 3
```

## Key Pitfalls (documented for reference)

1. **Do not manually quantize the image input.** If `mean_values`/`std_values` were set during RKNN conversion, normalization is fused into the model. Double-quantization drops accuracy from ~90% to ~30%.
2. **Lightweight API returns -5 for dual-input models.** Always use `rknn_input` structs with `index`, `type`, `size`, `fmt`, `buf`, `pass_through` fields set explicitly.
3. **Output type is fixed at conversion time.** You cannot change an int8-quantized model's output to float32 at runtime. Dequantize manually.
4. **uclibc vs glibc ABI.** The RV1106 Buildroot image uses uclibc. The cross-compiler and `librknnmrt.so` must both be the uclibc (armhf-uclibc) variants.
