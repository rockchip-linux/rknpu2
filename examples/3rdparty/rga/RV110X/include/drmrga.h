/*
 * Copyright (C) 2016 Rockchip Electronics Co., Ltd.
 * Authors:
 *  Zhiqin Wei <wzq@rock-chips.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _rk_drm_rga_
#define _rk_drm_rga_

#include <stdint.h>
#include <errno.h>
#include <sys/cdefs.h>

#include "rga.h"

#ifdef ANDROID
#define DRMRGA_HARDWARE_MODULE_ID "librga"

#include <hardware/gralloc.h>
#include <hardware/hardware.h>
#include <system/graphics.h>
#include <cutils/native_handle.h>

#ifdef ANDROID_12
#include <hardware/hardware_rockchip.h>
#endif

#endif

#ifndef ANDROID /* LINUX */
/* flip source image horizontally (around the vertical axis) */
#define HAL_TRANSFORM_FLIP_H     0x01
/* flip source image vertically (around the horizontal axis)*/
#define HAL_TRANSFORM_FLIP_V     0x02
/* rotate source image 90 degrees clockwise */
#define HAL_TRANSFORM_ROT_90     0x04
/* rotate source image 180 degrees */
#define HAL_TRANSFORM_ROT_180    0x03
/* rotate source image 270 degrees clockwise */
#define HAL_TRANSFORM_ROT_270    0x07
#endif

#define HAL_TRANSFORM_FLIP_H_V   0x08

/*****************************************************************************/

/* for compatibility */
#define DRM_RGA_MODULE_API_VERSION      HWC_MODULE_API_VERSION_0_1
#define DRM_RGA_DEVICE_API_VERSION      HWC_DEVICE_API_VERSION_0_1
#define DRM_RGA_API_VERSION             HWC_DEVICE_API_VERSION

#define DRM_RGA_TRANSFORM_ROT_MASK      0x0000000F
#define DRM_RGA_TRANSFORM_ROT_0         0x00000000
#define DRM_RGA_TRANSFORM_ROT_90        HAL_TRANSFORM_ROT_90
#define DRM_RGA_TRANSFORM_ROT_180       HAL_TRANSFORM_ROT_180
#define DRM_RGA_TRANSFORM_ROT_270       HAL_TRANSFORM_ROT_270

#define DRM_RGA_TRANSFORM_FLIP_MASK     0x00000003
#define DRM_RGA_TRANSFORM_FLIP_H        HAL_TRANSFORM_FLIP_H
#define DRM_RGA_TRANSFORM_FLIP_V        HAL_TRANSFORM_FLIP_V

enum {
    AWIDTH                      = 0,
    AHEIGHT,
    ASTRIDE,
    AFORMAT,
    ASIZE,
    ATYPE,
};
/*****************************************************************************/

#ifndef ANDROID
/* memory type definitions. */
enum drm_rockchip_gem_mem_type {
    /* Physically Continuous memory and used as default. */
    ROCKCHIP_BO_CONTIG  = 1 << 0,
    /* cachable mapping. */
    ROCKCHIP_BO_CACHABLE    = 1 << 1,
    /* write-combine mapping. */
    ROCKCHIP_BO_WC      = 1 << 2,
    ROCKCHIP_BO_SECURE  = 1 << 3,
    ROCKCHIP_BO_MASK    = ROCKCHIP_BO_CONTIG | ROCKCHIP_BO_CACHABLE |
                ROCKCHIP_BO_WC | ROCKCHIP_BO_SECURE
};

typedef struct bo {
    int fd;
    void *ptr;
    size_t size;
    size_t offset;
    size_t pitch;
    unsigned handle;
} bo_t;
#endif

/*
   @value size:     user not need care about.For avoid read/write out of memory
 */
typedef struct rga_rect {
    int xoffset;
    int yoffset;
    int width;
    int height;
    int wstride;
    int hstride;
    int format;
    int size;
} rga_rect_t;

typedef struct rga_nn {
    int nn_flag;
    int scale_r;
    int scale_g;
    int scale_b;
    int offset_r;
    int offset_g;
    int offset_b;
} rga_nn_t;

typedef struct rga_dither {
    int enable;
    int mode;
    int lut0_l;
    int lut0_h;
    int lut1_l;
    int lut1_h;
} rga_dither_t;

/*
   @value fd:     use fd to share memory, it can be ion shard fd,and dma fd.
   @value virAddr:userspace address
   @value phyAddr:use phy address
   @value hnd:    use buffer_handle_t
 */
typedef struct rga_info {
    int fd;
    void *virAddr;
    void *phyAddr;
#ifndef ANDROID /* LINUX */
    unsigned hnd;
#else /* Android */
    buffer_handle_t hnd;
#endif
    int format;
    rga_rect_t rect;
    unsigned int blend;
    int bufferSize;
    int rotation;
    int color;
    int testLog;
    int mmuFlag;
    int colorkey_en;
    int colorkey_mode;
    int colorkey_max;
    int colorkey_min;
    int scale_mode;
    int color_space_mode;
    int sync_mode;
    rga_nn_t nn;
    rga_dither_t dither;
    int rop_code;
    int rd_mode;
    unsigned short is_10b_compact;
    unsigned short is_10b_endian;

    int in_fence_fd;
    int out_fence_fd;

    int core;
    int priority;

    unsigned short enable;

    int handle;

    struct rga_mosaic_info mosaic_info;

    struct rga_osd_info osd_info;

    struct rga_pre_intr_info pre_intr;

    int mpi_mode;
    int ctx_id;

    char reserve[402];
} rga_info_t;


typedef struct drm_rga {
    rga_rect_t src;
    rga_rect_t dst;
} drm_rga_t;

/*
   @fun rga_set_rect:For use to set the rects esayly

   @param rect:The rect user want to set,like setting the src rect:
   drm_rga_t rects;
   rga_set_rect(rects.src,0,0,1920,1080,1920,NV12);
   mean to set the src rect to the value.
 */
static inline int rga_set_rect(rga_rect_t *rect,
                               int x, int y, int w, int h, int sw, int sh, int f) {
    if (!rect)
        return -EINVAL;

    rect->xoffset = x;
    rect->yoffset = y;
    rect->width = w;
    rect->height = h;
    rect->wstride = sw;
    rect->hstride = sh;
    rect->format = f;

    return 0;
}

#ifndef ANDROID /* LINUX */
static inline void rga_set_rotation(rga_info_t *info, int angle) {
    if (angle == 90)
        info->rotation = HAL_TRANSFORM_ROT_90;
    else if (angle == 180)
        info->rotation = HAL_TRANSFORM_ROT_180;
    else if (angle == 270)
        info->rotation = HAL_TRANSFORM_ROT_270;
}
#endif
/*****************************************************************************/

#endif
