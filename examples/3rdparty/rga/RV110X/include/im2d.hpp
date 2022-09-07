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
#ifndef _im2d_hpp_
#define _im2d_hpp_

#include "im2d.h"
#include "RgaUtils.h"

#ifdef ANDROID

#include <ui/GraphicBuffer.h>

using namespace android;
#endif

/*
 * Import external buffers into RGA driver.
 *
 * @param fd/va/pa
 *      Select dma_fd/virtual_address/physical_address by buffer type
 * @param size
 *      Describes the size of the image buffer
 *
 * @return rga_buffer_handle_t
 */
IM_API rga_buffer_handle_t importbuffer_fd(int fd, int size);
IM_API rga_buffer_handle_t importbuffer_virtualaddr(void *va, int size);
IM_API rga_buffer_handle_t importbuffer_physicaladdr(uint64_t pa, int size);

/*
 * Import external buffers into RGA driver.
 *
 * @param fd/va/pa
 *      Select dma_fd/virtual_address/physical_address by buffer type
 * @param width
 *      Describes the pixel width stride of the image buffer
 * @param height
 *      Describes the pixel height stride of the image buffer
 * @param format
 *      Describes the pixel format of the image buffer
 *
 * @return rga_buffer_handle_t
 */
IM_API rga_buffer_handle_t importbuffer_fd(int fd, int width, int height, int format);
IM_API rga_buffer_handle_t importbuffer_virtualaddr(void *va, int width, int height, int format);
IM_API rga_buffer_handle_t importbuffer_physicaladdr(uint64_t pa, int width, int height, int format);

#undef wrapbuffer_handle
IM_API rga_buffer_t wrapbuffer_handle(rga_buffer_handle_t  handle,
                                      int width, int height,
                                      int wstride, int hstride,
                                      int format);
IM_API rga_buffer_t wrapbuffer_handle(rga_buffer_handle_t  handle,
                                      int width, int height,
                                      int format);

#if ANDROID
IM_API rga_buffer_handle_t importbuffer_GraphicBuffer_handle(buffer_handle_t hnd);
IM_API rga_buffer_handle_t importbuffer_GraphicBuffer(sp<GraphicBuffer> buf);

IM_API rga_buffer_t wrapbuffer_handle(buffer_handle_t hnd);
IM_API rga_buffer_t wrapbuffer_GraphicBuffer(sp<GraphicBuffer> buf);

#if USE_AHARDWAREBUFFER
#include <android/hardware_buffer.h>
IM_API rga_buffer_handle_t importbuffer_AHardwareBuffer(AHardwareBuffer *buf);
IM_API rga_buffer_t wrapbuffer_AHardwareBuffer(AHardwareBuffer *buf);

#endif /* USE_AHARDWAREBUFFER */
#endif /* ANDROID */

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
#undef imresize
IM_API IM_STATUS imresize(const rga_buffer_t src, rga_buffer_t dst, double fx = 0, double fy = 0, int interpolation = 0, int sync = 1, int *release_fence_fd = NULL);

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
#undef imcrop
IM_API IM_STATUS imcrop(const rga_buffer_t src, rga_buffer_t dst, im_rect rect, int sync = 1, int *release_fence_fd = NULL);

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
#undef imrotate
IM_API IM_STATUS imrotate(const rga_buffer_t src, rga_buffer_t dst, int rotation, int sync = 1, int *release_fence_fd = NULL);

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
#undef imflip
IM_API IM_STATUS imflip(const rga_buffer_t src, rga_buffer_t dst, int mode, int sync = 1, int *release_fence_fd = NULL);

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
#undef imfill
IM_API IM_STATUS imfill(rga_buffer_t dst, im_rect rect, int color, int sync = 1, int *release_fence_fd = NULL);

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
#undef impalette
IM_API IM_STATUS impalette(rga_buffer_t src, rga_buffer_t dst, rga_buffer_t lut, int sync = 1, int *release_fence_fd = NULL);

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
#undef imtranslate
IM_API IM_STATUS imtranslate(const rga_buffer_t src, rga_buffer_t dst, int x, int y, int sync = 1, int *release_fence_fd = NULL);

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
#undef imcopy
IM_API IM_STATUS imcopy(const rga_buffer_t src, rga_buffer_t dst, int sync = 1, int *release_fence_fd = NULL);

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
#undef imblend
IM_API IM_STATUS imblend(const rga_buffer_t src, rga_buffer_t dst, int mode = IM_ALPHA_BLEND_SRC_OVER, int sync = 1, int *release_fence_fd = NULL);
#undef imcomposite
IM_API IM_STATUS imcomposite(const rga_buffer_t srcA, const rga_buffer_t srcB, rga_buffer_t dst, int mode = IM_ALPHA_BLEND_SRC_OVER, int sync = 1, int *release_fence_fd = NULL);

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
#undef imcolorkey
IM_API IM_STATUS imcolorkey(const rga_buffer_t src, rga_buffer_t dst, im_colorkey_range range, int mode = IM_ALPHA_COLORKEY_NORMAL, int sync = 1, int *release_fence_fd = NULL);

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
#undef imcvtcolor
IM_API IM_STATUS imcvtcolor(rga_buffer_t src, rga_buffer_t dst, int sfmt, int dfmt, int mode = IM_COLOR_SPACE_DEFAULT, int sync = 1, int *release_fence_fd = NULL);

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
#undef imquantize
IM_API IM_STATUS imquantize(const rga_buffer_t src, rga_buffer_t dst, im_nn_t nn_info, int sync = 1, int *release_fence_fd = NULL);

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
#undef imrop
IM_API IM_STATUS imrop(const rga_buffer_t src, rga_buffer_t dst, int rop_code, int sync = 1, int *release_fence_fd = NULL);

/*
 * MOSAIC
 *
 * @param src
 * @param dst
 * @param mosaic_mode
 * @param sync
 *      wait until operation complete
 *
 * @returns success or else negative error code.
 */
IM_API IM_STATUS immosaic(const rga_buffer_t image, im_rect rect, int mosaic_mode, int sync = 1, int *release_fence_fd = NULL);

/*
 * OSD
 *
 * @param osd
 *      osd block
 * @param dst
 *      background image
 * @param osd_rect
 * @param osd_config
 *      osd mode config
 * @param sync
 *      wait until operation complete
 *
 * @returns success or else negative error code.
 */
IM_API IM_STATUS imosd(const rga_buffer_t osd,const rga_buffer_t dst,
                       const im_rect osd_rect, im_osd_t *osd_config,
                       int sync = 1, int *release_fence_fd = NULL);

/*
 * process
 *
 * @param src
 * @param dst
 * @param pat
 * @param srect
 * @param drect
 * @param prect
 * @param acquire_fence_fd
 * @param release_fence_fd
 * @param opt
 * @param usage
 *
 * @returns success or else negative error code.
 */
IM_API IM_STATUS improcess(rga_buffer_t src, rga_buffer_t dst, rga_buffer_t pat,
                           im_rect srect, im_rect drect, im_rect prect,
                           int acquire_fence_fd, int *release_fence_fd, im_opt_t *opt_ptr, int usage);
IM_API IM_STATUS improcess(rga_buffer_t src, rga_buffer_t dst, rga_buffer_t pat,
                           im_rect srect, im_rect drect, im_rect prect,
                           int acquire_fence_fd, int *release_fence_fd, im_opt_t *opt, int usage, im_ctx_id_t ctx_id);

#endif /* _im2d_hpp_ */

