/****************************************************************************
 *
 *    Copyright (c) 2017 - 2018 by Rockchip Corp.  All rights reserved.
 *
 *    The material in this file is confidential and contains trade secrets
 *    of Rockchip Corporation. This is proprietary information owned by
 *    Rockchip Corporation. No part of this work may be disclosed,
 *    reproduced, copied, transmitted, or used in any way for any purpose,
 *    without the express written permission of Rockchip Corporation.
 *
 *****************************************************************************/

/*-------------------------------------------
                Includes
-------------------------------------------*/
#include "rknn_matmul_api.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

/*-------------------------------------------
                  Functions
-------------------------------------------*/
static inline int64_t getCurrentTimeUs()
{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec * 1000000 + tv.tv_usec;
}

static const char* get_dims_string(rknn_matmul_tensor_attr* attr)
{
  if (!attr->n_dims) {
    return "()";
  }
  static char dims_str[128];
  memset(&dims_str[0], 0, sizeof(dims_str));
  sprintf(&dims_str[0], "(%d", attr->dims[0]);
  for (uint32_t i = 1; i < attr->n_dims; ++i) {
    int idx = strlen(dims_str);
    sprintf(&dims_str[idx], ", %d", attr->dims[i]);
  }
  strcat(&dims_str[0], ")");
  return dims_str;
}

static void dump_matmul_tensor_attr(rknn_matmul_tensor_attr* attr)
{
  printf("  name=%s, dims=%s, size=%d, type=%s\n", attr->name, get_dims_string(attr), attr->size,
         get_type_string(attr->type));
}

static void dump_matmul_tensor(rknn_tensor_mem* tensor, rknn_matmul_tensor_attr* attr)
{
  printf("  %s%s:\n", attr->name, get_dims_string(attr));
  // normal layout
  if (attr->n_dims == 2) {
    for (uint32_t i = 0; i < attr->dims[0]; ++i) {
      for (uint32_t j = 0; j < attr->dims[1]; ++j) {
        void* virt_addr = (void*)((size_t)tensor->virt_addr + tensor->offset);
        if (attr->type == RKNN_TENSOR_INT8) {
          printf(" %2d", ((int8_t*)virt_addr)[i * attr->dims[1] + j]);
        } else if (attr->type == RKNN_TENSOR_INT32) {
          printf("  %3d", ((int32_t*)virt_addr)[i * attr->dims[1] + j]);
        }
      }
      printf("\n");
    }
    printf("\n");
  }
  // perf layout
  else if (attr->n_dims == 3) {
    for (uint32_t i = 0; i < attr->dims[0]; ++i) {
      for (uint32_t j = 0; j < attr->dims[1]; ++j) {
        for (uint32_t k = 0; k < attr->dims[2]; ++k) {
          void* virt_addr = (void*)((size_t)tensor->virt_addr + tensor->offset);
          if (attr->type == RKNN_TENSOR_INT8) {
            printf("  %2d ", ((int8_t*)virt_addr)[(i * attr->dims[1] + j) * attr->dims[2] + k]);
          } else if (attr->type == RKNN_TENSOR_INT32) {
            printf("  %2d ", ((int32_t*)virt_addr)[(i * attr->dims[1] + j) * attr->dims[2] + k]);
          }
        }
        printf("\n");
      }
      printf("\n");
    }
  }
  // native layout
  else if (attr->n_dims == 4) {
    // N / 16
    for (uint32_t n = 0; n < attr->dims[0]; ++n) {
      // K / 32
      for (uint32_t k = 0; k < attr->dims[1]; ++k) {
        // 16
        for (uint32_t nn = 0; nn < attr->dims[2]; ++nn) {
          // 32
          for (uint32_t kk = 0; kk < attr->dims[3]; kk++) {
            void* virt_addr = (void*)((size_t)tensor->virt_addr + tensor->offset);
            if (attr->type == RKNN_TENSOR_INT8) {
              printf("  %2d ",
                     ((int8_t*)virt_addr)[((n * attr->dims[1] + k) * attr->dims[2] + nn) * attr->dims[3] + kk]);
            } else if (attr->type == RKNN_TENSOR_INT32) {
              printf("  %2d ",
                     ((int32_t*)virt_addr)[((n * attr->dims[1] + k) * attr->dims[2] + nn) * attr->dims[3] + kk]);
            }
          }
          printf("\n");
        }
        printf("\n");
      }
      printf("\n");
    }
  }
}

static void print_usage(char* argv[])
{
  printf("Usage: %s [loop_count=1] [native_layout=0] [perf_layout=0]\n", argv[0]);
}

int main(int argc, char* argv[])
{
  int loop_count = 1;
  if (argc > 1) {
    if (!strcmp(argv[1], "-h")) {
      print_usage(argv);
      return 0;
    }
    loop_count = atoi(argv[1]);
  }

  // request normal or native layout for B
  int native_layout = 0;
  if (argc > 2) {
    native_layout = atoi(argv[2]);
  }

  // request normal or perf layout for A and C
  int perf_layout = 0;
  if (argc > 3) {
    perf_layout = atoi(argv[3]);
  }

  int32_t M = 4;
  int32_t K = 64;
  int32_t N = 32;

  printf("MatMul M=%d, K=%d, N=%d\n", M, K, N);

  rknn_matmul_ctx ctx;

  rknn_matmul_info info;
  memset(&info, 0, sizeof(rknn_matmul_info));
  info.M             = M;
  info.K             = K;
  info.N             = N;
  info.type          = RKNN_TENSOR_INT8;
  info.native_layout = native_layout;
  info.perf_layout   = perf_layout;

  rknn_matmul_io_attr io_attr;
  memset(&io_attr, 0, sizeof(rknn_matmul_io_attr));

  int ret = rknn_matmul_create(&ctx, &info, &io_attr);
  if (ret < 0) {
    printf("rknn_matmul_create fail! ret=%d\n", ret);
    return -1;
  }

  printf("input/output matmul tensor attribute:\n");
  dump_matmul_tensor_attr(&io_attr.A);
  dump_matmul_tensor_attr(&io_attr.B);
  dump_matmul_tensor_attr(&io_attr.C);

  // Create A
  rknn_tensor_mem* A = rknn_create_mem(ctx, io_attr.A.size);
  if (A == NULL) {
    printf("rknn_create_mem fail!\n");
    return -1;
  }
  memset(A->virt_addr, 1, A->size);

  // Create B
  rknn_tensor_mem* B = rknn_create_mem(ctx, io_attr.B.size);
  if (B == NULL) {
    printf("rknn_create_mem fail!\n");
    return -1;
  }

  // normal layout
  if (io_attr.B.n_dims == 2) {
    for (uint32_t i = 0; i < io_attr.B.dims[1]; ++i) {
      for (uint32_t j = 0; j < io_attr.B.dims[0]; ++j) {
        ((int8_t*)B->virt_addr)[i * io_attr.B.dims[0] + j] = (j % 16) + 1;
      }
    }
  }
  // native layout
  else if (io_attr.B.n_dims == 4) {
    for (uint32_t n = 0; n < io_attr.B.dims[0]; ++n) {
      for (uint32_t k = 0; k < io_attr.B.dims[1]; ++k) {
        for (uint32_t nn = 0; nn < io_attr.B.dims[2]; ++nn) {
          for (uint32_t kk = 0; kk < io_attr.B.dims[3]; ++kk) {
            ((int8_t*)B->virt_addr)[((n * io_attr.B.dims[1] + k) * io_attr.B.dims[2] + nn) * io_attr.B.dims[3] + kk] =
              nn + 1;
          }
        }
      }
    }
  }

  // Create C
  rknn_tensor_mem* C = rknn_create_mem(ctx, io_attr.C.size);
  if (C == NULL) {
    printf("rknn_create_mem fail!\n");
    return -1;
  }

  // Set A
  ret = rknn_matmul_set_io_mem(ctx, A, &io_attr.A);
  if (ret < 0) {
    printf("rknn_matmul_set_io_mem fail! ret=%d\n", ret);
    return -1;
  }

  // Set B
  ret = rknn_matmul_set_io_mem(ctx, B, &io_attr.B);
  if (ret < 0) {
    printf("rknn_matmul_set_io_mem fail! ret=%d\n", ret);
    return -1;
  }

  // Set C
  ret = rknn_matmul_set_io_mem(ctx, C, &io_attr.C);
  if (ret < 0) {
    printf("rknn_matmul_set_io_mem fail! ret=%d\n", ret);
    return -1;
  }

  // Run
  printf("Begin perf ...\n");
  for (int i = 0; i < loop_count; ++i) {
    int64_t start_us  = getCurrentTimeUs();
    ret               = rknn_matmul_run(ctx);
    int64_t elapse_us = getCurrentTimeUs() - start_us;
    if (ret < 0) {
      printf("rknn_matmul_run error %d\n", ret);
      return -1;
    }
    printf("%4d: Elapse Time = %.2fms, FPS = %.2f\n", i, elapse_us / 1000.f, 1000.f * 1000.f / elapse_us);
  }

  // Dump A/B/C tensors
  printf("matmul tensors:\n");
  dump_matmul_tensor(A, &io_attr.A);
  dump_matmul_tensor(B, &io_attr.B);
  dump_matmul_tensor(C, &io_attr.C);

  // destroy
  rknn_destroy_mem(ctx, A);
  rknn_destroy_mem(ctx, B);
  rknn_destroy_mem(ctx, C);

  rknn_matmul_destroy(ctx);

  return 0;
}