/*
 * Copyright (C) 2020 Rockchip Electronics Co., Ltd.
 * Authors:
 *  PutinLee <putin.lee@rock-chips.com>
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

#ifndef _im2d_h_
#define _im2d_h_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef IM_API
#define IM_API /* define API export as needed */
#endif

typedef enum {
    /* Rotation */
    IM_HAL_TRANSFORM_ROT_90     = 1 << 0,
    IM_HAL_TRANSFORM_ROT_180    = 1 << 1,
    IM_HAL_TRANSFORM_ROT_270    = 1 << 2,
    IM_HAL_TRANSFORM_FLIP_H     = 1 << 3,
    IM_HAL_TRANSFORM_FLIP_V     = 1 << 4,
    IM_HAL_TRANSFORM_FLIP_H_V   = 1 << 5,
    IM_HAL_TRANSFORM_MASK       = 0x3f,

    /*
     * Blend
     * Additional blend usage, can be used with both source and target configs.
     * If none of the below is set, the default "SRC over DST" is applied.
     */
    IM_ALPHA_BLEND_SRC_OVER     = 1 << 6,     /* Default, Porter-Duff "SRC over DST" */
    IM_ALPHA_BLEND_SRC          = 1 << 7,     /* Porter-Duff "SRC" */
    IM_ALPHA_BLEND_DST          = 1 << 8,     /* Porter-Duff "DST" */
    IM_ALPHA_BLEND_SRC_IN       = 1 << 9,     /* Porter-Duff "SRC in DST" */
    IM_ALPHA_BLEND_DST_IN       = 1 << 10,    /* Porter-Duff "DST in SRC" */
    IM_ALPHA_BLEND_SRC_OUT      = 1 << 11,    /* Porter-Duff "SRC out DST" */
    IM_ALPHA_BLEND_DST_OUT      = 1 << 12,    /* Porter-Duff "DST out SRC" */
    IM_ALPHA_BLEND_DST_OVER     = 1 << 13,    /* Porter-Duff "DST over SRC" */
    IM_ALPHA_BLEND_SRC_ATOP     = 1 << 14,    /* Porter-Duff "SRC ATOP" */
    IM_ALPHA_BLEND_DST_ATOP     = 1 << 15,    /* Porter-Duff "DST ATOP" */
    IM_ALPHA_BLEND_XOR          = 1 << 16,    /* Xor */
    IM_ALPHA_BLEND_MASK         = 0x1ffc0,

    IM_ALPHA_COLORKEY_NORMAL    = 1 << 17,
    IM_ALPHA_COLORKEY_INVERTED  = 1 << 18,
    IM_ALPHA_COLORKEY_MASK      = 0x60000,

    IM_SYNC                     = 1 << 19,
    IM_CROP                     = 1 << 20,
    IM_COLOR_FILL               = 1 << 21,
    IM_COLOR_PALETTE            = 1 << 22,
    IM_NN_QUANTIZE              = 1 << 23,
    IM_ROP                      = 1 << 24,

} IM_USAGE;

typedef enum {
    IM_ROP_AND                  = 0x88,
    IM_ROP_OR                   = 0xee,
    IM_ROP_NOT_DST              = 0x55,
    IM_ROP_NOT_SRC              = 0x33,
    IM_ROP_XOR                  = 0xf6,
    IM_ROP_NOT_XOR              = 0xf9,
} IM_ROP_CODE;

typedef enum {
    IM_RGA_SUPPORT_FORMAT_ERROR_INDEX = 0,
    IM_RGA_SUPPORT_FORMAT_RGB_INDEX,
    IM_RGA_SUPPORT_FORMAT_RGB_OTHER_INDEX,
    IM_RGA_SUPPORT_FORMAT_BPP_INDEX,
    IM_RGA_SUPPORT_FORMAT_YUV_8_INDEX,
    IM_RGA_SUPPORT_FORMAT_YUV_10_INDEX,
    IM_RGA_SUPPORT_FORMAT_YUYV_420_INDEX,
    IM_RGA_SUPPORT_FORMAT_YUYV_422_INDEX,
    IM_RGA_SUPPORT_FORMAT_YUV_400_INDEX,
    IM_RGA_SUPPORT_FORMAT_MASK_INDEX,
} IM_RGA_SUPPORT_FORMAT_INDEX;

typedef enum {
    IM_RGA_SUPPORT_FORMAT_ERROR     = 1 << IM_RGA_SUPPORT_FORMAT_ERROR_INDEX,
    IM_RGA_SUPPORT_FORMAT_RGB       = 1 << IM_RGA_SUPPORT_FORMAT_RGB_INDEX,
    IM_RGA_SUPPORT_FORMAT_RGB_OTHER = 1 << IM_RGA_SUPPORT_FORMAT_RGB_OTHER_INDEX,
    IM_RGA_SUPPORT_FORMAT_BPP       = 1 << IM_RGA_SUPPORT_FORMAT_BPP_INDEX,
    IM_RGA_SUPPORT_FORMAT_YUV_8     = 1 << IM_RGA_SUPPORT_FORMAT_YUV_8_INDEX,
    IM_RGA_SUPPORT_FORMAT_YUV_10    = 1 << IM_RGA_SUPPORT_FORMAT_YUV_10_INDEX,
    IM_RGA_SUPPORT_FORMAT_YUYV_420  = 1 << IM_RGA_SUPPORT_FORMAT_YUYV_420_INDEX,
    IM_RGA_SUPPORT_FORMAT_YUYV_422  = 1 << IM_RGA_SUPPORT_FORMAT_YUYV_422_INDEX,
    IM_RGA_SUPPORT_FORMAT_YUV_400   = 1 << IM_RGA_SUPPORT_FORMAT_YUV_400_INDEX,
    IM_RGA_SUPPORT_FORMAT_MASK      = ~((~(unsigned int)0x0 << IM_RGA_SUPPORT_FORMAT_MASK_INDEX) | 1),
} IM_RGA_SUPPORT_FORMAT;

typedef enum {
    /*RGA version*/
    IM_RGA_INFO_VERSION_RGA_1           = 1<< 0,
    IM_RGA_INFO_VERSION_RGA_1_PLUS      = 1<< 1,
    IM_RGA_INFO_VERSION_RGA_2           = 1<< 2,
    IM_RGA_INFO_VERSION_RGA_2_LITE0     = 1<< 3,
    IM_RGA_INFO_VERSION_RGA_2_LITE1     = 1<< 4,
    IM_RGA_INFO_VERSION_RGA_2_ENHANCE   = 1<< 5,
    IM_RGA_INFO_VERSION_MASK            = 0x3f,
    /*RGA resolution*/
    IM_RGA_INFO_RESOLUTION_INPUT_2048   = 1 << 6,
    IM_RGA_INFO_RESOLUTION_INPUT_4096   = 1 << 7,
    IM_RGA_INFO_RESOLUTION_INPUT_8192   = 1 << 8,
    IM_RGA_INFO_RESOLUTION_INPUT_MASK   = 0x1c0,
    IM_RGA_INFO_RESOLUTION_OUTPUT_2048  = 1 << 9,
    IM_RGA_INFO_RESOLUTION_OUTPUT_4096  = 1 << 10,
    IM_RGA_INFO_RESOLUTION_OUTPUT_8192  = 1 << 11,
    IM_RGA_INFO_RESOLUTION_OUTPUT_MASK  = 0xe00,
    /*RGA scale limit*/
    IM_RGA_INFO_SCALE_LIMIT_8           = 1 << 12,
    IM_RGA_INFO_SCALE_LIMIT_16          = 1 << 13,
    IM_RGA_INFO_SCALE_LIMIT_MASK        = 0x3000,
    /*RGA suport format*/
    IM_RGA_INFO_SUPPORT_FORMAT_INPUT_RGB       = 1 << 14,
    IM_RGA_INFO_SUPPORT_FORMAT_INPUT_BP        = 1 << 15,
    IM_RGA_INFO_SUPPORT_FORMAT_INPUT_YUV_8     = 1 << 16,
    IM_RGA_INFO_SUPPORT_FORMAT_INPUT_YUV_10    = 1 << 17,
    IM_RGA_INFO_SUPPORT_FORMAT_INPUT_YUYV      = 1 << 18,
    IM_RGA_INFO_SUPPORT_FORMAT_INPUT_YUV400    = 1 << 19,
    IM_RGA_INFO_SUPPORT_FORMAT_INPUT_MASK      = 0xfc000,
    IM_RGA_INFO_SUPPORT_FORMAT_OUTPUT_RGB      = 1 << 20,
    IM_RGA_INFO_SUPPORT_FORMAT_OUTPUT_BP       = 1 << 21,
    IM_RGA_INFO_SUPPORT_FORMAT_OUTPUT_YUV_8    = 1 << 22,
    IM_RGA_INFO_SUPPORT_FORMAT_OUTPUT_YUV_10   = 1 << 23,
    IM_RGA_INFO_SUPPORT_FORMAT_OUTPUT_YUYV     = 1 << 24,
    IM_RGA_INFO_SUPPORT_FORMAT_OUTPUT_YUV400   = 1 << 25,
    IM_RGA_INFO_SUPPORT_FORMAT_OUTPUT_MASK     = 0x3f00000,
    IM_RGA_INFO_PERFORMANCE_300         = 1 << 26,
    IM_RGA_INFO_PERFORMANCE_520         = 1 << 27,
    IM_RGA_INFO_PERFORMANCE_600         = 1 << 28,
    IM_RGA_INFO_PERFORMANCE_MASK        = 0x1C000000,
} IM_RGA_INFO_USAGE;

/* Status codes, returned by any blit function */
typedef enum {
    IM_STATUS_NOERROR         =  2,
    IM_STATUS_SUCCESS         =  1,
    IM_STATUS_NOT_SUPPORTED   = -1,
    IM_STATUS_OUT_OF_MEMORY   = -2,
    IM_STATUS_INVALID_PARAM   = -3,
    IM_STATUS_ILLEGAL_PARAM   = -4,
    IM_STATUS_FAILED          =  0,
} IM_STATUS;

/* Status codes, returned by any blit function */
typedef enum {
    IM_YUV_TO_RGB_BT601_FULL      = 1 << 0,
    IM_YUV_TO_RGB_BT601_LIMIT     = 2 << 0,
    IM_YUV_TO_RGB_BT709_LIMIT     = 3 << 0,
    IM_YUV_TO_RGB_MASK            = 3 << 0,
    IM_RGB_TO_YUV_BT601_LIMIT     = 1 << 2,
    IM_RGB_TO_YUV_BT601_FULL      = 2 << 2,
    IM_RGB_TO_YUV_BT709_LIMIT     = 3 << 2,
    IM_RGB_TO_YUV_MASK            = 3 << 2,
    IM_RGB_TO_Y4                  = 1 << 4,
    IM_RGB_TO_Y4_DITHER           = 2 << 4,
    IM_RGB_TO_Y1_DITHER           = 3 << 4,
    IM_COLOR_SPACE_DEFAULT        = 0,
} IM_COLOR_SPACE_MODE;

typedef enum {
    IM_UP_SCALE,
    IM_DOWN_SCALE,
} IM_SCALE;

typedef enum {
    INTER_NEAREST,
    INTER_LINEAR,
    INTER_CUBIC,
} IM_SCALE_MODE;

/* Get RGA basic information index */
typedef enum {
    RGA_VENDOR = 0,
    RGA_VERSION,
    RGA_MAX_INPUT,
    RGA_MAX_OUTPUT,
    RGA_SCALE_LIMIT,
    RGA_INPUT_FORMAT,
    RGA_OUTPUT_FORMAT,
    RGA_EXPECTED,
    RGA_ALL,
} IM_INFORMATION;

/*rga version index*/
typedef enum {
    RGA_V_ERR                  = 0x0,
    RGA_1                      = 0x1,
    RGA_1_PLUS                 = 0x2,
    RGA_2                      = 0x3,
    RGA_2_LITE0                = 0x4,
    RGA_2_LITE1                = 0x5,
    RGA_2_ENHANCE              = 0x6,
} RGA_VERSION_NUM;

//struct AHardwareBuffer AHardwareBuffer;

typedef struct {
    RGA_VERSION_NUM version;
    unsigned int input_resolution;
    unsigned int output_resolution;
    unsigned int scale_limit;
    unsigned int performance;
    unsigned int input_format;
    unsigned int output_format;
    char reserved[32];
} rga_info_table_entry;

/* Rectangle definition */
typedef struct {
    int x;        /* upper-left x */
    int y;        /* upper-left y */
    int width;    /* width */
    int height;   /* height */
} im_rect;

typedef struct {
    int max;                    /* The Maximum value of the color key */
    int min;                    /* The minimum value of the color key */
} im_colorkey_range;


typedef struct im_nn {
    int scale_r;                /* scaling factor on R channal */
    int scale_g;                /* scaling factor on G channal */
    int scale_b;                /* scaling factor on B channal */
    int offset_r;               /* offset on R channal */
    int offset_g;               /* offset on G channal */
    int offset_b;               /* offset on B channal */
} im_nn_t;

/* im_info definition */
typedef struct {
    void* vir_addr;                     /* virtual address */
    void* phy_addr;                     /* physical address */
    int fd;                             /* shared fd */
    int width;                          /* width */
    int height;                         /* height */
    int wstride;                        /* wstride */
    int hstride;                        /* hstride */
    int format;                         /* format */
    int color_space_mode;               /* color_space_mode */
    int color;                          /* color, used by color fill */
    int global_alpha;                   /* global_alpha */
    im_colorkey_range colorkey_range;   /* range value of color key */
    im_nn_t nn;
	int rop_code;
} rga_buffer_t;

/*
 * @return error message string
 */
#define imStrError(...) \
    ({ \
        const char* err; \
        int args[] = {__VA_ARGS__}; \
        int argc = sizeof(args)/sizeof(int); \
        if (argc == 0) { \
            err = imStrError_t(IM_STATUS_INVALID_PARAM); \
        } else if (argc == 1){ \
            err = imStrError_t((IM_STATUS)args[0]); \
        } else { \
            err = ("Fatal error, imStrError() too many parameters\n"); \
            printf("Fatal error, imStrError() too many parameters\n"); \
        } \
        err; \
    })
IM_API const char* imStrError_t(IM_STATUS status);

/*
 * @return rga_buffer_t
 */
#define wrapbuffer_virtualaddr(vir_addr, width, height, format, ...) \
    ({ \
        rga_buffer_t buffer; \
        int args[] = {__VA_ARGS__}; \
        int argc = sizeof(args)/sizeof(int); \
        if (argc == 0) { \
            buffer = wrapbuffer_virtualaddr_t(vir_addr, width, height, width, height, format); \
        } else if (argc == 2){ \
            buffer = wrapbuffer_virtualaddr_t(vir_addr, width, height, args[0], args[1], format); \
        } else { \
            printf("invalid parameter\n"); \
        } \
        buffer; \
    })

#define wrapbuffer_physicaladdr(phy_addr, width, height, format, ...) \
    ({ \
        rga_buffer_t buffer; \
        int args[] = {__VA_ARGS__}; \
        int argc = sizeof(args)/sizeof(int); \
        if (argc == 0) { \
            buffer = wrapbuffer_physicaladdr_t(phy_addr, width, height, width, height, format); \
        } else if (argc == 2){ \
            buffer = wrapbuffer_physicaladdr_t(phy_addr, width, height, args[0], args[1], format); \
        } else { \
            printf("invalid parameter\n"); \
        } \
        buffer; \
    })

#define wrapbuffer_fd(fd, width, height, format, ...) \
    ({ \
        rga_buffer_t buffer; \
        int args[] = {__VA_ARGS__}; \
        int argc = sizeof(args)/sizeof(int); \
        if (argc == 0) { \
            buffer = wrapbuffer_fd_t(fd, width, height, width, height, format); \
        } else if (argc == 2){ \
            buffer = wrapbuffer_fd_t(fd, width, height, args[0], args[1], format); \
        } else { \
            printf("invalid parameter\n"); \
        } \
        buffer; \
    })

IM_API rga_buffer_t wrapbuffer_virtualaddr_t(void* vir_addr, int width, int height, int wstride, int hstride, int format);
IM_API rga_buffer_t wrapbuffer_physicaladdr_t(void* phy_addr, int width, int height, int wstride, int hstride, int format);
IM_API rga_buffer_t wrapbuffer_fd_t(int fd, int width, int height, int wstride, int hstride, int format);

/*
 * Get RGA basic information, supported resolution, supported format, etc.
 *
 * @param name
 *      RGA_VENDOR
 *      RGA_VERSION
 *      RGA_MAX_INPUT
 *      RGA_MAX_OUTPUT
 *      RGA_INPUT_FORMAT
 *      RGA_OUTPUT_FORMAT
 *      RGA_EXPECTED
 *      RGA_ALL
 *
 * @returns a usage describing properties of RGA.
 */
//IM_API int rga_get_info(rga_info_table_entry *);
IM_API IM_STATUS rga_get_info(rga_info_table_entry *return_table);

/*
 * Query RGA basic information, supported resolution, supported format, etc.
 *
 * @param name
 *      RGA_VENDOR
 *      RGA_VERSION
 *      RGA_MAX_INPUT
 *      RGA_MAX_OUTPUT
 *      RGA_INPUT_FORMAT
 *      RGA_OUTPUT_FORMAT
 *      RGA_EXPECTED
 *      RGA_ALL
 *
 * @returns a string describing properties of RGA.
 */
IM_API const char* querystring(int name);

/*
 * check RGA basic information, supported resolution, supported format, etc.
 *
 * @param src
 * @param dst
 * @param src_rect
 * @param dst_rect
 * @param mode_usage
 *
 * @returns no error or else negative error code.
 */
#define imcheck(src, dst, src_rect, dst_rect, ...) \
    ({ \
        IM_STATUS ret = IM_STATUS_NOERROR; \
        rga_buffer_t pat; \
        im_rect pat_rect; \
        memset(&pat, 0, sizeof(rga_buffer_t)); \
        memset(&pat_rect, 0, sizeof(im_rect)); \
        int args[] = {__VA_ARGS__}; \
        int argc = sizeof(args)/sizeof(int); \
        if (argc == 0) { \
            ret = imcheck_t(src, dst, pat, src_rect, dst_rect, pat_rect, 0); \
        } else if (argc == 1){ \
            ret = imcheck_t(src, dst, pat, src_rect, dst_rect, pat_rect, args[0]); \
        } else { \
            ret = IM_STATUS_FAILED; \
            printf("check failed\n"); \
        } \
        ret; \
    })
#define imcheck_composite(src, dst, pat, src_rect, dst_rect, pat_rect, ...) \
    ({ \
        IM_STATUS ret = IM_STATUS_NOERROR; \
        int args[] = {__VA_ARGS__}; \
        int argc = sizeof(args)/sizeof(int); \
        if (argc == 0) { \
            ret = imcheck_t(src, dst, pat, src_rect, dst_rect, pat_rect, 0); \
        } else if (argc == 1){ \
            ret = imcheck_t(src, dst, pat, src_rect, dst_rect, pat_rect, args[0]); \
        } else { \
            ret = IM_STATUS_FAILED; \
            printf("check failed\n"); \
        } \
        ret; \
    })
IM_API IM_STATUS imcheck_t(const rga_buffer_t src, const rga_buffer_t dst, const rga_buffer_t pat,
                           const im_rect src_rect, const im_rect dst_rect, const im_rect pat_rect, const int mdoe_usage);

/*
 * Resize
 *
 * @param src
 * @param dst
 * @param fx
 * @param fy
 * @param interpolation
 * @param sync
 *      wait until operation complete
 *
 * @returns success or else negative error code.
 */
#define imresize(src, dst, ...) \
    ({ \
        IM_STATUS ret = IM_STATUS_SUCCESS; \
        double args[] = {__VA_ARGS__}; \
        int argc = sizeof(args)/sizeof(double); \
        if (argc == 0) { \
            ret = imresize_t(src, dst, 0, 0, INTER_LINEAR, 1); \
        } else if (argc == 2){ \
            ret = imresize_t(src, dst, args[0], args[1], INTER_LINEAR, 1); \
        } else if (argc == 3){ \
            ret = imresize_t(src, dst, args[0], args[1], (int)args[2], 1); \
        } else if (argc == 4){ \
            ret = imresize_t(src, dst, args[0], args[1], (int)args[2], (int)args[3]); \
        } else { \
            ret = IM_STATUS_INVALID_PARAM; \
            printf("invalid parameter\n"); \
        } \
        ret; \
    })

#define impyramid(src, dst, direction) \
        imresize_t(src, \
                   dst, \
                   direction == IM_UP_SCALE ? 0.5 : 2, \
                   direction == IM_UP_SCALE ? 0.5 : 2, \
                   INTER_LINEAR, 1)

IM_API IM_STATUS imresize_t(const rga_buffer_t src, rga_buffer_t dst, double fx, double fy, int interpolation, int sync);

/*
 * Crop
 *
 * @param src
 * @param dst
 * @param rect
 * @param sync
 *      wait until operation complete
 *
 * @returns success or else negative error code.
 */
#define imcrop(src, dst, rect, ...) \
    ({ \
        IM_STATUS ret = IM_STATUS_SUCCESS; \
        int args[] = {__VA_ARGS__}; \
        int argc = sizeof(args)/sizeof(int); \
        if (argc == 0) { \
            ret = imcrop_t(src, dst, rect, 1); \
        } else if (argc == 1){ \
            ret = imcrop_t(src, dst, rect, args[0]);; \
        } else { \
            ret = IM_STATUS_INVALID_PARAM; \
            printf("invalid parameter\n"); \
        } \
        ret; \
    })

IM_API IM_STATUS imcrop_t(const rga_buffer_t src, rga_buffer_t dst, im_rect rect, int sync);

/*
 * rotation
 *
 * @param src
 * @param dst
 * @param rotation
 *      IM_HAL_TRANSFORM_ROT_90
 *      IM_HAL_TRANSFORM_ROT_180
 *      IM_HAL_TRANSFORM_ROT_270
 * @param sync
 *      wait until operation complete
 *
 * @returns success or else negative error code.
 */
#define imrotate(src, dst, rotation, ...) \
    ({ \
        IM_STATUS ret = IM_STATUS_SUCCESS; \
        int args[] = {__VA_ARGS__}; \
        int argc = sizeof(args)/sizeof(int); \
        if (argc == 0) { \
            ret = imrotate_t(src, dst, rotation, 1); \
        } else if (argc == 1){ \
            ret = imrotate_t(src, dst, rotation, args[0]);; \
        } else { \
            ret = IM_STATUS_INVALID_PARAM; \
            printf("invalid parameter\n"); \
        } \
        ret; \
    })

IM_API IM_STATUS imrotate_t(const rga_buffer_t src, rga_buffer_t dst, int rotation, int sync);

/*
 * flip
 *
 * @param src
 * @param dst
 * @param mode
 *      IM_HAL_TRANSFORM_FLIP_H
 *      IM_HAL_TRANSFORM_FLIP_V
 * @param sync
 *      wait until operation complete
 *
 * @returns success or else negative error code.
 */
#define imflip(src, dst, mode, ...) \
    ({ \
        IM_STATUS ret = IM_STATUS_SUCCESS; \
        int args[] = {__VA_ARGS__}; \
        int argc = sizeof(args)/sizeof(int); \
        if (argc == 0) { \
            ret = imflip_t(src, dst, mode, 1); \
        } else if (argc == 1){ \
            ret = imflip_t(src, dst, mode, args[0]);; \
        } else { \
            ret = IM_STATUS_INVALID_PARAM; \
            printf("invalid parameter\n"); \
        } \
        ret; \
    })

IM_API IM_STATUS imflip_t (const rga_buffer_t src, rga_buffer_t dst, int mode, int sync);

/*
 * fill/reset/draw
 *
 * @param src
 * @param dst
 * @param rect
 * @param color
 * @param sync
 *      wait until operation complete
 *
 * @returns success or else negative error code.
 */
#define imfill(buf, rect, color, ...) \
    ({ \
        IM_STATUS ret = IM_STATUS_SUCCESS; \
        int args[] = {__VA_ARGS__}; \
        int argc = sizeof(args)/sizeof(int); \
        if (argc == 0) { \
            ret = imfill_t(buf, rect, color, 1); \
        } else if (argc == 1){ \
            ret = imfill_t(buf, rect, color, args[0]);; \
        } else { \
            ret = IM_STATUS_INVALID_PARAM; \
            printf("invalid parameter\n"); \
        } \
        ret; \
    })

#define imreset(buf, rect, color, ...) \
    ({ \
        IM_STATUS ret = IM_STATUS_SUCCESS; \
        int args[] = {__VA_ARGS__}; \
        int argc = sizeof(args)/sizeof(int); \
        if (argc == 0) { \
            ret = imfill_t(buf, rect, color, 1); \
        } else if (argc == 1){ \
            ret = imfill_t(buf, rect, color, args[0]);; \
        } else { \
            ret = IM_STATUS_INVALID_PARAM; \
            printf("invalid parameter\n"); \
        } \
        ret; \
    })

#define imdraw(buf, rect, color, ...) \
    ({ \
        IM_STATUS ret = IM_STATUS_SUCCESS; \
        int args[] = {__VA_ARGS__}; \
        int argc = sizeof(args)/sizeof(int); \
        if (argc == 0) { \
            ret = imfill_t(buf, rect, color, 1); \
        } else if (argc == 1){ \
            ret = imfill_t(buf, rect, color, args[0]);; \
        } else { \
            ret = IM_STATUS_INVALID_PARAM; \
            printf("invalid parameter\n"); \
        } \
        ret; \
    })
IM_API IM_STATUS imfill_t(rga_buffer_t dst, im_rect rect, int color, int sync);

/*
 * palette
 *
 * @param src
 * @param dst
 * @param lut
 * @param sync
 *      wait until operation complete
 *
 * @returns success or else negative error code.
 */
#define impalette(src, dst, lut,  ...) \
    ({ \
        IM_STATUS ret = IM_STATUS_SUCCESS; \
        int args[] = {__VA_ARGS__}; \
        int argc = sizeof(args)/sizeof(int); \
        if (argc == 0) { \
            ret = impalette_t(src, dst, lut, 1); \
        } else if (argc == 1){ \
            ret = impalette_t(src, dst, lut, args[0]);; \
        } else { \
            ret = IM_STATUS_INVALID_PARAM; \
            printf("invalid parameter\n"); \
        } \
        ret; \
    })
IM_API IM_STATUS impalette_t(rga_buffer_t src, rga_buffer_t dst, rga_buffer_t lut, int sync);

/*
 * translate
 *
 * @param src
 * @param dst
 * @param x
 * @param y
 * @param sync
 *      wait until operation complete
 *
 * @returns success or else negative error code.
 */
#define imtranslate(src, dst, x, y, ...) \
    ({ \
        IM_STATUS ret = IM_STATUS_SUCCESS; \
        int args[] = {__VA_ARGS__}; \
        int argc = sizeof(args)/sizeof(int); \
        if (argc == 0) { \
            ret = imtranslate_t(src, dst, x, y, 1); \
        } else if (argc == 1){ \
            ret = imtranslate_t(src, dst, x, y, args[0]);; \
        } else { \
            ret = IM_STATUS_INVALID_PARAM; \
            printf("invalid parameter\n"); \
        } \
        ret; \
    })
IM_API IM_STATUS imtranslate_t(const rga_buffer_t src, rga_buffer_t dst, int x, int y, int sync);

/*
 * copy
 *
 * @param src
 * @param dst
 * @param sync
 *      wait until operation complete
 *
 * @returns success or else negative error code.
 */
#define imcopy(src, dst, ...) \
    ({ \
        IM_STATUS ret = IM_STATUS_SUCCESS; \
        int args[] = {__VA_ARGS__}; \
        int argc = sizeof(args)/sizeof(int); \
        if (argc == 0) { \
            ret = imcopy_t(src, dst, 1); \
        } else if (argc == 1){ \
            ret = imcopy_t(src, dst, args[0]);; \
        } else { \
            ret = IM_STATUS_INVALID_PARAM; \
            printf("invalid parameter\n"); \
        } \
        ret; \
    })

IM_API IM_STATUS imcopy_t(const rga_buffer_t src, rga_buffer_t dst, int sync);

/*
 * blend (SRC + DST -> DST or SRCA + SRCB -> DST)
 *
 * @param srcA
 * @param srcB can be NULL.
 * @param dst
 * @param mode
 *      IM_ALPHA_BLEND_MODE
 * @param sync
 *      wait until operation complete
 *
 * @returns success or else negative error code.
 */
#define imblend(srcA, dst, ...) \
    ({ \
        IM_STATUS ret = IM_STATUS_SUCCESS; \
        rga_buffer_t srcB; \
        memset(&srcB, 0x00, sizeof(rga_buffer_t)); \
        int args[] = {__VA_ARGS__}; \
        int argc = sizeof(args)/sizeof(int); \
        if (argc == 0) { \
            ret = imblend_t(srcA, srcB, dst, IM_ALPHA_BLEND_SRC_OVER, 1); \
        } else if (argc == 1){ \
            ret = imblend_t(srcA, srcB, dst, args[0], 1); \
        } else if (argc == 2){ \
            ret = imblend_t(srcA, srcB, dst, args[0], args[1]); \
        } else { \
            ret = IM_STATUS_INVALID_PARAM; \
            printf("invalid parameter\n"); \
        } \
        ret; \
    })
#define imcomposite(srcA, srcB, dst, ...) \
    ({ \
        IM_STATUS ret = IM_STATUS_SUCCESS; \
        int args[] = {__VA_ARGS__}; \
        int argc = sizeof(args)/sizeof(int); \
        if (argc == 0) { \
            ret = imblend_t(srcA, srcB, dst, IM_ALPHA_BLEND_SRC_OVER, 1); \
        } else if (argc == 1){ \
            ret = imblend_t(srcA, srcB, dst, args[0], 1); \
        } else if (argc == 2){ \
            ret = imblend_t(srcA, srcB, dst, args[0], args[1]); \
        } else { \
            ret = IM_STATUS_INVALID_PARAM; \
            printf("invalid parameter\n"); \
        } \
        ret; \
    })
IM_API IM_STATUS imblend_t(const rga_buffer_t srcA, const rga_buffer_t srcB, rga_buffer_t dst, int mode, int sync);

/*
 * color key
 *
 * @param src
 * @param dst
 * @param colorkey_range
 *      max color
 *      min color
 * @param sync
 *      wait until operation complete
 *
 * @returns success or else negative error code.
 */
#define imcolorkey(src, dst, range, ...) \
    ({ \
        IM_STATUS ret = IM_STATUS_SUCCESS; \
        int args[] = {__VA_ARGS__}; \
        int argc = sizeof(args)/sizeof(int); \
        if (argc == 0) { \
            ret = imcolorkey_t(src, dst, range, IM_ALPHA_COLORKEY_NORMAL, 1); \
        } else if (argc == 1){ \
            ret = imcolorkey_t(src, dst, range, args[0], 1); \
        } else if (argc == 2){ \
            ret = imcolorkey_t(src, dst, range, args[0], args[1]); \
        } else { \
            ret = IM_STATUS_INVALID_PARAM; \
            printf("invalid parameter\n"); \
        } \
        ret; \
    })
IM_API IM_STATUS imcolorkey_t(const rga_buffer_t src, rga_buffer_t dst, im_colorkey_range range, int mode, int sync);

/*
 * format convert
 *
 * @param src
 * @param dst
 * @param sfmt
 * @param dfmt
 * @param mode
 *      color space mode: IM_COLOR_SPACE_MODE
 * @param sync
 *      wait until operation complete
 *
 * @returns success or else negative error code.
 */
#define imcvtcolor(src, dst, sfmt, dfmt, ...) \
    ({ \
        IM_STATUS ret = IM_STATUS_SUCCESS; \
        int args[] = {__VA_ARGS__}; \
        int argc = sizeof(args)/sizeof(int); \
        if (argc == 0) { \
            ret = imcvtcolor_t(src, dst, sfmt, dfmt, IM_COLOR_SPACE_DEFAULT, 1); \
        } else if (argc == 1){ \
            ret = imcvtcolor_t(src, dst, sfmt, dfmt, args[0], 1); \
        } else if (argc == 2){ \
            ret = imcvtcolor_t(src, dst, sfmt, dfmt, args[0], args[1]); \
        } else { \
            ret = IM_STATUS_INVALID_PARAM; \
            printf("invalid parameter\n"); \
        } \
        ret; \
    })

IM_API IM_STATUS imcvtcolor_t(rga_buffer_t src, rga_buffer_t dst, int sfmt, int dfmt, int mode, int sync);

/*
 * nn quantize
 *
 * @param src
 * @param dst
 * @param nninfo
 * @param sync
 *      wait until operation complete
 *
 * @returns success or else negative error code.
 */
#define imquantize(src, dst, nn_info, ...) \
    ({ \
        IM_STATUS ret = IM_STATUS_SUCCESS; \
        int args[] = {__VA_ARGS__}; \
        int argc = sizeof(args)/sizeof(int); \
        if (argc == 0) { \
            ret = imquantize_t(src, dst, nn_info, 1); \
        } else if (argc == 1){ \
            ret = imquantize_t(src, dst, nn_info, args[0]);; \
        } else { \
            ret = IM_STATUS_INVALID_PARAM; \
            printf("invalid parameter\n"); \
        } \
        ret; \
    })

IM_API IM_STATUS imquantize_t(const rga_buffer_t src, rga_buffer_t dst, im_nn_t nn_info, int sync);

/*
 * ROP
 *
 * @param src
 * @param dst
 * @param rop_code
 * @param sync
 *      wait until operation complete
 *
 * @returns success or else negative error code.
 */
#define imrop(src, dst, rop_code, ...) \
    ({ \
        IM_STATUS ret = IM_STATUS_SUCCESS; \
        int args[] = {__VA_ARGS__}; \
        int argc = sizeof(args)/sizeof(int); \
        if (argc == 0) { \
            ret = imrop_t(src, dst, rop_code, 1); \
        } else if (argc == 1){ \
            ret = imrop_t(src, dst, rop_code, args[0]);; \
        } else { \
            ret = IM_STATUS_INVALID_PARAM; \
            printf("invalid parameter\n"); \
        } \
        ret; \
    })
IM_API IM_STATUS imrop_t(const rga_buffer_t src, rga_buffer_t dst, int rop_code, int sync);

/*
 * process
 *
 * @param src
 * @param dst
 * @param usage
 * @param ...
 *      wait until operation complete
 *
 * @returns success or else negative error code.
 */
IM_API IM_STATUS improcess(rga_buffer_t src, rga_buffer_t dst, rga_buffer_t pat, im_rect srect, im_rect drect, im_rect prect, int usage);

/*
 * block until all execution is complete
 *
 * @returns success or else negative error code.
 */
IM_API IM_STATUS imsync(void);

#ifdef __cplusplus
}
#endif
#endif /* _im2d_h_ */

