// Copyright (c) 2021 by Rockchip Electronics Co., Ltd. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/*-------------------------------------------
                Includes
-------------------------------------------*/
#include "opencv2/core/core.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "rknn_api.h"
#include "ssd.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include <fstream>
#include <iostream>

using namespace std;
using namespace cv;

/*-------------------------------------------
                  Functions
-------------------------------------------*/

static void dump_tensor_attr(rknn_tensor_attr* attr)
{
  printf("  index=%d, name=%s, n_dims=%d, dims=[%d, %d, %d, %d], n_elems=%d, size=%d, fmt=%s, type=%s, qnt_type=%s, "
         "zp=%d, scale=%f\n",
         attr->index, attr->name, attr->n_dims, attr->dims[0], attr->dims[1], attr->dims[2], attr->dims[3],
         attr->n_elems, attr->size, get_format_string(attr->fmt), get_type_string(attr->type),
         get_qnt_type_string(attr->qnt_type), attr->zp, attr->scale);
}

static unsigned char* load_model(const char* filename, int* model_size)
{
  FILE* fp = fopen(filename, "rb");
  if (fp == nullptr) {
    printf("fopen %s fail!\n", filename);
    return NULL;
  }

  fseek(fp, 0, SEEK_END);

  int            model_len = ftell(fp);
  unsigned char* model     = (unsigned char*)malloc(model_len);

  fseek(fp, 0, SEEK_SET);

  if (model_len != fread(model, 1, model_len, fp)) {
    printf("fread %s fail!\n", filename);
    free(model);
    return NULL;
  }

  *model_size = model_len;

  if (fp) {
    fclose(fp);
  }
  return model;
}

/*-------------------------------------------
                  Main Function
-------------------------------------------*/
int main(int argc, char** argv)
{
  const int      img_width    = 300;
  const int      img_height   = 300;
  const int      img_channels = 3;
  int            ret          = 0;
  int            model_len    = 0;
  unsigned char* model        = nullptr;
  rknn_context   ctx;

  const char* model_path = argv[1];
  const char* img_path   = argv[2];

  if (argc != 3) {
    printf("Usage:%s model image\n", argv[0]);
    return -1;
  }

  // Load image
  cv::Mat orig_img = cv::imread(img_path, 1);
  if (!orig_img.data) {
    printf("cv::imread %s fail!\n", img_path);
    return -1;
  }

  // if origin model is from Caffe, you maybe not need do BGR2RGB.
  cv::Mat orig_img_rgb;
  cv::cvtColor(orig_img, orig_img_rgb, cv::COLOR_BGR2RGB);  
  cv::Mat img      = orig_img_rgb.clone();

  if (orig_img_rgb.cols != img_width || orig_img_rgb.rows != img_height) {
    printf("resize %d %d to %d %d\n", orig_img_rgb.cols, orig_img_rgb.rows, img_width, img_height);
    cv::resize(orig_img_rgb, img, cv::Size(img_width, img_height), (0, 0), (0, 0), cv::INTER_LINEAR);
  }

  // Load RKNN Model
  printf("Loading model ...\n");
  model = load_model(model_path, &model_len);

  printf("rknn_init ...\n");
  ret = rknn_init(&ctx, model, model_len, 0, NULL);
  if (ret < 0) {
    printf("rknn_init fail! ret=%d\n", ret);
    return -1;
  }

  // Get Model Input Output Info
  rknn_input_output_num io_num;
  ret = rknn_query(ctx, RKNN_QUERY_IN_OUT_NUM, &io_num, sizeof(io_num));
  if (ret != RKNN_SUCC) {
    printf("rknn_query fail! ret=%d\n", ret);
    return -1;
  }
  printf("model input num: %d, output num: %d\n", io_num.n_input, io_num.n_output);

  printf("input tensors:\n");
  rknn_tensor_attr input_attrs[io_num.n_input];
  memset(input_attrs, 0, sizeof(input_attrs));
  for (int i = 0; i < io_num.n_input; i++) {
    input_attrs[i].index = i;
    ret                  = rknn_query(ctx, RKNN_QUERY_INPUT_ATTR, &(input_attrs[i]), sizeof(rknn_tensor_attr));
    if (ret != RKNN_SUCC) {
      printf("rknn_query fail! ret=%d\n", ret);
      return -1;
    }
    dump_tensor_attr(&(input_attrs[i]));
  }

  printf("output tensors:\n");
  rknn_tensor_attr output_attrs[io_num.n_output];
  memset(output_attrs, 0, sizeof(output_attrs));
  for (int i = 0; i < io_num.n_output; i++) {
    output_attrs[i].index = i;
    ret                   = rknn_query(ctx, RKNN_QUERY_OUTPUT_ATTR, &(output_attrs[i]), sizeof(rknn_tensor_attr));
    if (ret != RKNN_SUCC) {
      printf("rknn_query fail! ret=%d\n", ret);
      return -1;
    }
    dump_tensor_attr(&(output_attrs[i]));
  }

  // Set Input Data
  rknn_input inputs[1];
  memset(inputs, 0, sizeof(inputs));
  inputs[0].index = 0;
  inputs[0].type  = RKNN_TENSOR_UINT8;
  inputs[0].size  = img.cols * img.rows * img.channels() * sizeof(uint8_t);
  inputs[0].fmt   = RKNN_TENSOR_NHWC;
  inputs[0].buf   = img.data;

  ret = rknn_inputs_set(ctx, io_num.n_input, inputs);
  if (ret < 0) {
    printf("rknn_input_set fail! ret=%d\n", ret);
    return -1;
  }

  // Run
  printf("rknn_run\n");
  ret = rknn_run(ctx, nullptr);
  if (ret < 0) {
    printf("rknn_run fail! ret=%d\n", ret);
    return -1;
  }

  // Get Output
  rknn_output outputs[2];
  memset(outputs, 0, sizeof(outputs));
  outputs[0].want_float = 1;
  outputs[1].want_float = 1;
  ret                   = rknn_outputs_get(ctx, io_num.n_output, outputs, NULL);
  if (ret < 0) {
    printf("rknn_outputs_get fail! ret=%d\n", ret);
    return -1;
  }

  // Post Process
  detect_result_group_t detect_result_group;
  postProcessSSD((float*)(outputs[0].buf), (float*)(outputs[1].buf), orig_img.cols, orig_img.rows,
                 &detect_result_group);
  // Release rknn_outputs
  rknn_outputs_release(ctx, 2, outputs);

  // Draw Objects
  for (int i = 0; i < detect_result_group.count; i++) {
    detect_result_t* det_result = &(detect_result_group.results[i]);
    printf("%s @ (%d %d %d %d) %f\n", det_result->name, det_result->box.left, det_result->box.top,
           det_result->box.right, det_result->box.bottom, det_result->prop);
    int x1 = det_result->box.left;
    int y1 = det_result->box.top;
    int x2 = det_result->box.right;
    int y2 = det_result->box.bottom;
    rectangle(orig_img, Point(x1, y1), Point(x2, y2), Scalar(255, 0, 0, 255), 3);
    putText(orig_img, det_result->name, Point(x1, y1 - 12), 1, 2, Scalar(0, 255, 0, 255));
  }

  imwrite("./out.jpg", orig_img);

  deinitPostProcessSSD();

  // Release
  if (ctx >= 0) {
    rknn_destroy(ctx);
  }
  if (model) {
    free(model);
  }
  return 0;
}
