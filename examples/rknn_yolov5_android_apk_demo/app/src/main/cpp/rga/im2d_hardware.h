/*
 * Copyright (C) 2021 Rockchip Electronics Co., Ltd.
 * Authors:
 *  Cerf Yu <cerf.yu@rock-chips.com>
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

#ifndef _RGA_IM2D_HARDWARE_H_
#define _RGA_IM2D_HARDWARE_H_

#include "rga.h"

typedef enum {
    IM_RGA_HW_VERSION_RGA_V_ERR_INDEX = 0x0,
    IM_RGA_HW_VERSION_RGA_1_INDEX,
    IM_RGA_HW_VERSION_RGA_1_PLUS_INDEX,
    IM_RGA_HW_VERSION_RGA_2_INDEX,
    IM_RGA_HW_VERSION_RGA_2_LITE0_INDEX,
    IM_RGA_HW_VERSION_RGA_2_LITE1_INDEX,
    IM_RGA_HW_VERSION_RGA_2_ENHANCE_INDEX,
    IM_RGA_HW_VERSION_RGA_3_INDEX,
    IM_RGA_HW_VERSION_MASK_INDEX,
} IM_RGA_HW_VERSION_INDEX;

typedef enum {
    IM_RGA_HW_VERSION_RGA_V_ERR     = 1 << IM_RGA_HW_VERSION_RGA_V_ERR_INDEX,
    IM_RGA_HW_VERSION_RGA_1         = 1 << IM_RGA_HW_VERSION_RGA_1_INDEX,
    IM_RGA_HW_VERSION_RGA_1_PLUS    = 1 << IM_RGA_HW_VERSION_RGA_1_PLUS_INDEX,
    IM_RGA_HW_VERSION_RGA_2         = 1 << IM_RGA_HW_VERSION_RGA_2_INDEX,
    IM_RGA_HW_VERSION_RGA_2_LITE0   = 1 << IM_RGA_HW_VERSION_RGA_2_LITE0_INDEX,
    IM_RGA_HW_VERSION_RGA_2_LITE1   = 1 << IM_RGA_HW_VERSION_RGA_2_LITE1_INDEX,
    IM_RGA_HW_VERSION_RGA_2_ENHANCE = 1 << IM_RGA_HW_VERSION_RGA_2_ENHANCE_INDEX,
    IM_RGA_HW_VERSION_RGA_3         = 1 << IM_RGA_HW_VERSION_RGA_3_INDEX,
    IM_RGA_HW_VERSION_MASK          = ~((~(unsigned int)0x0 << IM_RGA_HW_VERSION_MASK_INDEX) | 1),
}IM_RGA_HW_VERSION;

typedef enum {
    IM_RGA_SUPPORT_FORMAT_ERROR_INDEX = 0,
    IM_RGA_SUPPORT_FORMAT_RGB_INDEX,
    IM_RGA_SUPPORT_FORMAT_RGB_OTHER_INDEX,
    IM_RGA_SUPPORT_FORMAT_BPP_INDEX,
    IM_RGA_SUPPORT_FORMAT_YUV_420_SEMI_PLANNER_8_BIT_INDEX,
    IM_RGA_SUPPORT_FORMAT_YUV_420_SEMI_PLANNER_10_BIT_INDEX,
    IM_RGA_SUPPORT_FORMAT_YUV_420_PLANNER_8_BIT_INDEX,
    IM_RGA_SUPPORT_FORMAT_YUV_420_PLANNER_10_BIT_INDEX,
    IM_RGA_SUPPORT_FORMAT_YUV_422_SEMI_PLANNER_8_BIT_INDEX,
    IM_RGA_SUPPORT_FORMAT_YUV_422_SEMI_PLANNER_10_BIT_INDEX,
    IM_RGA_SUPPORT_FORMAT_YUV_422_PLANNER_8_BIT_INDEX,
    IM_RGA_SUPPORT_FORMAT_YUV_422_PLANNER_10_BIT_INDEX,
    IM_RGA_SUPPORT_FORMAT_YUYV_420_INDEX,
    IM_RGA_SUPPORT_FORMAT_YUYV_422_INDEX,
    IM_RGA_SUPPORT_FORMAT_YUV_400_INDEX,
    IM_RGA_SUPPORT_FORMAT_Y4_INDEX,
    IM_RGA_SUPPORT_FORMAT_RGBA2BPP_INDEX,
    IM_RGA_SUPPORT_FORMAT_MASK_INDEX,
} IM_RGA_SUPPORT_FORMAT_INDEX;

typedef enum {
    IM_RGA_SUPPORT_FORMAT_ERROR                         = 1 << IM_RGA_SUPPORT_FORMAT_ERROR_INDEX,
    IM_RGA_SUPPORT_FORMAT_RGB                           = 1 << IM_RGA_SUPPORT_FORMAT_RGB_INDEX,
    IM_RGA_SUPPORT_FORMAT_RGB_OTHER                     = 1 << IM_RGA_SUPPORT_FORMAT_RGB_OTHER_INDEX,
    IM_RGA_SUPPORT_FORMAT_BPP                           = 1 << IM_RGA_SUPPORT_FORMAT_BPP_INDEX,
    IM_RGA_SUPPORT_FORMAT_YUV_420_SEMI_PLANNER_8_BIT    = 1 << IM_RGA_SUPPORT_FORMAT_YUV_420_SEMI_PLANNER_8_BIT_INDEX,
    IM_RGA_SUPPORT_FORMAT_YUV_420_SEMI_PLANNER_10_BIT   = 1 << IM_RGA_SUPPORT_FORMAT_YUV_420_SEMI_PLANNER_10_BIT_INDEX,
    IM_RGA_SUPPORT_FORMAT_YUV_420_PLANNER_8_BIT         = 1 << IM_RGA_SUPPORT_FORMAT_YUV_420_PLANNER_8_BIT_INDEX,
    IM_RGA_SUPPORT_FORMAT_YUV_420_PLANNER_10_BIT        = 1 << IM_RGA_SUPPORT_FORMAT_YUV_420_PLANNER_10_BIT_INDEX,
    IM_RGA_SUPPORT_FORMAT_YUV_422_SEMI_PLANNER_8_BIT    = 1 << IM_RGA_SUPPORT_FORMAT_YUV_422_SEMI_PLANNER_8_BIT_INDEX,
    IM_RGA_SUPPORT_FORMAT_YUV_422_SEMI_PLANNER_10_BIT   = 1 << IM_RGA_SUPPORT_FORMAT_YUV_422_SEMI_PLANNER_10_BIT_INDEX,
    IM_RGA_SUPPORT_FORMAT_YUV_422_PLANNER_8_BIT         = 1 << IM_RGA_SUPPORT_FORMAT_YUV_422_PLANNER_8_BIT_INDEX,
    IM_RGA_SUPPORT_FORMAT_YUV_422_PLANNER_10_BIT        = 1 << IM_RGA_SUPPORT_FORMAT_YUV_422_PLANNER_10_BIT_INDEX,
    IM_RGA_SUPPORT_FORMAT_YUYV_420                      = 1 << IM_RGA_SUPPORT_FORMAT_YUYV_420_INDEX,
    IM_RGA_SUPPORT_FORMAT_YUYV_422                      = 1 << IM_RGA_SUPPORT_FORMAT_YUYV_422_INDEX,
    IM_RGA_SUPPORT_FORMAT_YUV_400                       = 1 << IM_RGA_SUPPORT_FORMAT_YUV_400_INDEX,
    IM_RGA_SUPPORT_FORMAT_Y4                            = 1 << IM_RGA_SUPPORT_FORMAT_Y4_INDEX,
    IM_RGA_SUPPORT_FORMAT_RGBA2BPP                      = 1 << IM_RGA_SUPPORT_FORMAT_RGBA2BPP_INDEX,
    IM_RGA_SUPPORT_FORMAT_MASK                          = ~((~(unsigned int)0x0 << IM_RGA_SUPPORT_FORMAT_MASK_INDEX) | 1),
} IM_RGA_SUPPORT_FORMAT;

typedef enum {
    IM_RGA_SUPPORT_FEATURE_ERROR_INDEX = 0,
    IM_RGA_SUPPORT_FEATURE_COLOR_FILL_INDEX,
    IM_RGA_SUPPORT_FEATURE_COLOR_PALETTE_INDEX,
    IM_RGA_SUPPORT_FEATURE_ROP_INDEX,
    IM_RGA_SUPPORT_FEATURE_QUANTIZE_INDEX,
    IM_RGA_SUPPORT_FEATURE_SRC1_R2Y_CSC_INDEX,
    IM_RGA_SUPPORT_FEATURE_DST_FULL_CSC_INDEX,
    IM_RGA_SUPPORT_FEATURE_FBC_INDEX,
    IM_RGA_SUPPORT_FEATURE_BLEND_YUV_INDEX,
    IM_RGA_SUPPORT_FEATURE_BT2020_INDEX,
    IM_RGA_SUPPORT_FEATURE_MOSAIC_INDEX,
    IM_RGA_SUPPORT_FEATURE_OSD_INDEX,
    IM_RGA_SUPPORT_FEATURE_PRE_INTR_INDEX,
    IM_RGA_SUPPORT_FEATURE_MASK_INDEX,
} IM_RGA_SUPPORT_FEATURE_INDEX;

typedef enum {
    IM_RGA_SUPPORT_FEATURE_ERROR          = 1 << IM_RGA_SUPPORT_FEATURE_ERROR_INDEX,
    IM_RGA_SUPPORT_FEATURE_COLOR_FILL     = 1 << IM_RGA_SUPPORT_FEATURE_COLOR_FILL_INDEX,
    IM_RGA_SUPPORT_FEATURE_COLOR_PALETTE  = 1 << IM_RGA_SUPPORT_FEATURE_COLOR_PALETTE_INDEX,
    IM_RGA_SUPPORT_FEATURE_ROP            = 1 << IM_RGA_SUPPORT_FEATURE_ROP_INDEX,
    IM_RGA_SUPPORT_FEATURE_QUANTIZE       = 1 << IM_RGA_SUPPORT_FEATURE_QUANTIZE_INDEX,
    IM_RGA_SUPPORT_FEATURE_SRC1_R2Y_CSC   = 1 << IM_RGA_SUPPORT_FEATURE_SRC1_R2Y_CSC_INDEX,
    IM_RGA_SUPPORT_FEATURE_DST_FULL_CSC   = 1 << IM_RGA_SUPPORT_FEATURE_DST_FULL_CSC_INDEX,
    IM_RGA_SUPPORT_FEATURE_FBC            = 1 << IM_RGA_SUPPORT_FEATURE_FBC_INDEX,
    IM_RGA_SUPPORT_FEATURE_BLEND_YUV      = 1 << IM_RGA_SUPPORT_FEATURE_BLEND_YUV_INDEX,
    IM_RGA_SUPPORT_FEATURE_BT2020         = 1 << IM_RGA_SUPPORT_FEATURE_BT2020_INDEX,
    IM_RGA_SUPPORT_FEATURE_MOSAIC         = 1 << IM_RGA_SUPPORT_FEATURE_MOSAIC_INDEX,
    IM_RGA_SUPPORT_FEATURE_OSD            = 1 << IM_RGA_SUPPORT_FEATURE_OSD_INDEX,
    IM_RGA_SUPPORT_FEATURE_PRE_INTR       = 1 << IM_RGA_SUPPORT_FEATURE_PRE_INTR_INDEX,
    IM_RGA_SUPPORT_FEATURE_MASK           = ~((~(unsigned int)0x0 << IM_RGA_SUPPORT_FEATURE_MASK_INDEX) | 1),
} IM_RGA_SUPPORT_FEATURE;

typedef struct {
    unsigned int version;
    unsigned int input_resolution;
    unsigned int output_resolution;
    unsigned int byte_stride;
    unsigned int scale_limit;
    unsigned int performance;
    unsigned int input_format;
    unsigned int output_format;
    unsigned int feature;
    char reserved[24];
} rga_info_table_entry;

typedef struct {
    struct rga_version_t user;
    struct rga_version_t driver;
} rga_dirver_bind_table_entry;

const rga_info_table_entry hw_info_table[] = {
    { IM_RGA_HW_VERSION_RGA_V_ERR       ,    0,    0, 0, 0, 0,   0, 0, 0, {0} },
    {   IM_RGA_HW_VERSION_RGA_1         , 8192, 2048, 4, 8, 1,
                                        /* input format */
                                        IM_RGA_SUPPORT_FORMAT_RGB |
                                        IM_RGA_SUPPORT_FORMAT_RGB_OTHER |
                                        IM_RGA_SUPPORT_FORMAT_BPP |
                                        IM_RGA_SUPPORT_FORMAT_YUV_420_SEMI_PLANNER_8_BIT |
                                        IM_RGA_SUPPORT_FORMAT_YUV_420_PLANNER_8_BIT |
                                        IM_RGA_SUPPORT_FORMAT_YUV_422_SEMI_PLANNER_8_BIT |
                                        IM_RGA_SUPPORT_FORMAT_YUV_422_PLANNER_8_BIT,
                                        /* output format */
                                        IM_RGA_SUPPORT_FORMAT_RGB |
                                        IM_RGA_SUPPORT_FORMAT_RGB_OTHER |
                                        IM_RGA_SUPPORT_FORMAT_YUV_420_SEMI_PLANNER_8_BIT |
                                        IM_RGA_SUPPORT_FORMAT_YUV_420_PLANNER_8_BIT |
                                        IM_RGA_SUPPORT_FORMAT_YUV_422_SEMI_PLANNER_8_BIT |
                                        IM_RGA_SUPPORT_FORMAT_YUV_422_PLANNER_8_BIT,
                                        /* feature */
                                        IM_RGA_SUPPORT_FEATURE_COLOR_FILL |
                                        IM_RGA_SUPPORT_FEATURE_COLOR_PALETTE |
                                        IM_RGA_SUPPORT_FEATURE_ROP,
                                        /* reserved */
                                        {0} },
    { IM_RGA_HW_VERSION_RGA_1_PLUS      , 8192, 2048, 4, 8, 1,
                                        /* input format */
                                        IM_RGA_SUPPORT_FORMAT_RGB |
                                        IM_RGA_SUPPORT_FORMAT_RGB_OTHER |
                                        IM_RGA_SUPPORT_FORMAT_BPP |
                                        IM_RGA_SUPPORT_FORMAT_YUV_420_SEMI_PLANNER_8_BIT |
                                        IM_RGA_SUPPORT_FORMAT_YUV_420_PLANNER_8_BIT |
                                        IM_RGA_SUPPORT_FORMAT_YUV_422_SEMI_PLANNER_8_BIT |
                                        IM_RGA_SUPPORT_FORMAT_YUV_422_PLANNER_8_BIT,
                                        /* output format */
                                        IM_RGA_SUPPORT_FORMAT_RGB |
                                        IM_RGA_SUPPORT_FORMAT_RGB_OTHER |
                                        IM_RGA_SUPPORT_FORMAT_YUV_420_SEMI_PLANNER_8_BIT |
                                        IM_RGA_SUPPORT_FORMAT_YUV_420_PLANNER_8_BIT |
                                        IM_RGA_SUPPORT_FORMAT_YUV_422_SEMI_PLANNER_8_BIT |
                                        IM_RGA_SUPPORT_FORMAT_YUV_422_PLANNER_8_BIT,
                                        /* feature */
                                        IM_RGA_SUPPORT_FEATURE_COLOR_FILL |
                                        IM_RGA_SUPPORT_FEATURE_COLOR_PALETTE,
                                        /* reserved */
                                        {0} },
    { IM_RGA_HW_VERSION_RGA_2           , 8192, 4096, 4, 16, 2,
                                        /* input format */
                                        IM_RGA_SUPPORT_FORMAT_RGB |
                                        IM_RGA_SUPPORT_FORMAT_RGB_OTHER |
                                        IM_RGA_SUPPORT_FORMAT_YUV_420_SEMI_PLANNER_8_BIT |
                                        IM_RGA_SUPPORT_FORMAT_YUV_420_PLANNER_8_BIT |
                                        IM_RGA_SUPPORT_FORMAT_YUV_422_SEMI_PLANNER_8_BIT |
                                        IM_RGA_SUPPORT_FORMAT_YUV_422_PLANNER_8_BIT,
                                        /* output format */
                                        IM_RGA_SUPPORT_FORMAT_RGB |
                                        IM_RGA_SUPPORT_FORMAT_RGB_OTHER |
                                        IM_RGA_SUPPORT_FORMAT_YUV_420_SEMI_PLANNER_8_BIT |
                                        IM_RGA_SUPPORT_FORMAT_YUV_420_PLANNER_8_BIT |
                                        IM_RGA_SUPPORT_FORMAT_YUV_422_SEMI_PLANNER_8_BIT |
                                        IM_RGA_SUPPORT_FORMAT_YUV_422_PLANNER_8_BIT,
                                        /* feature */
                                        IM_RGA_SUPPORT_FEATURE_COLOR_FILL |
                                        IM_RGA_SUPPORT_FEATURE_COLOR_PALETTE |
                                        IM_RGA_SUPPORT_FEATURE_ROP,
                                        /* reserved */
                                        {0} },
    { IM_RGA_HW_VERSION_RGA_2_LITE0     , 8192, 4096, 4, 8, 2,
                                        /* input format */
                                        IM_RGA_SUPPORT_FORMAT_RGB |
                                        IM_RGA_SUPPORT_FORMAT_RGB_OTHER |
                                        IM_RGA_SUPPORT_FORMAT_YUV_420_SEMI_PLANNER_8_BIT |
                                        IM_RGA_SUPPORT_FORMAT_YUV_420_PLANNER_8_BIT |
                                        IM_RGA_SUPPORT_FORMAT_YUV_422_SEMI_PLANNER_8_BIT |
                                        IM_RGA_SUPPORT_FORMAT_YUV_422_PLANNER_8_BIT,
                                        /* output format */
                                        IM_RGA_SUPPORT_FORMAT_RGB |
                                        IM_RGA_SUPPORT_FORMAT_RGB_OTHER |
                                        IM_RGA_SUPPORT_FORMAT_YUV_420_SEMI_PLANNER_8_BIT |
                                        IM_RGA_SUPPORT_FORMAT_YUV_420_PLANNER_8_BIT |
                                        IM_RGA_SUPPORT_FORMAT_YUV_422_SEMI_PLANNER_8_BIT |
                                        IM_RGA_SUPPORT_FORMAT_YUV_422_PLANNER_8_BIT,
                                        /* feature */
                                        IM_RGA_SUPPORT_FEATURE_COLOR_FILL |
                                        IM_RGA_SUPPORT_FEATURE_COLOR_PALETTE |
                                        IM_RGA_SUPPORT_FEATURE_ROP,
                                        /* reserved */
                                        {0} },
    { IM_RGA_HW_VERSION_RGA_2_LITE1     , 8192, 4096, 4, 8, 2,
                                        /* input format */
                                        IM_RGA_SUPPORT_FORMAT_RGB |
                                        IM_RGA_SUPPORT_FORMAT_RGB_OTHER |
                                        IM_RGA_SUPPORT_FORMAT_YUV_420_SEMI_PLANNER_8_BIT |
                                        IM_RGA_SUPPORT_FORMAT_YUV_420_PLANNER_8_BIT |
                                        IM_RGA_SUPPORT_FORMAT_YUV_422_SEMI_PLANNER_8_BIT |
                                        IM_RGA_SUPPORT_FORMAT_YUV_422_PLANNER_8_BIT |
                                        IM_RGA_SUPPORT_FORMAT_YUV_420_SEMI_PLANNER_10_BIT |
                                        IM_RGA_SUPPORT_FORMAT_YUV_420_PLANNER_10_BIT |
                                        IM_RGA_SUPPORT_FORMAT_YUV_422_SEMI_PLANNER_10_BIT |
                                        IM_RGA_SUPPORT_FORMAT_YUV_422_PLANNER_10_BIT,
                                        /* output format */
                                        IM_RGA_SUPPORT_FORMAT_RGB |
                                        IM_RGA_SUPPORT_FORMAT_RGB_OTHER |
                                        IM_RGA_SUPPORT_FORMAT_YUV_420_SEMI_PLANNER_8_BIT |
                                        IM_RGA_SUPPORT_FORMAT_YUV_420_PLANNER_8_BIT |
                                        IM_RGA_SUPPORT_FORMAT_YUV_422_SEMI_PLANNER_8_BIT |
                                        IM_RGA_SUPPORT_FORMAT_YUV_422_PLANNER_8_BIT,
                                        /* feature */
                                        IM_RGA_SUPPORT_FEATURE_COLOR_FILL |
                                        IM_RGA_SUPPORT_FEATURE_COLOR_PALETTE,
                                        /* reserved */
                                        {0} },
    { IM_RGA_HW_VERSION_RGA_2_ENHANCE   , 8192, 4096, 4, 16,  2,
                                        /* input format */
                                        IM_RGA_SUPPORT_FORMAT_RGB |
                                        IM_RGA_SUPPORT_FORMAT_RGB_OTHER |
                                        IM_RGA_SUPPORT_FORMAT_YUV_420_SEMI_PLANNER_8_BIT |
                                        IM_RGA_SUPPORT_FORMAT_YUV_420_PLANNER_8_BIT |
                                        IM_RGA_SUPPORT_FORMAT_YUV_422_SEMI_PLANNER_8_BIT |
                                        IM_RGA_SUPPORT_FORMAT_YUV_422_PLANNER_8_BIT |
                                        IM_RGA_SUPPORT_FORMAT_YUV_420_SEMI_PLANNER_10_BIT |
                                        IM_RGA_SUPPORT_FORMAT_YUV_420_PLANNER_10_BIT |
                                        IM_RGA_SUPPORT_FORMAT_YUV_422_SEMI_PLANNER_10_BIT |
                                        IM_RGA_SUPPORT_FORMAT_YUV_422_PLANNER_10_BIT,
                                        /* output format */
                                        IM_RGA_SUPPORT_FORMAT_RGB |
                                        IM_RGA_SUPPORT_FORMAT_RGB_OTHER |
                                        IM_RGA_SUPPORT_FORMAT_YUV_420_SEMI_PLANNER_8_BIT |
                                        IM_RGA_SUPPORT_FORMAT_YUV_420_PLANNER_8_BIT |
                                        IM_RGA_SUPPORT_FORMAT_YUV_422_SEMI_PLANNER_8_BIT |
                                        IM_RGA_SUPPORT_FORMAT_YUV_422_PLANNER_8_BIT |
                                        IM_RGA_SUPPORT_FORMAT_YUYV_420 |
                                        IM_RGA_SUPPORT_FORMAT_YUYV_422,
                                        /* feature */
                                        IM_RGA_SUPPORT_FEATURE_COLOR_FILL |
                                        IM_RGA_SUPPORT_FEATURE_COLOR_PALETTE |
                                        IM_RGA_SUPPORT_FEATURE_ROP,
                                        /* reserved */
                                        {0} },
    { IM_RGA_HW_VERSION_RGA_3           , 8176, 8128, 16, 8,  4,
                                        /* input format */
                                        IM_RGA_SUPPORT_FORMAT_RGB |
                                        IM_RGA_SUPPORT_FORMAT_YUV_420_SEMI_PLANNER_8_BIT |
                                        IM_RGA_SUPPORT_FORMAT_YUV_422_SEMI_PLANNER_8_BIT |
                                        IM_RGA_SUPPORT_FORMAT_YUV_420_SEMI_PLANNER_10_BIT |
                                        IM_RGA_SUPPORT_FORMAT_YUV_422_SEMI_PLANNER_10_BIT |
                                        IM_RGA_SUPPORT_FORMAT_YUYV_422,
                                        /* output format */
                                        IM_RGA_SUPPORT_FORMAT_RGB |
                                        IM_RGA_SUPPORT_FORMAT_YUV_420_SEMI_PLANNER_8_BIT |
                                        IM_RGA_SUPPORT_FORMAT_YUV_422_SEMI_PLANNER_8_BIT |
                                        IM_RGA_SUPPORT_FORMAT_YUV_420_SEMI_PLANNER_10_BIT |
                                        IM_RGA_SUPPORT_FORMAT_YUV_422_SEMI_PLANNER_10_BIT |
                                        IM_RGA_SUPPORT_FORMAT_YUYV_422,
                                        /* feature */
                                        IM_RGA_SUPPORT_FEATURE_FBC |
                                        IM_RGA_SUPPORT_FEATURE_BLEND_YUV |
                                        IM_RGA_SUPPORT_FEATURE_BT2020,
                                        /* reserved */
                                        {0} },
};

/* The range of the version is [min, max), that is version >= min, version < max. */
const rga_dirver_bind_table_entry driver_bind_table[] = {
    { { 0, 0, 0, "0.0.0" }, {0, 0, 0, "0.0.0" } },
    { { 1, 0, 3, "1.0.3" }, {0, 0, 0, "0.0.0" } },
    { { 1, 6, 0, "1.6.0" }, {1, 1, 5, "1.1.5" } },
    { { 1, 7, 2, "1.7.2" }, {1, 2, 0, "1.2.0" } },
    { { 1, 7, 3, "1.7.3" }, {1, 2, 4, "1.2.4" } },
};

#endif
