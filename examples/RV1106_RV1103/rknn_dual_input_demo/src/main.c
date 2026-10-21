/*
 * rknn_dual_input_demo
 *
 * Minimal example for running a dual-input RKNN model on RV1106/RV1103.
 *
 * Input 0: image  (uint8,  NHWC, e.g. 64x64x3)  -- feed raw pixels, NPU fuses normalization
 * Input 1: features (int8, manually quantized)    -- must be quantized by user before feeding
 *
 * Build:  see build-linux_RV1106.sh
 * Run:    ./rknn_dual_input_demo <model.rknn> <image.bin> <features.bin>
 *
 * Note: The lightweight rknn_inputs_set API (passing pointer array directly) returns
 *       error -5 for dual-input models. Always use the standard rknn_input struct API.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "rknn_api.h"

/* ---------- configurable model dimensions ---------- */
#define IMG_H       64
#define IMG_W       64
#define IMG_C       3
#define IMG_SIZE    (IMG_H * IMG_W * IMG_C)   /* 12288 bytes */
#define FEAT_DIM    16
#define FEAT_SIZE   (FEAT_DIM * sizeof(int8_t)) /* 16 bytes */
#define NUM_CLASS   8

/* ---------- quantization parameters (read from your RKNN model) ---------- */
/* These are example values; replace with your model's actual parameters.
 * You can print them with rknn_query(ctx, RKNN_QUERY_INPUT_ATTR / OUTPUT_ATTR). */
#define FEAT_SCALE      0.099453f
#define FEAT_ZP         13
#define OUT_SCALE       0.255337f
#define OUT_ZP          3

static int read_file(const char *path, void *buf, int size)
{
    FILE *fp = fopen(path, "rb");
    if (!fp) {
        printf("failed to open %s\n", path);
        return -1;
    }
    int n = fread(buf, 1, size, fp);
    fclose(fp);
    if (n != size) {
        printf("file %s: expected %d bytes, got %d\n", path, size, n);
        return -1;
    }
    return 0;
}

int main(int argc, char **argv)
{
    if (argc != 4) {
        printf("Usage: %s <model.rknn> <image.bin> <features.bin>\n", argv[0]);
        printf("  image.bin    : %d bytes (uint8 RGB NHWC)\n", IMG_SIZE);
        printf("  features.bin : %d bytes (float32, will be quantized)\n", FEAT_DIM * 4);
        return -1;
    }

    const char *model_path = argv[1];
    const char *img_path   = argv[2];
    const char *feat_path  = argv[3];

    /* ---------- 1. load model ---------- */
    FILE *fp = fopen(model_path, "rb");
    if (!fp) {
        printf("failed to open model %s\n", model_path);
        return -1;
    }
    fseek(fp, 0, SEEK_END);
    int model_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    unsigned char *model_data = malloc(model_size);
    fread(model_data, 1, model_size, fp);
    fclose(fp);

    /* ---------- 2. init RKNN context ---------- */
    rknn_context ctx;
    int ret = rknn_init(&ctx, model_data, model_size, 0, NULL);
    if (ret < 0) {
        printf("rknn_init failed: %d\n", ret);
        free(model_data);
        return -1;
    }
    free(model_data); /* rknn_init copies the data, safe to free */

    /* ---------- 3. prepare inputs ---------- */
    /* image: raw uint8 RGB NHWC (0-255). Do NOT manually quantize --
     * the NPU fuses normalization (scale=1/255) internally. */
    unsigned char *img_buf = malloc(IMG_SIZE);
    if (read_file(img_path, img_buf, IMG_SIZE) < 0) {
        rknn_destroy(ctx);
        free(img_buf);
        return -1;
    }

    /* features: read float32, manually quantize to int8
     * q = round(x / scale + zero_point) */
    float feat_float[FEAT_DIM];
    if (read_file(feat_path, feat_float, FEAT_DIM * (int)sizeof(float)) < 0) {
        rknn_destroy(ctx);
        free(img_buf);
        return -1;
    }
    int8_t feat_q[FEAT_DIM];
    for (int i = 0; i < FEAT_DIM; i++) {
        int q = (int)(feat_float[i] / FEAT_SCALE + FEAT_ZP + 0.5f);
        if (q < -128) q = -128;
        if (q > 127)  q = 127;
        feat_q[i] = (int8_t)q;
    }

    /* ---------- 4. set inputs (standard rknn_input struct, NOT lightweight API) ---------- */
    rknn_input inputs[2];
    memset(inputs, 0, sizeof(inputs));

    inputs[0].index        = 0;
    inputs[0].type         = RKNN_TENSOR_UINT8;
    inputs[0].size         = IMG_SIZE;
    inputs[0].fmt          = RKNN_TENSOR_NHWC;
    inputs[0].buf          = img_buf;
    inputs[0].pass_through = 0;

    inputs[1].index        = 1;
    inputs[1].type         = RKNN_TENSOR_INT8;
    inputs[1].size         = FEAT_SIZE;
    inputs[1].fmt          = RKNN_TENSOR_NHWC;
    inputs[1].buf          = feat_q;
    inputs[1].pass_through = 0;

    ret = rknn_inputs_set(ctx, 2, inputs);
    if (ret < 0) {
        printf("rknn_inputs_set failed: %d\n", ret);
        printf("(dual-input models must use standard rknn_input struct, not lightweight API)\n");
        rknn_destroy(ctx);
        free(img_buf);
        return -1;
    }

    /* ---------- 5. run inference ---------- */
    ret = rknn_run(ctx, NULL);
    if (ret < 0) {
        printf("rknn_run failed: %d\n", ret);
        rknn_destroy(ctx);
        free(img_buf);
        return -1;
    }

    /* ---------- 6. get output (int8, then dequantize) ---------- */
    rknn_output outputs[1];
    memset(outputs, 0, sizeof(outputs));
    outputs[0].want_float = 0; /* get raw int8, dequantize manually */

    ret = rknn_outputs_get(ctx, 1, outputs, NULL);
    if (ret < 0) {
        printf("rknn_outputs_get failed: %d\n", ret);
        rknn_destroy(ctx);
        free(img_buf);
        return -1;
    }

    int8_t *out_int8 = (int8_t *)outputs[0].buf;
    float logits[NUM_CLASS];
    for (int i = 0; i < NUM_CLASS; i++) {
        logits[i] = ((float)out_int8[i] - OUT_ZP) * OUT_SCALE;
    }

    /* ---------- 7. print results ---------- */
    printf("\n=== inference result ===\n");
    int best = 0;
    for (int i = 0; i < NUM_CLASS; i++) {
        printf("  class %d: logit=%.4f\n", i, logits[i]);
        if (logits[i] > logits[best]) best = i;
    }
    printf("predicted class: %d\n", best);

    /* ---------- 8. cleanup ---------- */
    rknn_outputs_release(ctx, 1, outputs);
    rknn_destroy(ctx);
    free(img_buf);

    return 0;
}
