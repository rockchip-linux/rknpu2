/*
 * Copyright (C) 2016 Rockchip Electronics Co., Ltd.
 * Authors:
 *    Zhiqin Wei <wzq@rock-chips.com>
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

#ifndef _RGA_DRIVER_H_
#define _RGA_DRIVER_H_

#include <asm/ioctl.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

/* Use 'r' as magic number */
#define RGA_IOC_MAGIC       'r'
#define RGA_IOW(nr, type)   _IOW(RGA_IOC_MAGIC, nr, type)
#define RGA_IOR(nr, type)   _IOR(RGA_IOC_MAGIC, nr, type)
#define RGA_IOWR(nr, type)  _IOWR(RGA_IOC_MAGIC, nr, type)

#define RGA_IOC_GET_DRVIER_VERSION  RGA_IOR(0x1, struct rga_version_t)
#define RGA_IOC_GET_HW_VERSION      RGA_IOR(0x2, struct rga_hw_versions_t)
#define RGA_IOC_IMPORT_BUFFER       RGA_IOWR(0x3, struct rga_buffer_pool)
#define RGA_IOC_RELEASE_BUFFER      RGA_IOW(0x4, struct rga_buffer_pool)
#define RGA_START_CONFIG            RGA_IOR(0x5, uint32_t)
#define RGA_END_CONFIG              RGA_IOWR(0x6, struct rga_user_ctx_t)
#define RGA_CMD_CONFIG              RGA_IOWR(0x7, struct rga_user_ctx_t)
#define RGA_CANCEL_CONFIG           RGA_IOWR(0x8, uint32_t)

#define RGA_BLIT_SYNC   0x5017
#define RGA_BLIT_ASYNC  0x5018
#define RGA_FLUSH       0x5019
#define RGA_GET_RESULT  0x501a
#define RGA_GET_VERSION 0x501b

#define RGA2_BLIT_SYNC   0x6017
#define RGA2_BLIT_ASYNC  0x6018
#define RGA2_FLUSH       0x6019
#define RGA2_GET_RESULT  0x601a
#define RGA2_GET_VERSION 0x601b

#define RGA_REG_CTRL_LEN    0x8    /* 8  */
#define RGA_REG_CMD_LEN     0x1c   /* 28 */
#define RGA_CMD_BUF_SIZE    0x700  /* 16*28*4 */

#ifndef ENABLE
#define ENABLE 1
#endif

#ifndef DISABLE
#define DISABLE 0
#endif

enum rga_memory_type {
    RGA_DMA_BUFFER = 0,
    RGA_VIRTUAL_ADDRESS,
    RGA_PHYSICAL_ADDRESS
};

/* RGA process mode enum */
enum {
    bitblt_mode               = 0x0,
    color_palette_mode        = 0x1,
    color_fill_mode           = 0x2,
    line_point_drawing_mode   = 0x3,
    blur_sharp_filter_mode    = 0x4,
    pre_scaling_mode          = 0x5,
    update_palette_table_mode = 0x6,
    update_patten_buff_mode   = 0x7,
};


enum {
    rop_enable_mask          = 0x2,
    dither_enable_mask       = 0x8,
    fading_enable_mask       = 0x10,
    PD_enbale_mask           = 0x20,
};

enum {
    yuv2rgb_mode0            = 0x0,     /* BT.601 MPEG */
    yuv2rgb_mode1            = 0x1,     /* BT.601 JPEG */
    yuv2rgb_mode2            = 0x2,     /* BT.709      */

    rgb2yuv_601_full                = 0x1 << 8,
    rgb2yuv_709_full                = 0x2 << 8,
    yuv2yuv_601_limit_2_709_limit   = 0x3 << 8,
    yuv2yuv_601_limit_2_709_full    = 0x4 << 8,
    yuv2yuv_709_limit_2_601_limit   = 0x5 << 8,
    yuv2yuv_709_limit_2_601_full    = 0x6 << 8,     //not support
    yuv2yuv_601_full_2_709_limit    = 0x7 << 8,
    yuv2yuv_601_full_2_709_full     = 0x8 << 8,     //not support
    yuv2yuv_709_full_2_601_limit    = 0x9 << 8,     //not support
    yuv2yuv_709_full_2_601_full     = 0xa << 8,     //not support
    full_csc_mask = 0xf00,
};

/* RGA rotate mode */
enum {
    rotate_mode0             = 0x0,     /* no rotate */
    rotate_mode1             = 0x1,     /* rotate    */
    rotate_mode2             = 0x2,     /* x_mirror  */
    rotate_mode3             = 0x3,     /* y_mirror  */
};

enum {
    color_palette_mode0      = 0x0,     /* 1K */
    color_palette_mode1      = 0x1,     /* 2K */
    color_palette_mode2      = 0x2,     /* 4K */
    color_palette_mode3      = 0x3,     /* 8K */
};

enum {
    BB_BYPASS   = 0x0,     /* no rotate */
    BB_ROTATE   = 0x1,     /* rotate    */
    BB_X_MIRROR = 0x2,     /* x_mirror  */
    BB_Y_MIRROR = 0x3      /* y_mirror  */
};

enum {
    nearby   = 0x0,     /* no rotate */
    bilinear = 0x1,     /* rotate    */
    bicubic  = 0x2,     /* x_mirror  */
};

#define RGA_SCHED_PRIORITY_DEFAULT 0
#define RGA_SCHED_PRIORITY_MAX 6

enum {
    RGA3_SCHEDULER_CORE0    = 1 << 0,
    RGA3_SCHEDULER_CORE1    = 1 << 1,
    RGA2_SCHEDULER_CORE0    = 1 << 2,
};

/*
//          Alpha    Red     Green   Blue
{  4, 32, {{32,24,   8, 0,  16, 8,  24,16 }}, GGL_RGBA },   // RK_FORMAT_RGBA_8888
{  4, 24, {{ 0, 0,   8, 0,  16, 8,  24,16 }}, GGL_RGB  },   // RK_FORMAT_RGBX_8888
{  3, 24, {{ 0, 0,   8, 0,  16, 8,  24,16 }}, GGL_RGB  },   // RK_FORMAT_RGB_888
{  4, 32, {{32,24,  24,16,  16, 8,   8, 0 }}, GGL_BGRA },   // RK_FORMAT_BGRA_8888
{  2, 16, {{ 0, 0,  16,11,  11, 5,   5, 0 }}, GGL_RGB  },   // RK_FORMAT_RGB_565
{  2, 16, {{ 1, 0,  16,11,  11, 6,   6, 1 }}, GGL_RGBA },   // RK_FORMAT_RGBA_5551
{  2, 16, {{ 4, 0,  16,12,  12, 8,   8, 4 }}, GGL_RGBA },   // RK_FORMAT_RGBA_4444
{  3, 24, {{ 0, 0,  24,16,  16, 8,   8, 0 }}, GGL_BGR  },   // RK_FORMAT_BGB_888

*/
/* In order to be compatible with RK_FORMAT_XX and HAL_PIXEL_FORMAT_XX,
 * RK_FORMAT_XX is shifted to the left by 8 bits to distinguish.  */
typedef enum _Rga_SURF_FORMAT {
    RK_FORMAT_RGBA_8888    = 0x0 << 8,
    RK_FORMAT_RGBX_8888    = 0x1 << 8,
    RK_FORMAT_RGB_888      = 0x2 << 8,
    RK_FORMAT_BGRA_8888    = 0x3 << 8,
    RK_FORMAT_RGB_565      = 0x4 << 8,
    RK_FORMAT_RGBA_5551    = 0x5 << 8,
    RK_FORMAT_RGBA_4444    = 0x6 << 8,
    RK_FORMAT_BGR_888      = 0x7 << 8,

    RK_FORMAT_YCbCr_422_SP = 0x8 << 8,
    RK_FORMAT_YCbCr_422_P  = 0x9 << 8,
    RK_FORMAT_YCbCr_420_SP = 0xa << 8,
    RK_FORMAT_YCbCr_420_P  = 0xb << 8,

    RK_FORMAT_YCrCb_422_SP = 0xc << 8,
    RK_FORMAT_YCrCb_422_P  = 0xd << 8,
    RK_FORMAT_YCrCb_420_SP = 0xe << 8,
    RK_FORMAT_YCrCb_420_P  = 0xf << 8,

    RK_FORMAT_BPP1         = 0x10 << 8,
    RK_FORMAT_BPP2         = 0x11 << 8,
    RK_FORMAT_BPP4         = 0x12 << 8,
    RK_FORMAT_BPP8         = 0x13 << 8,

    RK_FORMAT_Y4           = 0x14 << 8,
    RK_FORMAT_YCbCr_400    = 0x15 << 8,

    RK_FORMAT_BGRX_8888    = 0x16 << 8,

    RK_FORMAT_YVYU_422     = 0x18 << 8,
    RK_FORMAT_YVYU_420     = 0x19 << 8,
    RK_FORMAT_VYUY_422     = 0x1a << 8,
    RK_FORMAT_VYUY_420     = 0x1b << 8,
    RK_FORMAT_YUYV_422     = 0x1c << 8,
    RK_FORMAT_YUYV_420     = 0x1d << 8,
    RK_FORMAT_UYVY_422     = 0x1e << 8,
    RK_FORMAT_UYVY_420     = 0x1f << 8,

    RK_FORMAT_YCbCr_420_SP_10B = 0x20 << 8,
    RK_FORMAT_YCrCb_420_SP_10B = 0x21 << 8,
    RK_FORMAT_YCbCr_422_SP_10B = 0x22 << 8,
    RK_FORMAT_YCrCb_422_SP_10B = 0x23 << 8,
    /* For compatibility with misspellings */
    RK_FORMAT_YCbCr_422_10b_SP = RK_FORMAT_YCbCr_422_SP_10B,
    RK_FORMAT_YCrCb_422_10b_SP = RK_FORMAT_YCrCb_422_SP_10B,

    RK_FORMAT_BGR_565      = 0x24 << 8,
    RK_FORMAT_BGRA_5551    = 0x25 << 8,
    RK_FORMAT_BGRA_4444    = 0x26 << 8,

    RK_FORMAT_ARGB_8888    = 0x28 << 8,
    RK_FORMAT_XRGB_8888    = 0x29 << 8,
    RK_FORMAT_ARGB_5551    = 0x2a << 8,
    RK_FORMAT_ARGB_4444    = 0x2b << 8,
    RK_FORMAT_ABGR_8888    = 0x2c << 8,
    RK_FORMAT_XBGR_8888    = 0x2d << 8,
    RK_FORMAT_ABGR_5551    = 0x2e << 8,
    RK_FORMAT_ABGR_4444    = 0x2f << 8,

    RK_FORMAT_RGBA2BPP     = 0x30 << 8,

    RK_FORMAT_UNKNOWN      = 0x100 << 8,
} RgaSURF_FORMAT;

/* RGA3 rd_mode */
enum
{
    raster_mode             = 0x1 << 0,
    fbc_mode                = 0x1 << 1,
    tile_mode               = 0x1 << 2,
};

typedef struct rga_img_info_t {
    uint64_t yrgb_addr;          /* yrgb    mem addr         */
    uint64_t uv_addr;            /* cb/cr   mem addr         */
    uint64_t v_addr;             /* cr      mem addr         */

    uint32_t format;             //definition by RK_FORMAT
    uint16_t act_w;
    uint16_t act_h;
    uint16_t x_offset;
    uint16_t y_offset;

    uint16_t vir_w;
    uint16_t vir_h;

    uint16_t endian_mode; //for BPP
    uint16_t alpha_swap;

    //used by RGA3
    uint16_t rotate_mode;
    uint16_t rd_mode;

    uint16_t is_10b_compact;
    uint16_t is_10b_endian;

    uint16_t enable;
}
rga_img_info_t;

typedef struct POINT {
    uint16_t x;
    uint16_t y;
}
POINT;

typedef struct RECT {
    uint16_t xmin;
    uint16_t xmax; // width - 1
    uint16_t ymin;
    uint16_t ymax; // height - 1
} RECT;

typedef struct MMU {
    uint8_t mmu_en;
    uint64_t base_addr;
    uint32_t mmu_flag;     /* [0] mmu enable [1] src_flush [2] dst_flush [3] CMD_flush [4~5] page size*/
} MMU;

typedef struct COLOR_FILL {
    int16_t gr_x_a;
    int16_t gr_y_a;
    int16_t gr_x_b;
    int16_t gr_y_b;
    int16_t gr_x_g;
    int16_t gr_y_g;
    int16_t gr_x_r;
    int16_t gr_y_r;
    //u8  cp_gr_saturation;
}
COLOR_FILL;

typedef struct FADING {
    uint8_t b;
    uint8_t g;
    uint8_t r;
    uint8_t res;
}
FADING;

typedef struct line_draw_t {
    POINT start_point;                  /* LineDraw_start_point                */
    POINT end_point;                    /* LineDraw_end_point                  */
    uint32_t   color;                   /* LineDraw_color                      */
    uint32_t   flag;                    /* (enum) LineDrawing mode sel         */
    uint32_t   line_width;              /* range 1~16 */
}
line_draw_t;

/* color space convert coefficient. */
typedef struct csc_coe_t {
    int16_t r_v;
    int16_t g_y;
    int16_t b_u;
    int32_t off;
} csc_coe_t;

typedef struct full_csc_t {
    uint8_t flag;
    csc_coe_t coe_y;
    csc_coe_t coe_u;
    csc_coe_t coe_v;
} full_csc_t;

struct rga_mosaic_info {
    uint8_t enable;
    uint8_t mode;
};

struct rga_pre_intr_info {
    uint8_t enable;

    uint8_t read_intr_en;
    uint8_t write_intr_en;
    uint8_t read_hold_en;
    uint32_t read_threshold;
    uint32_t write_start;
    uint32_t write_step;
};

/* MAX(min, (max - channel_value)) */
struct rga_osd_invert_factor {
    uint8_t alpha_max;
    uint8_t alpha_min;
    uint8_t yg_max;
    uint8_t yg_min;
    uint8_t crb_max;
    uint8_t crb_min;
};

struct rga_color {
    union {
        struct {
            uint8_t red;
            uint8_t green;
            uint8_t blue;
            uint8_t alpha;
        };
        uint32_t value;
    };
};

struct rga_osd_bpp2 {
    uint8_t  ac_swap;           // ac swap flag
                                // 0: CA
                                // 1: AC
    uint8_t  endian_swap;       // rgba2bpp endian swap
                                // 0: Big endian
                                // 1: Little endian
    struct rga_color color0;
    struct rga_color color1;
};

struct rga_osd_mode_ctrl {
    uint8_t mode;               // OSD cal mode:
                                //   0b'1: statistics mode
                                //   1b'1: auto inversion overlap mode
    uint8_t direction_mode;     // horizontal or vertical
                                //   0: horizontal
                                //   1: vertical
    uint8_t width_mode;         // using @fix_width or LUT width
                                //   0: fix width
                                //   1: LUT width
    uint16_t block_fix_width;   // OSD block fixed width
                                //   real width = (fix_width + 1) * 2
    uint8_t block_num;          // OSD block num
    uint16_t flags_index;       // auto invert flags index

    /* invertion config */
    uint8_t color_mode;         // selete color
                                //   0: src1 color
                                //   1: config data color
    uint8_t invert_flags_mode;  // invert flag selete
                                //   0: use RAM flag
                                //   1: usr last result
    uint8_t default_color_sel;  // default color mode
                                //   0: default is bright
                                //   1: default is dark
    uint8_t invert_enable;      // invert channel enable
                                //   1 << 0: aplha enable
                                //   1 << 1: Y/G disable
                                //   1 << 2: C/RB disable
    uint8_t invert_mode;        // invert cal mode
                                //   0: normal(max-data)
                                //   1: swap
    uint8_t invert_thresh;      // if luma > thresh, osd_flag to be 1
    uint8_t unfix_index;        // OSD width config index
};

struct rga_osd_info {
    uint8_t  enable;

    struct rga_osd_mode_ctrl mode_ctrl;
    struct rga_osd_invert_factor cal_factor;
    struct rga_osd_bpp2 bpp2_info;

    union {
        struct {
            uint32_t last_flags1;
            uint32_t last_flags0;
        };
        uint64_t last_flags;
    };

    union {
        struct {
            uint32_t cur_flags1;
            uint32_t cur_flags0;
        };
        uint64_t cur_flags;
    };
};

#define RGA_VERSION_SIZE    16
#define RGA_HW_SIZE            5

struct rga_version_t {
    uint32_t major;
    uint32_t minor;
    uint32_t revision;
    uint8_t str[RGA_VERSION_SIZE];
};

struct rga_hw_versions_t {
    struct rga_version_t version[RGA_HW_SIZE];
    uint32_t size;
};

struct rga_memory_parm {
    uint32_t width;
    uint32_t height;
    uint32_t format;

    uint32_t size;
};

struct rga_external_buffer {
    uint64_t memory;
    uint32_t type;

    uint32_t handle;
    struct rga_memory_parm memory_info;

    uint8_t reserve[252];
};

struct rga_buffer_pool {
    uint64_t buffers;
    uint32_t size;
};

struct rga_req {
    uint8_t render_mode;                  /* (enum) process mode sel */

    rga_img_info_t src;                   /* src image info */
    rga_img_info_t dst;                   /* dst image info */
    rga_img_info_t pat;                   /* patten image info */

    uint64_t rop_mask_addr;               /* rop4 mask addr */
    uint64_t LUT_addr;                    /* LUT addr */

    RECT clip;                            /* dst clip window default value is dst_vir */
                                          /* value from [0, w-1] / [0, h-1]*/

    int32_t sina;                         /* dst angle  default value 0  16.16 scan from table */
    int32_t cosa;                         /* dst angle  default value 0  16.16 scan from table */

    uint16_t alpha_rop_flag;              /* alpha rop process flag           */
                                          /* ([0] = 1 alpha_rop_enable)       */
                                          /* ([1] = 1 rop enable)             */
                                          /* ([2] = 1 fading_enable)          */
                                          /* ([3] = 1 PD_enable)              */
                                          /* ([4] = 1 alpha cal_mode_sel)     */
                                          /* ([5] = 1 dither_enable)          */
                                          /* ([6] = 1 gradient fill mode sel) */
                                          /* ([7] = 1 AA_enable)              */
                                          /* ([8] = 1 nn_quantize)            */
                                          /* ([9] = 1 Real color mode)        */

    uint8_t  scale_mode;                  /* 0 nearst / 1 bilnear / 2 bicubic */

    uint32_t color_key_max;               /* color key max */
    uint32_t color_key_min;               /* color key min */

    uint32_t fg_color;                    /* foreground color */
    uint32_t bg_color;                    /* background color */

    COLOR_FILL gr_color;                  /* color fill use gradient */

    line_draw_t line_draw_info;

    FADING fading;

    uint8_t PD_mode;                      /* porter duff alpha mode sel */

    uint8_t alpha_global_value;           /* global alpha value */

    uint16_t rop_code;                    /* rop2/3/4 code  scan from rop code table*/

    uint8_t bsfilter_flag;                /* [2] 0 blur 1 sharp / [1:0] filter_type*/

    uint8_t palette_mode;                 /* (enum) color palatte  0/1bpp, 1/2bpp 2/4bpp 3/8bpp*/

    uint8_t yuv2rgb_mode;                 /* (enum) BT.601 MPEG / BT.601 JPEG / BT.709  */

    uint8_t endian_mode;                  /* 0/big endian 1/little endian*/

    uint8_t rotate_mode;                  /* (enum) rotate mode  */
                                          /* 0x0,     no rotate  */
                                          /* 0x1,     rotate     */
                                          /* 0x2,     x_mirror   */
                                          /* 0x3,     y_mirror   */

    uint8_t color_fill_mode;              /* 0 solid color / 1 patten color */

    MMU mmu_info;                         /* mmu information */

    uint8_t  alpha_rop_mode;              /* ([0~1] alpha mode)       */
                                          /* ([2~3] rop   mode)       */
                                          /* ([4]   zero  mode en)    */
                                          /* ([5]   dst   alpha mode) (RGA1) */

    uint8_t  src_trans_mode;

    uint8_t  dither_mode;

    full_csc_t full_csc;                  /* full color space convert */

    int32_t in_fence_fd;
    uint8_t core;
    uint8_t priority;
    int32_t out_fence_fd;

    uint8_t handle_flag;

    /* RGA2 1106 add */
    struct rga_mosaic_info mosaic_info;

    uint8_t uvhds_mode;
    uint8_t uvvds_mode;

    struct rga_osd_info osd_info;

    struct rga_pre_intr_info pre_intr_info;

    uint8_t reservr[59];
};

struct rga_user_ctx_t {
    uint64_t cmd_ptr;
    uint32_t cmd_num;
    uint32_t id;
    uint32_t sync_mode;
    uint32_t out_fence_fd;

    uint32_t mpi_config_flags;

    uint8_t reservr[124];
};

#ifdef __cplusplus
}
#endif

#endif /*_RK29_IPP_DRIVER_H_*/
