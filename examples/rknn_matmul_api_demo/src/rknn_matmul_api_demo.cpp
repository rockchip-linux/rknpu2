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
#include "Float16.h"
#include "rknn_matmul_api.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include <random>
#include <vector>
using namespace rknpu2;

/*-------------------------------------------
                  Functions
-------------------------------------------*/
static inline int64_t getCurrentTimeUs()
{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec * 1000000 + tv.tv_usec;
}

// 一维矩阵乘法函数
template <typename Ti, typename To>
std::vector<To> matrixMultiply(const Ti* A, const Ti* B, int M, int K, int N)
{
  std::vector<To> result(M * N, 0);

  for (int i = 0; i < M; ++i) {
    for (int j = 0; j < N; ++j) {
      float sum = 0;
      for (int k = 0; k < K; ++k) {
        sum += (float)A[i * K + k] * (float)B[k * N + j];
      }
      result[i * N + j] = sum;
    }
  }

  return result;
}

template <typename T>
bool arraysEqual(const std::vector<T>& arr1, const std::vector<T>& arr2, float eps = 0.0001f)
{
  if (arr1.size() != arr2.size()) {
    return false;
  }

  for (size_t i = 0; i < arr1.size(); ++i) {
    if (std::abs(arr1[i] - arr2[i]) > eps) {
      return false;
    }
  }

  return true;
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
          printf(" %4d", ((int8_t*)virt_addr)[i * attr->dims[1] + j]);
        } else if (attr->type == RKNN_TENSOR_INT32) {
          printf("  %4d", ((int32_t*)virt_addr)[i * attr->dims[1] + j]);
        } else if (attr->type == RKNN_TENSOR_FLOAT16) {
          printf(" %4.2f", (float)(((float16*)virt_addr)[i * attr->dims[1] + j]));
        } else if (attr->type == RKNN_TENSOR_FLOAT32) {
          printf(" %4.2f", ((float*)virt_addr)[i * attr->dims[1] + j]);
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
            printf("  %4d ", ((int8_t*)virt_addr)[(i * attr->dims[1] + j) * attr->dims[2] + k]);
          } else if (attr->type == RKNN_TENSOR_INT32) {
            printf("  %4d ", ((int32_t*)virt_addr)[(i * attr->dims[1] + j) * attr->dims[2] + k]);
          } else if (attr->type == RKNN_TENSOR_FLOAT16) {
            printf(" %4.2f ", (float)(((float16*)virt_addr)[(i * attr->dims[1] + j) * attr->dims[2] + k]));
          } else if (attr->type == RKNN_TENSOR_FLOAT32) {
            printf(" %4.2f ", ((float*)virt_addr)[(i * attr->dims[1] + j) * attr->dims[2] + k]);
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
              printf("  %4d ",
                     ((int8_t*)virt_addr)[((n * attr->dims[1] + k) * attr->dims[2] + nn) * attr->dims[3] + kk]);
            } else if (attr->type == RKNN_TENSOR_INT32) {
              printf("  %4d ",
                     ((int32_t*)virt_addr)[((n * attr->dims[1] + k) * attr->dims[2] + nn) * attr->dims[3] + kk]);
            } else if (attr->type == RKNN_TENSOR_FLOAT16) {
              printf(
                " %4.2f ",
                (float)(((float16*)virt_addr)[((n * attr->dims[1] + k) * attr->dims[2] + nn) * attr->dims[3] + kk]));
            } else if (attr->type == RKNN_TENSOR_FLOAT32) {
              printf(" %4.2f ",
                     ((float*)virt_addr)[((n * attr->dims[1] + k) * attr->dims[2] + nn) * attr->dims[3] + kk]);
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

  // 生成随机种子
  std::random_device rd;
  std::mt19937       gen(rd());
  std::uniform_int_distribution<> int_dis(-128, 127);
  std::normal_distribution<> float_dis(0.0, 1.0);

  // Create A
  rknn_tensor_mem* A = rknn_create_mem(ctx, io_attr.A.size);
  if (A == NULL) {
    printf("rknn_create_mem fail!\n");
    return -1;
  }
  // normal layout
  if (io_attr.A.n_dims == 2) {
    for (uint32_t i = 0; i < io_attr.A.dims[0]; ++i) {
      for (uint32_t j = 0; j < io_attr.A.dims[1]; ++j) {
        if (info.type == RKNN_TENSOR_INT8) {
          ((int8_t*)A->virt_addr)[i * io_attr.A.dims[1] + j] = int_dis(gen);
        } else if (info.type == RKNN_TENSOR_FLOAT16) {
          ((float16*)A->virt_addr)[i * io_attr.A.dims[1] + j] = float_dis(gen);
        }
      }
    }
  }
  // perf layout
  else if (io_attr.A.n_dims == 4) {
    for (uint32_t n = 0; n < io_attr.A.dims[0]; ++n) {
      for (uint32_t k = 0; k < io_attr.A.dims[1]; ++k) {
        for (uint32_t nn = 0; nn < io_attr.A.dims[2]; ++nn) {
          for (uint32_t kk = 0; kk < io_attr.A.dims[3]; ++kk) {
            if (info.type == RKNN_TENSOR_INT8) {
              ((int8_t*)A->virt_addr)[((n * io_attr.A.dims[1] + k) * io_attr.A.dims[2] + nn) * io_attr.A.dims[3] + kk] =
                1;
            } else if (info.type == RKNN_TENSOR_FLOAT16) {
              ((float16*)
                 A->virt_addr)[((n * io_attr.A.dims[1] + k) * io_attr.A.dims[2] + nn) * io_attr.A.dims[3] + kk] = 1;
            }
          }
        }
      }
    }
  }

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
        if (info.type == RKNN_TENSOR_INT8) {
          ((int8_t*)B->virt_addr)[i * io_attr.B.dims[0] + j] = int_dis(gen);
        } else if (info.type == RKNN_TENSOR_FLOAT16) {
          ((float16*)B->virt_addr)[i * io_attr.B.dims[0] + j] = float_dis(gen);
        }
      }
    }
  }
  // native layout
  else if (io_attr.B.n_dims == 4) {
    for (uint32_t n = 0; n < io_attr.B.dims[0]; ++n) {
      for (uint32_t k = 0; k < io_attr.B.dims[1]; ++k) {
        for (uint32_t nn = 0; nn < io_attr.B.dims[2]; ++nn) {
          for (uint32_t kk = 0; kk < io_attr.B.dims[3]; ++kk) {
            if (info.type == RKNN_TENSOR_INT8) {
              ((int8_t*)B->virt_addr)[((n * io_attr.B.dims[1] + k) * io_attr.B.dims[2] + nn) * io_attr.B.dims[3] + kk] =
                nn + 1;
            } else if (info.type == RKNN_TENSOR_FLOAT16) {
              ((float16*)
                 B->virt_addr)[((n * io_attr.B.dims[1] + k) * io_attr.B.dims[2] + nn) * io_attr.B.dims[3] + kk] =
                nn + 1;
            }
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

  // compare NPU res vs CPU res
  if (io_attr.A.n_dims == 2 && io_attr.B.n_dims == 2) {
    size_t C_elems = 1;
    for (int i = 0; i < io_attr.C.n_dims; ++i) {
      C_elems *= io_attr.C.dims[i];
    }
    if (info.type == RKNN_TENSOR_INT8) {
      std::vector<int32_t> cpu_res;
      cpu_res.reserve(C_elems);
      cpu_res = matrixMultiply<int8_t, int32_t>((const int8_t*)A->virt_addr, (const int8_t*)B->virt_addr, M, K, N);
      std::vector<int32_t> npu_res((int32_t*)C->virt_addr, (int32_t*)C->virt_addr + C_elems);
      if (arraysEqual<int32_t>(cpu_res, npu_res)) {
        printf("int8 matmul result is correct\n");
      } else {
        printf("int8 matmul result is wrong\n");
      }
    } else if (info.type == RKNN_TENSOR_FLOAT16) {
      std::vector<float> cpu_res;
      cpu_res.reserve(C_elems);
      cpu_res = matrixMultiply<float16, float>((const float16*)A->virt_addr, (const float16*)B->virt_addr, M, K, N);
      std::vector<float> npu_res((float*)C->virt_addr, (float*)C->virt_addr + C_elems);
      if (arraysEqual<float>(cpu_res, npu_res)) {
        printf("fp16 matmul result is correct\n");
      } else {
        printf("fp16 matmul result is wrong\n");
      }
    }
  }

  // destroy
  rknn_destroy_mem(ctx, A);
  rknn_destroy_mem(ctx, B);
  rknn_destroy_mem(ctx, C);

  rknn_matmul_destroy(ctx);

  return 0;
}