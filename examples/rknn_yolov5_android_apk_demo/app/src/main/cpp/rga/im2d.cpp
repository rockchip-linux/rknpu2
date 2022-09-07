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

#include <math.h>
#include <sstream>

#include "im2d.hpp"
#include "im2d_hardware.h"
#include "im2d_common.h"
#include "RgaUtils.h"
#include "core/rga_sync.h"

#ifdef ANDROID
#include "core/NormalRga.h"
#include "RockchipRga.h"

using namespace android;
#endif

#ifdef LINUX
#include <sys/ioctl.h>

#include "../core/NormalRga.h"
#include "../include/RockchipRga.h"

#define ALOGE(...) { printf(__VA_ARGS__); printf("\n"); }
#endif

__thread im_context_t g_im2d_context;
__thread char rga_err_str[ERR_MSG_LEN] = "The current error message is empty!";

using namespace std;

IM_API const char* imStrError_t(IM_STATUS status) {
    const char *error_type[] = {
        "No errors during operation",
        "Run successfully",
        "Unsupported function",
        "Memory overflow",
        "Invalid parameters",
        "Illegal parameters",
        "verify librga and driver version",
        "Fatal error",
        "unkown status"
    };
    static __thread char error_str[ERR_MSG_LEN] = "The current error message is empty!";
    const char *ptr = NULL;

    switch(status) {
        case IM_STATUS_NOERROR :
            return error_type[0];

        case IM_STATUS_SUCCESS :
            return error_type[1];

        case IM_STATUS_NOT_SUPPORTED :
            ptr = error_type[2];
            break;

        case IM_STATUS_OUT_OF_MEMORY :
            ptr = error_type[3];
            break;

        case IM_STATUS_INVALID_PARAM :
            ptr = error_type[4];
            break;

        case IM_STATUS_ILLEGAL_PARAM :
            ptr = error_type[5];
            break;

        case IM_STATUS_ERROR_VERSION :
            ptr = error_type[6];
            break;

        case IM_STATUS_FAILED :
            ptr = error_type[7];
            break;

        default :
            return error_type[8];
    }

    snprintf(error_str, ERR_MSG_LEN, "%s: %s", ptr, rga_err_str);
    imSetErrorMsg("No error message, it has been cleared.");

    return error_str;
}

IM_API rga_buffer_handle_t importbuffer_fd(int fd, im_handle_param_t *param) {
    return rga_import_buffer((uint64_t)fd, RGA_DMA_BUFFER, param);
}

IM_API rga_buffer_handle_t importbuffer_fd(int fd, int width, int height, int format) {
    im_handle_param_t param = {(uint32_t)width, (uint32_t)height, (uint32_t)format};
    return rga_import_buffer((uint64_t)fd, RGA_DMA_BUFFER, &param);
}

IM_API rga_buffer_handle_t importbuffer_virtualaddr(void *va, im_handle_param_t *param) {
    return rga_import_buffer((uint64_t)va, RGA_VIRTUAL_ADDRESS, param);
}

IM_API rga_buffer_handle_t importbuffer_virtualaddr(void *va, int width, int height, int format) {
    im_handle_param_t param = {(uint32_t)width, (uint32_t)height, (uint32_t)format};
    return rga_import_buffer((uint64_t)va, RGA_VIRTUAL_ADDRESS, &param);
}

IM_API rga_buffer_handle_t importbuffer_physicaladdr(uint64_t pa, im_handle_param_t *param) {
    return rga_import_buffer(pa, RGA_PHYSICAL_ADDRESS, param);
}

IM_API rga_buffer_handle_t importbuffer_physicaladdr(uint64_t pa, int width, int height, int format) {
    im_handle_param_t param = {(uint32_t)width, (uint32_t)height, (uint32_t)format};
    return rga_import_buffer(pa, RGA_PHYSICAL_ADDRESS, &param);
}

IM_API IM_STATUS releasebuffer_handle(rga_buffer_handle_t handle) {
    return rga_release_buffer(handle);
}

IM_API rga_buffer_t wrapbuffer_virtualaddr_t(void* vir_addr, int width, int height, int wstride, int hstride, int format) {
    rga_buffer_t buffer;

    memset(&buffer, 0, sizeof(rga_buffer_t));

    buffer.vir_addr = vir_addr;
    buffer.width    = width;
    buffer.height   = height;
    buffer.wstride  = wstride;
    buffer.hstride  = hstride;
    buffer.format   = format;

    return buffer;
}

IM_API rga_buffer_t wrapbuffer_physicaladdr_t(void* phy_addr, int width, int height, int wstride, int hstride, int format) {
    rga_buffer_t buffer;

    memset(&buffer, 0, sizeof(rga_buffer_t));

    buffer.phy_addr = phy_addr;
    buffer.width    = width;
    buffer.height   = height;
    buffer.wstride  = wstride;
    buffer.hstride  = hstride;
    buffer.format   = format;

    return buffer;
}

IM_API rga_buffer_t wrapbuffer_fd_t(int fd, int width, int height, int wstride, int hstride, int format) {
    rga_buffer_t buffer;

    memset(&buffer, 0, sizeof(rga_buffer_t));

    buffer.fd      = fd;
    buffer.width   = width;
    buffer.height  = height;
    buffer.wstride = wstride;
    buffer.hstride = hstride;
    buffer.format  = format;

    return buffer;
}

IM_API rga_buffer_t wrapbuffer_handle(rga_buffer_handle_t  handle,
                                      int width, int height,
                                      int wstride, int hstride,
                                      int format) {
    rga_buffer_t buffer;

    memset(&buffer, 0, sizeof(rga_buffer_t));

    buffer.handle  = handle;
    buffer.width   = width;
    buffer.height  = height;
    buffer.wstride = wstride;
    buffer.hstride = hstride;
    buffer.format  = format;

    return buffer;
}

IM_API rga_buffer_t wrapbuffer_handle(rga_buffer_handle_t  handle,
                                      int width, int height,
                                      int format) {
    return wrapbuffer_handle(handle, width, height, width, height, format);
}

#ifdef ANDROID
IM_API rga_buffer_handle_t importbuffer_GraphicBuffer_handle(buffer_handle_t hnd) {
    int ret = 0;
    int fd = -1;
    void *virt_addr = NULL;
    std::vector<int> dstAttrs;
    im_handle_param_t param;

    RockchipRga& rkRga(RockchipRga::get());

    ret = RkRgaGetHandleAttributes(hnd, &dstAttrs);
    if (ret) {
        ALOGE("rga_im2d: handle get Attributes fail ret = %d,hnd=%p", ret, &hnd);
        imSetErrorMsg("handle get Attributes fail, ret = %d,hnd = %p", ret, (void *)hnd);
        return -1;
    }

    param.width = dstAttrs.at(ASTRIDE);
    param.height = dstAttrs.at(AHEIGHT);
    param.format = dstAttrs.at(AFORMAT);

    ret = rkRga.RkRgaGetBufferFd(hnd, &fd);
    if (ret)
        ALOGE("rga_im2d: get buffer fd fail: %s, hnd=%p", strerror(errno), (void*)(hnd));

    if (fd <= 0) {
        ret = rkRga.RkRgaGetHandleMapCpuAddress(hnd, &virt_addr);
        if(!virt_addr) {
            ALOGE("rga_im2d: invaild GraphicBuffer, can not get fd and virtual address.");
            imSetErrorMsg("invaild GraphicBuffer, can not get fd and virtual address, hnd = %p", (void *)hnd);
            return -1;
        } else {
            return importbuffer_virtualaddr(virt_addr, &param);
        }
    } else {
        return importbuffer_fd(fd, &param);
    }
}

IM_API rga_buffer_handle_t importbuffer_GraphicBuffer(sp<GraphicBuffer> buf) {
    return importbuffer_GraphicBuffer_handle(buf->handle);
}

IM_API rga_buffer_handle_t importbuffer_AHardwareBuffer(AHardwareBuffer *buf) {
    GraphicBuffer *gbuffer = reinterpret_cast<GraphicBuffer*>(buf);

    return importbuffer_GraphicBuffer_handle(gbuffer->handle);
}

/*When wrapbuffer_GraphicBuffer and wrapbuffer_AHardwareBuffer are used, */
/*it is necessary to check whether fd and virtual address of the return rga_buffer_t are valid parameters*/
IM_API rga_buffer_t wrapbuffer_handle(buffer_handle_t hnd) {
    int ret = 0;
    rga_buffer_t buffer;
    std::vector<int> dstAttrs;

    RockchipRga& rkRga(RockchipRga::get());

    memset(&buffer, 0, sizeof(rga_buffer_t));

    ret = rkRga.RkRgaGetBufferFd(hnd, &buffer.fd);
    if (ret)
        ALOGE("rga_im2d: get buffer fd fail: %s, hnd=%p", strerror(errno), (void*)(hnd));

    if (buffer.fd <= 0) {
        ret = rkRga.RkRgaGetHandleMapCpuAddress(hnd, &buffer.vir_addr);
        if(!buffer.vir_addr) {
            ALOGE("rga_im2d: invaild GraphicBuffer, can not get fd and virtual address.");
            imSetErrorMsg("invaild GraphicBuffer, can not get fd and virtual address, hnd = %p", (void *)hnd);
            goto INVAILD;
        }
    }

    ret = RkRgaGetHandleAttributes(hnd, &dstAttrs);
    if (ret) {
        ALOGE("rga_im2d: handle get Attributes fail ret = %d,hnd=%p", ret, &hnd);
        imSetErrorMsg("handle get Attributes fail, ret = %d,hnd = %p", ret, (void *)hnd);
        goto INVAILD;
    }

    buffer.width   = dstAttrs.at(AWIDTH);
    buffer.height  = dstAttrs.at(AHEIGHT);
    buffer.wstride = dstAttrs.at(ASTRIDE);
    buffer.hstride = dstAttrs.at(AHEIGHT);
    buffer.format  = dstAttrs.at(AFORMAT);

    if (buffer.wstride % 16) {
        ALOGE("rga_im2d: Graphicbuffer wstride needs align to 16, please align to 16 or use other buffer types.");
        imSetErrorMsg("Graphicbuffer wstride needs align to 16, please align to 16 or use other buffer types, wstride = %d", buffer.wstride);
        goto INVAILD;
    }

INVAILD:
    return buffer;
}

IM_API rga_buffer_t wrapbuffer_GraphicBuffer(sp<GraphicBuffer> buf) {
    int ret = 0;
    rga_buffer_t buffer;
    std::vector<int> dstAttrs;

    RockchipRga& rkRga(RockchipRga::get());

    memset(&buffer, 0, sizeof(rga_buffer_t));

    ret = rkRga.RkRgaGetBufferFd(buf->handle, &buffer.fd);
    if (ret)
        ALOGE("rga_im2d: get buffer fd fail: %s, hnd=%p", strerror(errno), (void*)(buf->handle));

    if (buffer.fd <= 0) {
        ret = rkRga.RkRgaGetHandleMapCpuAddress(buf->handle, &buffer.vir_addr);
        if(!buffer.vir_addr) {
            ALOGE("rga_im2d: invaild GraphicBuffer, can not get fd and virtual address.");
            imSetErrorMsg("invaild GraphicBuffer, can not get fd and virtual address, hnd = %p", (void *)(buf->handle));
            goto INVAILD;
        }
    }

    ret = RkRgaGetHandleAttributes(buf->handle, &dstAttrs);
    if (ret) {
        ALOGE("rga_im2d: handle get Attributes fail ret = %d,hnd=%p", ret, &buf->handle);
        imSetErrorMsg("handle get Attributes fail, ret = %d, hnd = %p", ret, (void *)(buf->handle));
        goto INVAILD;
    }

    buffer.width   = dstAttrs.at(AWIDTH);
    buffer.height  = dstAttrs.at(AHEIGHT);
    buffer.wstride = dstAttrs.at(ASTRIDE);
    buffer.hstride = dstAttrs.at(AHEIGHT);
    buffer.format  = dstAttrs.at(AFORMAT);

    if (buffer.wstride % 16) {
        ALOGE("rga_im2d: Graphicbuffer wstride needs align to 16, please align to 16 or use other buffer types, wstride = %d", buffer.wstride);
        imSetErrorMsg("Graphicbuffer wstride needs align to 16, please align to 16 or use other buffer types, wstride = %d", buffer.wstride);
        goto INVAILD;
    }

INVAILD:
    return buffer;
}

#if USE_AHARDWAREBUFFER
#include <android/hardware_buffer.h>
IM_API rga_buffer_t wrapbuffer_AHardwareBuffer(AHardwareBuffer *buf) {
    int ret = 0;
    rga_buffer_t buffer;
    std::vector<int> dstAttrs;

    RockchipRga& rkRga(RockchipRga::get());

    memset(&buffer, 0, sizeof(rga_buffer_t));

    GraphicBuffer *gbuffer = reinterpret_cast<GraphicBuffer*>(buf);

    ret = rkRga.RkRgaGetBufferFd(gbuffer->handle, &buffer.fd);
    if (ret)
        ALOGE("rga_im2d: get buffer fd fail: %s, hnd=%p", strerror(errno), (void*)(gbuffer->handle));

    if (buffer.fd <= 0) {
        ret = rkRga.RkRgaGetHandleMapCpuAddress(gbuffer->handle, &buffer.vir_addr);
        if(!buffer.vir_addr) {
            ALOGE("rga_im2d: invaild GraphicBuffer, can not get fd and virtual address.");
            imSetErrorMsg("invaild GraphicBuffer, can not get fd and virtual address, hnd = %p", (void *)(gbuffer->handle));
            goto INVAILD;
        }
    }

    ret = RkRgaGetHandleAttributes(gbuffer->handle, &dstAttrs);
    if (ret) {
        ALOGE("rga_im2d: handle get Attributes fail ret = %d,hnd=%p", ret, &gbuffer->handle);
        imSetErrorMsg("handle get Attributes fail, ret = %d, hnd = %p", ret, (void *)(gbuffer->handle));
        goto INVAILD;
    }

    buffer.width   = dstAttrs.at(AWIDTH);
    buffer.height  = dstAttrs.at(AHEIGHT);
    buffer.wstride = dstAttrs.at(ASTRIDE);
    buffer.hstride = dstAttrs.at(AHEIGHT);
    buffer.format  = dstAttrs.at(AFORMAT);

    if (buffer.wstride % 16) {
        ALOGE("rga_im2d: Graphicbuffer wstride needs align to 16, please align to 16 or use other buffer types, wstride = %d", buffer.wstride);
        imSetErrorMsg("Graphicbuffer wstride needs align to 16, please align to 16 or use other buffer types, wstride = %d", buffer.wstride);
        goto INVAILD;
    }

INVAILD:
    return buffer;
}
#endif
#endif

IM_API void rga_check_perpare(rga_buffer_t *src, rga_buffer_t *dst, rga_buffer_t *pat,
                              im_rect *src_rect, im_rect *dst_rect, im_rect *pat_rect, int mode_usage) {

      if (mode_usage & IM_CROP) {
          dst_rect->width = src_rect->width;
          dst_rect->height = src_rect->height;
      }

      rga_apply_rect(src, src_rect);
      rga_apply_rect(dst, dst_rect);
      if (rga_is_buffer_valid(*pat))
          rga_apply_rect(pat, pat_rect);
}

IM_API const char* querystring(int name) {
    bool all_output = 0, all_output_prepared = 0;
    int rga_version = 0;
    long usage = 0;
    enum {
        RGA_API = 0,
    };
    const char *temp;
    const char *output_vendor = "Rockchip Electronics Co.,Ltd.";
    const char *output_name[] = {
        "RGA vendor            : ",
        "RGA version           : ",
        "Max input             : ",
        "Max output            : ",
        "Byte stride           : ",
        "Scale limit           : ",
        "Input support format  : ",
        "output support format : ",
        "RGA feature           : ",
        "expected performance  : ",
    };
    const char *version_name[] = {
        "RGA_api version       : ",
    };
    const char *output_version[] = {
        "unknown ",
        "RGA_1 ",
        "RGA_1_plus ",
        "RGA_2 ",
        "RGA_2_lite0 ",
        "RGA_2_lite1 ",
        "RGA_2_Enhance ",
        "RGA_3 ",
    };
    const char *output_resolution[] = {
        "unknown",
        "2048x2048",
        "4096x4096",
        "8192x8192",
        "8128x8128",
    };
    const char *output_scale_limit[] = {
        "unknown",
        "0.125 ~ 8",
        "0.0625 ~ 16"
    };
    const char *output_format[] = {
        "unknown",
        "RGBA_8888 RGB_888 RGB_565 ",
        "RGBA_4444 RGBA_5551 ",
        "BPP8 BPP4 BPP2 BPP1 ",
        "YUV420_sp_8bit ",
        "YUV420_sp_10bit ",
        "YUV420_p_8bit ",
        "YUV420_p_10bit ",
        "YUV422_sp_8bit ",
        "YUV422_sp_10bit ",
        "YUV422_p_8bit ",
        "YUV422_p_10bit ",
        "YUYV420 ",
        "YUYV422 ",
        "YUV400/Y4 "
    };
    const char *feature[] = {
        "unknown ",
        "color_fill ",
        "color_palette ",
        "ROP ",
        "quantize ",
        "src1_r2y_csc ",
        "dst_full_csc ",
        "FBC_mode ",
        "blend_in_YUV ",
        "BT.2020 ",
        "mosaic ",
        "OSD ",
        "early_interruption ",
    };
    const char *performance[] = {
        "unknown",
        "max 1 pixel/cycle ",
        "max 2 pixel/cycle ",
        "max 4 pixel/cycle ",
    };
    ostringstream out;
    static string info;

    rga_info_table_entry rga_info;

    memset(&rga_info, 0x0, sizeof(rga_info));
    usage = rga_get_info(&rga_info);
    if (IM_STATUS_FAILED == usage) {
        ALOGE("rga im2d: rga2 get info failed!\n");
        return "get info failed";
    }

    do {
        switch(name) {
            case RGA_VENDOR :
                out << output_name[name] << output_vendor << endl;
                break;

            case RGA_VERSION :
                out << version_name[RGA_API] << "v" << RGA_API_VERSION << endl;

                out << output_name[name];
                if (rga_info.version == IM_RGA_HW_VERSION_RGA_V_ERR) {
                    out << output_version[IM_RGA_HW_VERSION_RGA_V_ERR_INDEX];
                } else {
                    if (rga_info.version & IM_RGA_HW_VERSION_RGA_1)
                        out << output_version[IM_RGA_HW_VERSION_RGA_1_INDEX];
                    if (rga_info.version & IM_RGA_HW_VERSION_RGA_1_PLUS)
                        out << output_version[IM_RGA_HW_VERSION_RGA_1_PLUS_INDEX];
                    if (rga_info.version & IM_RGA_HW_VERSION_RGA_2)
                        out << output_version[IM_RGA_HW_VERSION_RGA_2_INDEX];
                    if (rga_info.version & IM_RGA_HW_VERSION_RGA_2_LITE0)
                        out << output_version[IM_RGA_HW_VERSION_RGA_2_LITE0_INDEX];
                    if (rga_info.version & IM_RGA_HW_VERSION_RGA_2_LITE1)
                        out << output_version[IM_RGA_HW_VERSION_RGA_2_LITE1_INDEX];
                    if (rga_info.version & IM_RGA_HW_VERSION_RGA_2_ENHANCE)
                        out << output_version[IM_RGA_HW_VERSION_RGA_2_ENHANCE_INDEX];
                    if (rga_info.version & IM_RGA_HW_VERSION_RGA_3)
                        out << output_version[IM_RGA_HW_VERSION_RGA_3_INDEX];
                }
                out << endl;
                break;

            case RGA_MAX_INPUT :
                switch (rga_info.input_resolution) {
                    case 2048 :
                        out << output_name[name] << output_resolution[1] << endl;
                        break;
                    case 4096 :
                        out << output_name[name] << output_resolution[2] << endl;
                        break;
                    case 8192 :
                        out << output_name[name] << output_resolution[3] << endl;
                        break;
                    case 8128 :
                        out << output_name[name] << output_resolution[4] << endl;
                        break;
                    default :
                        out << output_name[name] << output_resolution[IM_RGA_HW_VERSION_RGA_V_ERR_INDEX] << endl;
                        break;
                }
                break;

            case RGA_MAX_OUTPUT :
                switch(rga_info.output_resolution) {
                    case 2048 :
                        out << output_name[name] << output_resolution[1] << endl;
                        break;
                    case 4096 :
                        out << output_name[name] << output_resolution[2] << endl;
                        break;
                    case 8192 :
                        out << output_name[name] << output_resolution[3] << endl;
                        break;
                    case 8128 :
                        out << output_name[name] << output_resolution[4] << endl;
                        break;
                    default :
                        out << output_name[name] << output_resolution[IM_RGA_HW_VERSION_RGA_V_ERR_INDEX] << endl;
                        break;
                }
                break;

            case RGA_BYTE_STRIDE :
                if (rga_info.byte_stride > 0)
                    out << output_name[name] << rga_info.byte_stride << " byte" << endl;
                else
                    out << output_name[name] << "unknown" << endl;

                break;

            case RGA_SCALE_LIMIT :
                switch(rga_info.scale_limit) {
                    case 8 :
                        out << output_name[name] << output_scale_limit[1] << endl;
                        break;
                    case 16 :
                        out << output_name[name] << output_scale_limit[2] << endl;
                        break;
                    default :
                        out << output_name[name] << output_scale_limit[IM_RGA_HW_VERSION_RGA_V_ERR_INDEX] << endl;
                        break;
                }
                break;

            case RGA_INPUT_FORMAT :
                out << output_name[name];
                if(rga_info.input_format & IM_RGA_SUPPORT_FORMAT_RGB)
                    out << output_format[IM_RGA_SUPPORT_FORMAT_RGB_INDEX];
                if(rga_info.input_format & IM_RGA_SUPPORT_FORMAT_RGB_OTHER)
                    out << output_format[IM_RGA_SUPPORT_FORMAT_RGB_OTHER_INDEX];
                if(rga_info.input_format & IM_RGA_SUPPORT_FORMAT_BPP)
                    out << output_format[IM_RGA_SUPPORT_FORMAT_BPP_INDEX];
                if(rga_info.input_format & IM_RGA_SUPPORT_FORMAT_YUV_420_SEMI_PLANNER_8_BIT)
                    out << output_format[IM_RGA_SUPPORT_FORMAT_YUV_420_SEMI_PLANNER_8_BIT_INDEX];
                if(rga_info.input_format & IM_RGA_SUPPORT_FORMAT_YUV_420_SEMI_PLANNER_10_BIT)
                    out << output_format[IM_RGA_SUPPORT_FORMAT_YUV_420_SEMI_PLANNER_10_BIT_INDEX];
                if(rga_info.input_format & IM_RGA_SUPPORT_FORMAT_YUV_420_PLANNER_8_BIT)
                    out << output_format[IM_RGA_SUPPORT_FORMAT_YUV_420_PLANNER_8_BIT_INDEX];
                if(rga_info.input_format & IM_RGA_SUPPORT_FORMAT_YUV_420_PLANNER_10_BIT)
                    out << output_format[IM_RGA_SUPPORT_FORMAT_YUV_420_PLANNER_10_BIT_INDEX];
                if(rga_info.input_format & IM_RGA_SUPPORT_FORMAT_YUV_422_SEMI_PLANNER_8_BIT)
                    out << output_format[IM_RGA_SUPPORT_FORMAT_YUV_422_SEMI_PLANNER_8_BIT_INDEX];
                if(rga_info.input_format & IM_RGA_SUPPORT_FORMAT_YUV_422_SEMI_PLANNER_10_BIT)
                    out << output_format[IM_RGA_SUPPORT_FORMAT_YUV_422_SEMI_PLANNER_10_BIT_INDEX];
                if(rga_info.input_format & IM_RGA_SUPPORT_FORMAT_YUV_422_PLANNER_8_BIT)
                    out << output_format[IM_RGA_SUPPORT_FORMAT_YUV_422_PLANNER_8_BIT_INDEX];
                if(rga_info.input_format & IM_RGA_SUPPORT_FORMAT_YUV_422_PLANNER_10_BIT)
                    out << output_format[IM_RGA_SUPPORT_FORMAT_YUV_422_PLANNER_10_BIT_INDEX];
                if(rga_info.input_format & IM_RGA_SUPPORT_FORMAT_YUYV_420)
                    out << output_format[IM_RGA_SUPPORT_FORMAT_YUYV_420_INDEX];
                if(rga_info.input_format & IM_RGA_SUPPORT_FORMAT_YUYV_422)
                    out << output_format[IM_RGA_SUPPORT_FORMAT_YUYV_422_INDEX];
                if(rga_info.input_format & IM_RGA_SUPPORT_FORMAT_YUV_400)
                    out << output_format[IM_RGA_SUPPORT_FORMAT_YUV_400_INDEX];
                if(!(rga_info.input_format & IM_RGA_SUPPORT_FORMAT_MASK))
                    out << output_format[IM_RGA_SUPPORT_FORMAT_ERROR_INDEX];
                out << endl;
                break;

            case RGA_OUTPUT_FORMAT :
                out << output_name[name];
                if(rga_info.output_format & IM_RGA_SUPPORT_FORMAT_RGB)
                    out << output_format[IM_RGA_SUPPORT_FORMAT_RGB_INDEX];
                if(rga_info.output_format & IM_RGA_SUPPORT_FORMAT_RGB_OTHER)
                    out << output_format[IM_RGA_SUPPORT_FORMAT_RGB_OTHER_INDEX];
                if(rga_info.output_format & IM_RGA_SUPPORT_FORMAT_BPP)
                    out << output_format[IM_RGA_SUPPORT_FORMAT_BPP_INDEX];
                if(rga_info.output_format & IM_RGA_SUPPORT_FORMAT_YUV_420_SEMI_PLANNER_8_BIT)
                    out << output_format[IM_RGA_SUPPORT_FORMAT_YUV_420_SEMI_PLANNER_8_BIT_INDEX];
                if(rga_info.output_format & IM_RGA_SUPPORT_FORMAT_YUV_420_SEMI_PLANNER_10_BIT)
                    out << output_format[IM_RGA_SUPPORT_FORMAT_YUV_420_SEMI_PLANNER_10_BIT_INDEX];
                if(rga_info.output_format & IM_RGA_SUPPORT_FORMAT_YUV_420_PLANNER_8_BIT)
                    out << output_format[IM_RGA_SUPPORT_FORMAT_YUV_420_PLANNER_8_BIT_INDEX];
                if(rga_info.output_format & IM_RGA_SUPPORT_FORMAT_YUV_420_PLANNER_10_BIT)
                    out << output_format[IM_RGA_SUPPORT_FORMAT_YUV_420_PLANNER_10_BIT_INDEX];
                if(rga_info.output_format & IM_RGA_SUPPORT_FORMAT_YUV_422_SEMI_PLANNER_8_BIT)
                    out << output_format[IM_RGA_SUPPORT_FORMAT_YUV_422_SEMI_PLANNER_8_BIT_INDEX];
                if(rga_info.output_format & IM_RGA_SUPPORT_FORMAT_YUV_422_SEMI_PLANNER_10_BIT)
                    out << output_format[IM_RGA_SUPPORT_FORMAT_YUV_422_SEMI_PLANNER_10_BIT_INDEX];
                if(rga_info.output_format & IM_RGA_SUPPORT_FORMAT_YUV_422_PLANNER_8_BIT)
                    out << output_format[IM_RGA_SUPPORT_FORMAT_YUV_422_PLANNER_8_BIT_INDEX];
                if(rga_info.output_format & IM_RGA_SUPPORT_FORMAT_YUV_422_PLANNER_10_BIT)
                    out << output_format[IM_RGA_SUPPORT_FORMAT_YUV_422_PLANNER_10_BIT_INDEX];
                if(rga_info.output_format & IM_RGA_SUPPORT_FORMAT_YUYV_420)
                    out << output_format[IM_RGA_SUPPORT_FORMAT_YUYV_420_INDEX];
                if(rga_info.output_format & IM_RGA_SUPPORT_FORMAT_YUYV_422)
                    out << output_format[IM_RGA_SUPPORT_FORMAT_YUYV_422_INDEX];
                if(rga_info.output_format & IM_RGA_SUPPORT_FORMAT_YUV_400)
                    out << output_format[IM_RGA_SUPPORT_FORMAT_YUV_400_INDEX];
                if(!(rga_info.output_format & IM_RGA_SUPPORT_FORMAT_MASK))
                    out << output_format[IM_RGA_SUPPORT_FORMAT_ERROR_INDEX];
                out << endl;
                break;

            case RGA_FEATURE :
                out << output_name[name];
                if(rga_info.feature & IM_RGA_SUPPORT_FEATURE_COLOR_FILL)
                    out << feature[IM_RGA_SUPPORT_FEATURE_COLOR_FILL_INDEX];
                if(rga_info.feature & IM_RGA_SUPPORT_FEATURE_COLOR_PALETTE)
                    out << feature[IM_RGA_SUPPORT_FEATURE_COLOR_PALETTE_INDEX];
                if(rga_info.feature & IM_RGA_SUPPORT_FEATURE_ROP)
                    out << feature[IM_RGA_SUPPORT_FEATURE_ROP_INDEX];
                if(rga_info.feature & IM_RGA_SUPPORT_FEATURE_QUANTIZE)
                    out << feature[IM_RGA_SUPPORT_FEATURE_QUANTIZE_INDEX];
                if(rga_info.feature & IM_RGA_SUPPORT_FEATURE_SRC1_R2Y_CSC)
                    out << feature[IM_RGA_SUPPORT_FEATURE_SRC1_R2Y_CSC_INDEX];
                if(rga_info.feature & IM_RGA_SUPPORT_FEATURE_DST_FULL_CSC)
                    out << feature[IM_RGA_SUPPORT_FEATURE_DST_FULL_CSC_INDEX];
                if(rga_info.feature & IM_RGA_SUPPORT_FEATURE_FBC)
                    out << feature[IM_RGA_SUPPORT_FEATURE_FBC_INDEX];
                if(rga_info.feature & IM_RGA_SUPPORT_FEATURE_BLEND_YUV)
                    out << feature[IM_RGA_SUPPORT_FEATURE_BLEND_YUV_INDEX];
                if(rga_info.feature & IM_RGA_SUPPORT_FEATURE_BT2020)
                    out << feature[IM_RGA_SUPPORT_FEATURE_BT2020_INDEX];
                if(rga_info.feature & IM_RGA_SUPPORT_FEATURE_MOSAIC)
                    out << feature[IM_RGA_SUPPORT_FEATURE_MOSAIC_INDEX];
                if(rga_info.feature & IM_RGA_SUPPORT_FEATURE_OSD)
                    out << feature[IM_RGA_SUPPORT_FEATURE_OSD_INDEX];
                if(rga_info.feature & IM_RGA_SUPPORT_FEATURE_PRE_INTR)
                    out << feature[IM_RGA_SUPPORT_FEATURE_PRE_INTR_INDEX];
                out << endl;
                break;

            case RGA_EXPECTED :
                switch(rga_info.performance) {
                    case 1 :
                        out << output_name[name] << performance[1] << endl;
                        break;
                    case 2 :
                        out << output_name[name] << performance[2] << endl;
                        break;
                    case 4 :
                        out << output_name[name] << performance[3] << endl;
                        break;
                    default :
                        out << output_name[name] << performance[IM_RGA_HW_VERSION_RGA_V_ERR_INDEX] << endl;
                        break;
                }
                break;

            case RGA_ALL :
                if (!all_output) {
                    all_output = 1;
                    name = 0;
                } else
                    all_output_prepared = 1;
                break;

            default:
                return "Invalid instruction";
        }

        info = out.str();

        if (all_output_prepared)
            break;
        else if (all_output && strcmp(info.c_str(),"0")>0)
            name++;

    } while(all_output);

    temp = info.c_str();

    return temp;
}

IM_API IM_STATUS imcheck_t(const rga_buffer_t src, const rga_buffer_t dst, const rga_buffer_t pat,
                           const im_rect src_rect, const im_rect dst_rect, const im_rect pat_rect, int mode_usage) {
    bool pat_enable = 0;
    IM_STATUS ret = IM_STATUS_NOERROR;
    rga_info_table_entry rga_info;

    memset(&rga_info, 0x0, sizeof(rga_info));
    ret = rga_get_info(&rga_info);
    if (IM_STATUS_FAILED == ret) {
        ALOGE("rga im2d: rga2 get info failed!\n");
        return IM_STATUS_FAILED;
    }

    /* check driver version. */
    ret = rga_check_driver();
    if (ret == IM_STATUS_ERROR_VERSION)
        return ret;

    if (mode_usage & IM_ALPHA_BLEND_MASK) {
        if (rga_is_buffer_valid(pat))
            pat_enable = 1;
    }

    /**************** feature judgment ****************/
    ret = rga_check_feature(src, pat, dst, pat_enable, mode_usage, rga_info.feature);
    if (ret != IM_STATUS_NOERROR)
        return ret;

    /**************** info judgment ****************/
    if (~mode_usage & IM_COLOR_FILL) {
        ret = rga_check_info("src", src, src_rect, rga_info.input_resolution);
        if (ret != IM_STATUS_NOERROR)
            return ret;
        ret = rga_check_format("src", src, src_rect, rga_info.input_format, mode_usage);
        if (ret != IM_STATUS_NOERROR)
            return ret;
        ret = rga_check_align("src", src, rga_info.byte_stride);
        if (ret != IM_STATUS_NOERROR)
            return ret;
    }
    if (pat_enable) {
        /* RGA1 cannot support src1. */
        if (rga_info.version & (IM_RGA_HW_VERSION_RGA_1 | IM_RGA_HW_VERSION_RGA_1_PLUS)) {
            imSetErrorMsg("RGA1/RGA1_PLUS cannot support src1.");
            return IM_STATUS_NOT_SUPPORTED;
        }


        ret = rga_check_info("pat", pat, pat_rect, rga_info.input_resolution);
        if (ret != IM_STATUS_NOERROR)
            return ret;
        ret = rga_check_format("pat", pat, pat_rect, rga_info.input_format, mode_usage);
        if (ret != IM_STATUS_NOERROR)
            return ret;
        ret = rga_check_align("pat", pat, rga_info.byte_stride);
        if (ret != IM_STATUS_NOERROR)
            return ret;
    }
    ret = rga_check_info("dst", dst, dst_rect, rga_info.output_resolution);
    if (ret != IM_STATUS_NOERROR)
        return ret;
    ret = rga_check_format("dst", dst, dst_rect, rga_info.output_format, mode_usage);
    if (ret != IM_STATUS_NOERROR)
        return ret;
    ret = rga_check_align("dst", dst, rga_info.byte_stride);
    if (ret != IM_STATUS_NOERROR)
        return ret;

    if ((~mode_usage & IM_COLOR_FILL)) {
        ret = rga_check_limit(src, dst, rga_info.scale_limit, mode_usage);
        if (ret != IM_STATUS_NOERROR)
            return ret;
    }

    if (mode_usage & IM_ALPHA_BLEND_MASK) {
        ret = rga_check_blend(src, pat, dst, pat_enable, mode_usage);
        if (ret != IM_STATUS_NOERROR)
            return ret;
    }

    ret = rga_check_rotate(mode_usage, rga_info);
    if (ret != IM_STATUS_NOERROR)
        return ret;

    return IM_STATUS_NOERROR;
}

IM_API IM_STATUS imresize(const rga_buffer_t src, rga_buffer_t dst, double fx, double fy, int interpolation, int sync, int *release_fence_fd) {
    int usage = 0;
    int width = 0, height = 0;
    IM_STATUS ret = IM_STATUS_NOERROR;

    im_opt_t opt;

    rga_buffer_t pat;

    im_rect srect;
    im_rect drect;
    im_rect prect;

    empty_structure(NULL, NULL, &pat, &srect, &drect, &prect, &opt);

    if (fx > 0 || fy > 0) {
        if (fx == 0) fx = 1;
        if (fy == 0) fy = 1;

        dst.width = (int)(src.width * fx);
        dst.height = (int)(src.height * fy);

        if(NormalRgaIsYuvFormat(RkRgaGetRgaFormat(src.format))) {
            width = dst.width;
            height = dst.height;
            dst.width = DOWN_ALIGN(dst.width, 2);
            dst.height = DOWN_ALIGN(dst.height, 2);

            ret = imcheck(src, dst, srect, drect, usage);
            if (ret != IM_STATUS_NOERROR) {
                ALOGE("imresize error, factor[fx,fy]=[%lf,%lf], ALIGN[dw,dh]=[%d,%d][%d,%d]", fx, fy, width, height, dst.width, dst.height);
                return ret;
            }
        }
    }
    UNUSED(interpolation);

    if (sync == 0)
        usage |= IM_ASYNC;
    else if (sync == 1)
        usage |= IM_SYNC;

    ret = improcess(src, dst, pat, srect, drect, prect, -1, release_fence_fd, &opt, usage);

    return ret;
}

IM_API IM_STATUS imcrop(const rga_buffer_t src, rga_buffer_t dst, im_rect rect, int sync, int *release_fence_fd) {
    int usage = 0;
    IM_STATUS ret = IM_STATUS_NOERROR;

    im_opt_t opt;

    rga_buffer_t pat;

    im_rect drect;
    im_rect prect;

    empty_structure(NULL, NULL, &pat, NULL, &drect, &prect, &opt);

    drect.width = rect.width;
    drect.height = rect.height;

    if (sync == 0)
        usage |= IM_ASYNC;
    else if (sync == 1)
        usage |= IM_SYNC;

    ret = improcess(src, dst, pat, rect, drect, prect, -1, release_fence_fd, &opt, usage);

    return ret;
}

IM_API IM_STATUS imrotate(const rga_buffer_t src, rga_buffer_t dst, int rotation, int sync, int *release_fence_fd) {
    int usage = 0;
    IM_STATUS ret = IM_STATUS_NOERROR;

    im_opt_t opt;

    rga_buffer_t pat;

    im_rect srect;
    im_rect drect;
    im_rect prect;

    empty_structure(NULL, NULL, &pat, &srect, &drect, &prect, &opt);

    usage |= rotation;

    if (sync == 0)
        usage |= IM_ASYNC;
    else if (sync == 1)
        usage |= IM_SYNC;

    ret = improcess(src, dst, pat, srect, drect, prect, -1, release_fence_fd, &opt, usage);

    return ret;
}

IM_API IM_STATUS imflip(const rga_buffer_t src, rga_buffer_t dst, int mode, int sync, int *release_fence_fd) {
    int usage = 0;
    IM_STATUS ret = IM_STATUS_NOERROR;

    im_opt_t opt;

    rga_buffer_t pat;

    im_rect srect;
    im_rect drect;
    im_rect prect;

    empty_structure(NULL, NULL, &pat, &srect, &drect, &prect, &opt);

    usage |= mode;

    if (sync == 0)
        usage |= IM_ASYNC;
    else if (sync == 1)
        usage |= IM_SYNC;

    ret = improcess(src, dst, pat, srect, drect, prect, -1, release_fence_fd, &opt, usage);

    return ret;
}

IM_API IM_STATUS imfill(rga_buffer_t dst, im_rect rect, int color, int sync, int *release_fence_fd) {
    int usage = 0;
    IM_STATUS ret = IM_STATUS_NOERROR;

    im_opt_t opt;

    rga_buffer_t pat;
    rga_buffer_t src;

    im_rect srect;
    im_rect prect;

    empty_structure(&src, NULL, &pat, &srect, NULL, &prect, &opt);

    memset(&src, 0, sizeof(rga_buffer_t));

    usage |= IM_COLOR_FILL;

    opt.color = color;

    if (sync == 0)
        usage |= IM_ASYNC;
    else if (sync == 1)
        usage |= IM_SYNC;

    ret = improcess(src, dst, pat, srect, rect, prect, -1, release_fence_fd, &opt, usage);

    return ret;
}

IM_API IM_STATUS impalette(rga_buffer_t src, rga_buffer_t dst, rga_buffer_t lut, int sync, int *release_fence_fd) {
    int usage = 0;
    IM_STATUS ret = IM_STATUS_NOERROR;
    im_rect srect;
    im_rect drect;
    im_rect prect;

    im_opt_t opt;

    empty_structure(NULL, NULL, NULL, &srect, &drect, &prect, &opt);

    /*Don't know if it supports zooming.*/
    if ((src.width != dst.width) || (src.height != dst.height))
        return IM_STATUS_INVALID_PARAM;

    usage |= IM_COLOR_PALETTE;

    if (sync == 0)
        usage |= IM_ASYNC;
    else if (sync == 1)
        usage |= IM_SYNC;

    ret = improcess(src, dst, lut, srect, drect, prect, -1, release_fence_fd, &opt, usage);

    return ret;
}

IM_API IM_STATUS imtranslate(const rga_buffer_t src, rga_buffer_t dst, int x, int y, int sync, int *release_fence_fd) {
    int usage = 0;
    IM_STATUS ret = IM_STATUS_NOERROR;

    im_opt_t opt;

    rga_buffer_t pat;

    im_rect srect;
    im_rect drect;
    im_rect prect;

    empty_structure(NULL, NULL, &pat, &srect, &drect, &prect, &opt);

    if ((src.width != dst.width) || (src.height != dst.height))
        return IM_STATUS_INVALID_PARAM;

    if (sync == 0)
        usage |= IM_ASYNC;
    else if (sync == 1)
        usage |= IM_SYNC;

    srect.width = src.width - x;
    srect.height = src.height - y;
    drect.x = x;
    drect.y = y;
    drect.width = src.width - x;
    drect.height = src.height - y;

    ret = improcess(src, dst, pat, srect, drect, prect, -1, release_fence_fd, &opt, usage);

    return ret;
}

IM_API IM_STATUS imcopy(const rga_buffer_t src, rga_buffer_t dst, int sync, int *release_fence_fd) {
    int usage = 0;
    IM_STATUS ret = IM_STATUS_NOERROR;

    im_opt_t opt;

    rga_buffer_t pat;

    im_rect srect;
    im_rect drect;
    im_rect prect;

    empty_structure(NULL, NULL, &pat, &srect, &drect, &prect, &opt);

    if ((src.width != dst.width) || (src.height != dst.height)) {
        imSetErrorMsg("imcopy cannot support scale, src[w,h] = [%d, %d], dst[w,h] = [%d, %d]",
                      src.width, src.height, dst.width, dst.height);
        return IM_STATUS_INVALID_PARAM;
    }

    if (sync == 0)
        usage |= IM_ASYNC;
    else if (sync == 1)
        usage |= IM_SYNC;

    ret = improcess(src, dst, pat, srect, drect, prect, -1, release_fence_fd, &opt, usage);

    return ret;
}

IM_API IM_STATUS imcolorkey(const rga_buffer_t src, rga_buffer_t dst, im_colorkey_range range, int mode, int sync, int *release_fence_fd) {
    int usage = 0;
    IM_STATUS ret = IM_STATUS_NOERROR;

    im_opt_t opt;

    rga_buffer_t pat;

    im_rect srect;
    im_rect drect;
    im_rect prect;

    empty_structure(NULL, NULL, &pat, &srect, &drect, &prect, &opt);

    usage |= mode;

    opt.colorkey_range = range;

    if (sync == 0)
        usage |= IM_ASYNC;
    else if (sync == 1)
        usage |= IM_SYNC;

    ret = improcess(src, dst, pat, srect, drect, prect, -1, release_fence_fd, &opt, usage);

    return ret;
}

IM_API IM_STATUS imcomposite(const rga_buffer_t srcA, const rga_buffer_t srcB, rga_buffer_t dst, int mode, int sync, int *release_fence_fd) {
    int usage = 0;
    IM_STATUS ret = IM_STATUS_NOERROR;

    im_opt_t opt;

    im_rect srect;
    im_rect drect;
    im_rect prect;

    empty_structure(NULL, NULL, NULL, &srect, &drect, &prect, &opt);

    usage |= mode;

    if (sync == 0)
        usage |= IM_ASYNC;
    else if (sync == 1)
        usage |= IM_SYNC;

    ret = improcess(srcA, dst, srcB, srect, drect, prect, -1, release_fence_fd, &opt, usage);

    return ret;
}

IM_API IM_STATUS imblend(const rga_buffer_t src, rga_buffer_t dst, int mode, int sync, int *release_fence_fd) {
    rga_buffer_t pat;

    memset(&pat, 0x0, sizeof(pat));

    return imcomposite(src, pat, dst, mode, sync, release_fence_fd);
}

IM_API IM_STATUS imosd(const rga_buffer_t osd,const rga_buffer_t dst,
                       const im_rect osd_rect, im_osd_t *osd_info,
                       int sync, int *release_fence_fd) {
    int usage = 0;
    im_opt_t opt;
    im_rect tmp_rect;

    memset(&opt, 0x0, sizeof(opt));
    memset(&tmp_rect, 0x0, sizeof(tmp_rect));

    opt.version = RGA_SET_CURRENT_API_VERISON;
    memcpy(&opt.osd_config, osd_info, sizeof(im_osd_t));

    usage |= IM_ALPHA_BLEND_DST_OVER | IM_OSD;

    if (sync == 0)
        usage |= IM_ASYNC;
    else if (sync == 1)
        usage |= IM_SYNC;

    return improcess(dst, dst, osd, osd_rect, osd_rect, tmp_rect, -1, release_fence_fd, &opt, usage);
}

IM_API IM_STATUS imcvtcolor(rga_buffer_t src, rga_buffer_t dst, int sfmt, int dfmt, int mode, int sync, int *release_fence_fd) {
    int usage = 0;
    IM_STATUS ret = IM_STATUS_NOERROR;

    im_opt_t opt;

    rga_buffer_t pat;

    im_rect srect;
    im_rect drect;
    im_rect prect;

    empty_structure(NULL, NULL, &pat, &srect, &drect, &prect, &opt);

    src.format = sfmt;
    dst.format = dfmt;

    dst.color_space_mode = mode;

    if (sync == 0)
        usage |= IM_ASYNC;
    else if (sync == 1)
        usage |= IM_SYNC;

    ret = improcess(src, dst, pat, srect, drect, prect, -1, release_fence_fd, &opt, usage);

    return ret;
}

IM_API IM_STATUS imquantize(const rga_buffer_t src, rga_buffer_t dst, im_nn_t nn_info, int sync, int *release_fence_fd) {
    int usage = 0;
    IM_STATUS ret = IM_STATUS_NOERROR;

    im_opt_t opt;

    rga_buffer_t pat;

    im_rect srect;
    im_rect drect;
    im_rect prect;

    empty_structure(NULL, NULL, &pat, &srect, &drect, &prect, &opt);

    usage |= IM_NN_QUANTIZE;

    opt.nn = nn_info;

    if (sync == 0)
        usage |= IM_ASYNC;
    else if (sync == 1)
        usage |= IM_SYNC;

    ret = improcess(src, dst, pat, srect, drect, prect, -1, release_fence_fd, &opt, usage);

    return ret;
}

IM_API IM_STATUS imrop(const rga_buffer_t src, rga_buffer_t dst, int rop_code, int sync, int *release_fence_fd) {
    int usage = 0;
    IM_STATUS ret = IM_STATUS_NOERROR;

    im_opt_t opt;

    rga_buffer_t pat;

    im_rect srect;
    im_rect drect;
    im_rect prect;

    empty_structure(NULL, NULL, &pat, &srect, &drect, &prect, &opt);

    usage |= IM_ROP;

    opt.rop_code = rop_code;

    if (sync == 0)
        usage |= IM_ASYNC;
    else if (sync == 1)
        usage |= IM_SYNC;

    ret = improcess(src, dst, pat, srect, drect, prect, -1, release_fence_fd, &opt, usage);

    return ret;
}

IM_API IM_STATUS immosaic(const rga_buffer_t image, im_rect rect, int mosaic_mode, int sync, int *release_fence_fd) {
    IM_STATUS ret = IM_STATUS_NOERROR;
    int usage = 0;
    im_opt_t opt;
    rga_buffer_t tmp_image;
    im_rect tmp_rect;

    memset(&opt, 0x0, sizeof(opt));
    memset(&tmp_image, 0x0, sizeof(tmp_image));
    memset(&tmp_rect, 0x0, sizeof(tmp_rect));

    usage |= IM_MOSAIC;

    opt.version = RGA_SET_CURRENT_API_VERISON;
    opt.mosaic_mode = mosaic_mode;

    if (sync == 0)
        usage |= IM_ASYNC;
    else if (sync == 1)
        usage |= IM_SYNC;

    return improcess(image, image, tmp_image, rect, rect, tmp_rect, -1, release_fence_fd, &opt, usage);
}

IM_API IM_STATUS improcess(rga_buffer_t src, rga_buffer_t dst, rga_buffer_t pat,
                        im_rect srect, im_rect drect, im_rect prect, int usage) {
    IM_STATUS ret = IM_STATUS_NOERROR;
    int release_fence_fd;
    im_opt_t opt;

    memset(&opt, 0, sizeof(opt));

    ret = improcess(src, dst, pat, srect, drect, prect, -1, &release_fence_fd, &opt, usage);

    return ret;
}

IM_API IM_STATUS improcess(rga_buffer_t src, rga_buffer_t dst, rga_buffer_t pat,
                           im_rect srect, im_rect drect, im_rect prect,
                           int acquire_fence_fd, int *release_fence_fd, im_opt_t *opt_ptr, int usage) {
    return improcess(src, dst, pat, srect, drect, prect, acquire_fence_fd, release_fence_fd, opt_ptr, usage, 0);
}

IM_API IM_STATUS improcess(rga_buffer_t src, rga_buffer_t dst, rga_buffer_t pat,
                           im_rect srect, im_rect drect, im_rect prect,
                           int acquire_fence_fd, int *release_fence_fd,
                           im_opt_t *opt_ptr, int usage, im_ctx_id_t ctx_id) {
    int ret;
    rga_info_t srcinfo;
    rga_info_t dstinfo;
    rga_info_t patinfo;

    im_opt_t opt;

    if (rga_get_opt(&opt, opt_ptr) == IM_STATUS_FAILED)
        memset(&opt, 0x0, sizeof(opt));

    src.format = RkRgaCompatibleFormat(src.format);
    dst.format = RkRgaCompatibleFormat(dst.format);
    pat.format = RkRgaCompatibleFormat(pat.format);

    memset(&srcinfo, 0, sizeof(rga_info_t));
    memset(&dstinfo, 0, sizeof(rga_info_t));
    memset(&patinfo, 0, sizeof(rga_info_t));

    if (usage & IM_COLOR_FILL)
        ret = rga_set_buffer_info(dst, &dstinfo);
    else
        ret = rga_set_buffer_info(src, dst, &srcinfo, &dstinfo);

    if (ret <= 0)
        return (IM_STATUS)ret;

    rga_apply_rect(&src, &srect);
    rga_apply_rect(&dst, &drect);

    rga_set_rect(&srcinfo.rect, srect.x, srect.y, src.width, src.height, src.wstride, src.hstride, src.format);
    rga_set_rect(&dstinfo.rect, drect.x, drect.y, dst.width, dst.height, dst.wstride, dst.hstride, dst.format);

    if (((usage & IM_COLOR_PALETTE) || (usage & IM_ALPHA_BLEND_MASK)) &&
        rga_is_buffer_valid(pat)) {

        ret = rga_set_buffer_info(pat, &patinfo);
        if (ret <= 0)
            return (IM_STATUS)ret;

        rga_apply_rect(&pat, &prect);

        rga_set_rect(&patinfo.rect, prect.x, prect.y, pat.width, pat.height, pat.wstride, pat.hstride, pat.format);
    }

    if ((usage & IM_ALPHA_BLEND_MASK) && rga_is_buffer_valid(pat)) /* A+B->C */
        ret = imcheck_composite(src, dst, pat, srect, drect, prect, usage);
    else
        ret = imcheck(src, dst, srect, drect, usage);
    if(ret != IM_STATUS_NOERROR)
        return (IM_STATUS)ret;

    /* Transform */
    if (usage & IM_HAL_TRANSFORM_MASK) {
        switch (usage & (IM_HAL_TRANSFORM_ROT_90 + IM_HAL_TRANSFORM_ROT_180 + IM_HAL_TRANSFORM_ROT_270)) {
            case IM_HAL_TRANSFORM_ROT_90:
                srcinfo.rotation = HAL_TRANSFORM_ROT_90;
                break;
            case IM_HAL_TRANSFORM_ROT_180:
                srcinfo.rotation = HAL_TRANSFORM_ROT_180;
                break;
            case IM_HAL_TRANSFORM_ROT_270:
                srcinfo.rotation = HAL_TRANSFORM_ROT_270;
                break;
        }

        switch (usage & (IM_HAL_TRANSFORM_FLIP_V + IM_HAL_TRANSFORM_FLIP_H + IM_HAL_TRANSFORM_FLIP_H_V)) {
            case IM_HAL_TRANSFORM_FLIP_V:
                srcinfo.rotation |= srcinfo.rotation ?
                                    HAL_TRANSFORM_FLIP_V << 4 :
                                    HAL_TRANSFORM_FLIP_V;
                break;
            case IM_HAL_TRANSFORM_FLIP_H:
                srcinfo.rotation |= srcinfo.rotation ?
                                    HAL_TRANSFORM_FLIP_H << 4 :
                                    HAL_TRANSFORM_FLIP_H;
                break;
            case IM_HAL_TRANSFORM_FLIP_H_V:
                srcinfo.rotation |= srcinfo.rotation ?
                                    HAL_TRANSFORM_FLIP_H_V << 4 :
                                    HAL_TRANSFORM_FLIP_H_V;
                break;
        }

        if(srcinfo.rotation ==0)
            ALOGE("rga_im2d: Could not find rotate/flip usage : 0x%x \n", usage);
    }

    /* Blend */
    if (usage & IM_ALPHA_BLEND_MASK) {
        switch(usage & IM_ALPHA_BLEND_MASK) {
            case IM_ALPHA_BLEND_SRC:
                srcinfo.blend = 0x1;
                break;
            case IM_ALPHA_BLEND_DST:
                srcinfo.blend = 0x2;
                break;
            case IM_ALPHA_BLEND_SRC_OVER:
                srcinfo.blend = (usage & IM_ALPHA_BLEND_PRE_MUL) ? 0x405 : 0x105;
                break;
            case IM_ALPHA_BLEND_SRC_IN:
                break;
            case IM_ALPHA_BLEND_DST_IN:
                break;
            case IM_ALPHA_BLEND_SRC_OUT:
                break;
            case IM_ALPHA_BLEND_DST_OVER:
                srcinfo.blend = (usage & IM_ALPHA_BLEND_PRE_MUL) ? 0x504 : 0x501;
                break;
            case IM_ALPHA_BLEND_SRC_ATOP:
                break;
            case IM_ALPHA_BLEND_DST_OUT:
                break;
            case IM_ALPHA_BLEND_XOR:
                break;
        }

        if(srcinfo.blend == 0)
            ALOGE("rga_im2d: Could not find blend usage : 0x%x \n", usage);

        /* set global alpha */
        if (src.global_alpha > 0)
            srcinfo.blend ^= src.global_alpha << 16;
        else {
            srcinfo.blend ^= 0xFF << 16;
        }
    }

    /* color key */
    if (usage & IM_ALPHA_COLORKEY_MASK) {
        srcinfo.blend = 0xff0105;

        srcinfo.colorkey_en = 1;
        srcinfo.colorkey_min = opt.colorkey_range.min;
        srcinfo.colorkey_max = opt.colorkey_range.max;
        switch (usage & IM_ALPHA_COLORKEY_MASK) {
            case IM_ALPHA_COLORKEY_NORMAL:
                srcinfo.colorkey_mode = 0;
                break;
            case IM_ALPHA_COLORKEY_INVERTED:
                srcinfo.colorkey_mode = 1;
                break;
        }
    }

    /* OSD */
    if (usage & IM_OSD) {
        srcinfo.osd_info.enable = true;

        srcinfo.osd_info.mode_ctrl.mode = opt.osd_config.osd_mode;

        srcinfo.osd_info.mode_ctrl.width_mode = opt.osd_config.block_parm.width_mode;
        if (opt.osd_config.block_parm.width_mode == IM_OSD_BLOCK_MODE_NORMAL)
            srcinfo.osd_info.mode_ctrl.block_fix_width = opt.osd_config.block_parm.width;
        else if (opt.osd_config.block_parm.width_mode == IM_OSD_BLOCK_MODE_DIFFERENT)
            srcinfo.osd_info.mode_ctrl.unfix_index = opt.osd_config.block_parm.width_index;
        srcinfo.osd_info.mode_ctrl.block_num = opt.osd_config.block_parm.block_count;
        srcinfo.osd_info.mode_ctrl.default_color_sel = opt.osd_config.block_parm.background_config;
        srcinfo.osd_info.mode_ctrl.direction_mode = opt.osd_config.block_parm.direction;
        srcinfo.osd_info.mode_ctrl.color_mode = opt.osd_config.block_parm.color_mode;

        if (pat.format == RK_FORMAT_RGBA2BPP) {
            srcinfo.osd_info.bpp2_info.ac_swap = opt.osd_config.bpp2_info.ac_swap;
            srcinfo.osd_info.bpp2_info.endian_swap = opt.osd_config.bpp2_info.endian_swap;
            srcinfo.osd_info.bpp2_info.color0.value = opt.osd_config.bpp2_info.color0.value;
            srcinfo.osd_info.bpp2_info.color1.value = opt.osd_config.bpp2_info.color1.value;
        } else {
            srcinfo.osd_info.bpp2_info.color0.value = opt.osd_config.block_parm.normal_color.value;
            srcinfo.osd_info.bpp2_info.color1.value = opt.osd_config.block_parm.invert_color.value;
        }

        switch (opt.osd_config.invert_config.invert_channel) {
            case IM_OSD_INVERT_CHANNEL_NONE:
                srcinfo.osd_info.mode_ctrl.invert_enable = (0x1 << 1) | (0x1 << 2);
                break;
            case IM_OSD_INVERT_CHANNEL_Y_G:
                srcinfo.osd_info.mode_ctrl.invert_enable = 0x1 << 2;
                break;
            case IM_OSD_INVERT_CHANNEL_C_RB:
                srcinfo.osd_info.mode_ctrl.invert_enable = 0x1 << 1;
                break;
            case IM_OSD_INVERT_CHANNEL_ALPHA:
                srcinfo.osd_info.mode_ctrl.invert_enable = (0x1 << 0) | (0x1 << 1) | (0x1 << 2);
                break;
            case IM_OSD_INVERT_CHANNEL_COLOR:
                srcinfo.osd_info.mode_ctrl.invert_enable = 0;
                break;
            case IM_OSD_INVERT_CHANNEL_BOTH:
                srcinfo.osd_info.mode_ctrl.invert_enable = 0x1 << 0;
        }
        srcinfo.osd_info.mode_ctrl.invert_flags_mode = opt.osd_config.invert_config.flags_mode;
        srcinfo.osd_info.mode_ctrl.flags_index = opt.osd_config.invert_config.flags_index;

        srcinfo.osd_info.last_flags = opt.osd_config.invert_config.invert_flags;
        srcinfo.osd_info.cur_flags = opt.osd_config.invert_config.current_flags;

        srcinfo.osd_info.mode_ctrl.invert_mode = opt.osd_config.invert_config.invert_mode;
        if (opt.osd_config.invert_config.invert_mode == IM_OSD_INVERT_USE_FACTOR) {
            srcinfo.osd_info.cal_factor.alpha_max = opt.osd_config.invert_config.factor.alpha_max;
            srcinfo.osd_info.cal_factor.alpha_min = opt.osd_config.invert_config.factor.alpha_min;
            srcinfo.osd_info.cal_factor.crb_max = opt.osd_config.invert_config.factor.crb_max;
            srcinfo.osd_info.cal_factor.crb_min = opt.osd_config.invert_config.factor.crb_min;
            srcinfo.osd_info.cal_factor.yg_max = opt.osd_config.invert_config.factor.yg_max;
            srcinfo.osd_info.cal_factor.yg_min = opt.osd_config.invert_config.factor.yg_min;
        }
        srcinfo.osd_info.mode_ctrl.invert_thresh = opt.osd_config.invert_config.threash;
    }

    /* set NN quantize */
    if (usage & IM_NN_QUANTIZE) {
        dstinfo.nn.nn_flag = 1;
        dstinfo.nn.scale_r  = opt.nn.scale_r;
        dstinfo.nn.scale_g  = opt.nn.scale_g;
        dstinfo.nn.scale_b  = opt.nn.scale_b;
        dstinfo.nn.offset_r = opt.nn.offset_r;
        dstinfo.nn.offset_g = opt.nn.offset_g;
        dstinfo.nn.offset_b = opt.nn.offset_b;
    }

    /* set ROP */
    if (usage & IM_ROP) {
        srcinfo.rop_code = opt.rop_code;
    }

    /* set mosaic */
    if (usage & IM_MOSAIC) {
        srcinfo.mosaic_info.enable = true;
        srcinfo.mosaic_info.mode = opt.mosaic_mode;
    }

    /* set intr config */
    if (usage & IM_PRE_INTR) {
        srcinfo.pre_intr.enable = true;

        srcinfo.pre_intr.read_intr_en = opt.intr_config.flags & IM_INTR_READ_INTR ? true : false;
        if (srcinfo.pre_intr.read_intr_en) {
            srcinfo.pre_intr.read_intr_en = true;
            srcinfo.pre_intr.read_hold_en = opt.intr_config.flags & IM_INTR_READ_HOLD ? true : false;
            srcinfo.pre_intr.read_threshold = opt.intr_config.read_threshold;
        }

        srcinfo.pre_intr.write_intr_en = opt.intr_config.flags & IM_INTR_WRITE_INTR ? true : false;
        if (srcinfo.pre_intr.write_intr_en > 0) {
                srcinfo.pre_intr.write_start = opt.intr_config.write_start;
                srcinfo.pre_intr.write_step = opt.intr_config.write_step;
        }
    }

    /* special config for color space convert */
    if ((dst.color_space_mode & IM_YUV_TO_RGB_MASK) && (dst.color_space_mode & IM_RGB_TO_YUV_MASK)) {
        if (rga_is_buffer_valid(pat) &&
            NormalRgaIsYuvFormat(RkRgaGetRgaFormat(src.format)) &&
            NormalRgaIsRgbFormat(RkRgaGetRgaFormat(pat.format)) &&
            NormalRgaIsYuvFormat(RkRgaGetRgaFormat(dst.format))) {
            dstinfo.color_space_mode = dst.color_space_mode;
        } else {
            imSetErrorMsg("Not yuv + rgb -> yuv does not need for color_sapce_mode R2Y & Y2R, please fix, "
                          "src_fromat = 0x%x(%s), src1_format = 0x%x(%s), dst_format = 0x%x(%s)",
                          src.format, translate_format_str(src.format),
                          pat.format, translate_format_str(pat.format),
                          dst.format, translate_format_str(dst.format));
            return IM_STATUS_ILLEGAL_PARAM;
        }
    } else if (dst.color_space_mode & (IM_YUV_TO_RGB_MASK)) {
        if (rga_is_buffer_valid(pat) &&
            NormalRgaIsYuvFormat(RkRgaGetRgaFormat(src.format)) &&
            NormalRgaIsRgbFormat(RkRgaGetRgaFormat(pat.format)) &&
            NormalRgaIsRgbFormat(RkRgaGetRgaFormat(dst.format))) {
            dstinfo.color_space_mode = dst.color_space_mode;
        } else if (NormalRgaIsYuvFormat(RkRgaGetRgaFormat(src.format)) &&
                   NormalRgaIsRgbFormat(RkRgaGetRgaFormat(dst.format))) {
            dstinfo.color_space_mode = dst.color_space_mode;
        } else {
            imSetErrorMsg("Not yuv to rgb does not need for color_sapce_mode, please fix, "
                          "src_fromat = 0x%x(%s), src1_format = 0x%x(%s), dst_format = 0x%x(%s)",
                          src.format, translate_format_str(src.format),
                          pat.format, rga_is_buffer_valid(pat) ? translate_format_str(pat.format) : "none",
                          dst.format, translate_format_str(dst.format));
            return IM_STATUS_ILLEGAL_PARAM;
        }
    } else if (dst.color_space_mode & (IM_RGB_TO_YUV_MASK)) {
        if (rga_is_buffer_valid(pat) &&
            NormalRgaIsRgbFormat(RkRgaGetRgaFormat(src.format)) &&
            NormalRgaIsRgbFormat(RkRgaGetRgaFormat(pat.format)) &&
            NormalRgaIsYuvFormat(RkRgaGetRgaFormat(dst.format))) {
            dstinfo.color_space_mode = dst.color_space_mode;
        } else if (NormalRgaIsRgbFormat(RkRgaGetRgaFormat(src.format)) &&
                   NormalRgaIsYuvFormat(RkRgaGetRgaFormat(dst.format))) {
            dstinfo.color_space_mode = dst.color_space_mode;
        } else {
            imSetErrorMsg("Not rgb to yuv does not need for color_sapce_mode, please fix, "
                          "src_fromat = 0x%x(%s), src1_format = 0x%x(%s), dst_format = 0x%x(%s)",
                          src.format, translate_format_str(src.format),
                          pat.format, rga_is_buffer_valid(pat) ? translate_format_str(pat.format) : "none",
                          dst.format, translate_format_str(dst.format));
            return IM_STATUS_ILLEGAL_PARAM;
        }
    } else if (src.color_space_mode & IM_FULL_CSC_MASK ||
               dst.color_space_mode & IM_FULL_CSC_MASK) {
        /* Get default color space */
        if (src.color_space_mode == IM_COLOR_SPACE_DEFAULT) {
            if  (NormalRgaIsRgbFormat(RkRgaGetRgaFormat(src.format))) {
                src.color_space_mode = IM_RGB_FULL;
            } else if (NormalRgaIsYuvFormat(RkRgaGetRgaFormat(src.format))) {
                src.color_space_mode = IM_YUV_BT601_LIMIT_RANGE;
            }
        }

        if (dst.color_space_mode == IM_COLOR_SPACE_DEFAULT) {
            if  (NormalRgaIsRgbFormat(RkRgaGetRgaFormat(dst.format))) {
                src.color_space_mode = IM_RGB_FULL;
            } else if (NormalRgaIsYuvFormat(RkRgaGetRgaFormat(dst.format))) {
                src.color_space_mode = IM_YUV_BT601_LIMIT_RANGE;
            }
        }

        if (src.color_space_mode == IM_RGB_FULL &&
            dst.color_space_mode == IM_YUV_BT709_FULL_RANGE) {
            dstinfo.color_space_mode = rgb2yuv_709_full;
        } else if (src.color_space_mode == IM_YUV_BT601_FULL_RANGE &&
                   dst.color_space_mode == IM_YUV_BT709_LIMIT_RANGE) {
            dstinfo.color_space_mode = yuv2yuv_601_full_2_709_limit;
        } else if (src.color_space_mode == IM_YUV_BT709_LIMIT_RANGE &&
                   dst.color_space_mode == IM_YUV_BT601_LIMIT_RANGE) {
            dstinfo.color_space_mode = yuv2yuv_709_limit_2_601_limit;
        } else if (src.color_space_mode == IM_YUV_BT709_FULL_RANGE &&
                   dst.color_space_mode == IM_YUV_BT601_LIMIT_RANGE) {
            dstinfo.color_space_mode = yuv2yuv_709_full_2_601_limit;
        } else if (src.color_space_mode == IM_YUV_BT709_FULL_RANGE &&
                   dst.color_space_mode == IM_YUV_BT601_FULL_RANGE) {
            dstinfo.color_space_mode = yuv2yuv_709_full_2_601_full;
        } else {
            imSetErrorMsg("Unsupported full csc mode! src_csm = 0x%x, dst_csm = 0x%x",
                          src.color_space_mode, dst.color_space_mode);
            return IM_STATUS_NOT_SUPPORTED;
        }
    }

    if (dst.format == RK_FORMAT_Y4) {
        switch (dst.color_space_mode) {
            case IM_RGB_TO_Y4 :
                dstinfo.dither.enable = 0;
                dstinfo.dither.mode = 0;
                break;
            case IM_RGB_TO_Y4_DITHER :
                dstinfo.dither.enable = 1;
                dstinfo.dither.mode = 0;
                break;
            case IM_RGB_TO_Y1_DITHER :
                dstinfo.dither.enable = 1;
                dstinfo.dither.mode = 1;
                break;
            default :
                dstinfo.dither.enable = 1;
                dstinfo.dither.mode = 0;
                break;
        }
        dstinfo.dither.lut0_l = 0x3210;
        dstinfo.dither.lut0_h = 0x7654;
        dstinfo.dither.lut1_l = 0xba98;
        dstinfo.dither.lut1_h = 0xfedc;
    }

	srcinfo.rd_mode = src.rd_mode;
	dstinfo.rd_mode = dst.rd_mode;
	if (rga_is_buffer_valid(pat))
		patinfo.rd_mode = pat.rd_mode;

    RockchipRga& rkRga(RockchipRga::get());

    if (usage & IM_ASYNC) {
        if (release_fence_fd == NULL) {
            imSetErrorMsg("Async mode release_fence_fd cannot be NULL!");
            return IM_STATUS_ILLEGAL_PARAM;
        }

        dstinfo.sync_mode = RGA_BLIT_ASYNC;

    } else if (usage & IM_SYNC) {
        dstinfo.sync_mode = RGA_BLIT_SYNC;
    }

    dstinfo.in_fence_fd = acquire_fence_fd;
    dstinfo.core = opt.core ? opt.core : g_im2d_context.core;
    dstinfo.priority = opt.priority ? opt.priority : g_im2d_context.priority;

    if (ctx_id != 0) {
        dstinfo.ctx_id = ctx_id;
        if (dstinfo.ctx_id <= 0) {
            imSetErrorMsg("ctx id is invalid");
            return IM_STATUS_ILLEGAL_PARAM;
        }
        dstinfo.mpi_mode = 1;
    }

    if (usage & IM_COLOR_FILL) {
        dstinfo.color = opt.color;
        ret = rkRga.RkRgaCollorFill(&dstinfo);
    } else if (usage & IM_COLOR_PALETTE) {
        ret = rkRga.RkRgaCollorPalette(&srcinfo, &dstinfo, &patinfo);
    } else if ((usage & IM_ALPHA_BLEND_MASK) && rga_is_buffer_valid(pat)) {
        ret = rkRga.RkRgaBlit(&srcinfo, &dstinfo, &patinfo);
    }else {
        ret = rkRga.RkRgaBlit(&srcinfo, &dstinfo, NULL);
    }

    if (ret) {
        imSetErrorMsg("Failed to call RockChipRga interface, query log to find the cause of failure.");
        ALOGE("srect[x,y,w,h] = [%d, %d, %d, %d] src[w,h,ws,hs] = [%d, %d, %d, %d]\n",
               srect.x, srect.y, srect.width, srect.height, src.width, src.height, src.wstride, src.hstride);
        if (rga_is_buffer_valid(pat))
           ALOGE("s1/prect[x,y,w,h] = [%d, %d, %d, %d] src1/pat[w,h,ws,hs] = [%d, %d, %d, %d]\n",
               prect.x, prect.y, prect.width, prect.height, pat.width, pat.height, pat.wstride, pat.hstride);
        ALOGE("drect[x,y,w,h] = [%d, %d, %d, %d] dst[w,h,ws,hs] = [%d, %d, %d, %d]\n",
               drect.x, drect.y, drect.width, drect.height, dst.width, dst.height, dst.wstride, dst.hstride);
        ALOGE("usage[0x%x]", usage);
        return IM_STATUS_FAILED;
    }

    if (usage & IM_ASYNC)
        *release_fence_fd = dstinfo.out_fence_fd;

    return IM_STATUS_SUCCESS;
}

IM_API IM_STATUS imsync(int fence_fd) {
    int ret = 0;

    ret = rga_sync_wait(fence_fd, -1);
    if (ret) {
        ALOGE("Failed to wait for out fence = %d, ret = %d", fence_fd, ret);
        return IM_STATUS_FAILED;
    }

    close(fence_fd);

    return IM_STATUS_SUCCESS;
}

IM_API IM_STATUS imconfig(IM_CONFIG_NAME name, uint64_t value) {

    switch (name) {
        case IM_CONFIG_SCHEDULER_CORE :
            if (value & IM_SCHEDULER_MASK) {
                g_im2d_context.core = (IM_SCHEDULER_CORE)value;
            } else {
                ALOGE("IM2D: It's not legal rga_core, it needs to be a 'IM_SCHEDULER_CORE'.");
                return IM_STATUS_ILLEGAL_PARAM;
            }
            break;
        case IM_CONFIG_PRIORITY :
            if (value >= 0 && value <= 6) {
                g_im2d_context.priority = (int)value;
            } else {
                ALOGE("IM2D: It's not legal priority, it needs to be a 'int', and it should be in the range of 0~6.");
                return IM_STATUS_ILLEGAL_PARAM;
            }
            break;
        case IM_CONFIG_CHECK :
            if (value == false || value == true) {
                g_im2d_context.check_mode = (bool)value;
            } else {
                ALOGE("IM2D: It's not legal check config, it needs to be a 'bool'.");
                return IM_STATUS_ILLEGAL_PARAM;
            }
            break;
        default :
            ALOGE("IM2D: Unsupported config name!");
            return IM_STATUS_NOT_SUPPORTED;
    }

    return IM_STATUS_SUCCESS;
}

IM_API im_ctx_id_t imbegin(uint32_t flags) {
    return rga_begin_job(flags);
}

/* For the C interface */
IM_API rga_buffer_t wrapbuffer_handle_t(rga_buffer_handle_t  handle,
                                        int width, int height,
                                        int wstride, int hstride,
                                        int format) {
    return wrapbuffer_handle(handle, width, height, wstride, hstride, format);
}

IM_API IM_STATUS imresize_t(const rga_buffer_t src, rga_buffer_t dst, double fx, double fy, int interpolation, int sync) {
    return imresize(src, dst, fx, fy, interpolation, sync, NULL);
}

IM_API IM_STATUS imcrop_t(const rga_buffer_t src, rga_buffer_t dst, im_rect rect, int sync) {
    return imcrop(src, dst, rect, sync, NULL);
}

IM_API IM_STATUS imrotate_t(const rga_buffer_t src, rga_buffer_t dst, int rotation, int sync) {
    return imrotate(src, dst, rotation, sync, NULL);
}

IM_API IM_STATUS imflip_t (const rga_buffer_t src, rga_buffer_t dst, int mode, int sync) {
    return imflip(src, dst, mode, sync, NULL);
}

IM_API IM_STATUS imfill_t(rga_buffer_t dst, im_rect rect, int color, int sync) {
    return imfill(dst, rect, color, sync, NULL);
}

IM_API IM_STATUS impalette_t(rga_buffer_t src, rga_buffer_t dst, rga_buffer_t lut, int sync) {
    return impalette(src, dst, lut, sync, NULL);
}

IM_API IM_STATUS imtranslate_t(const rga_buffer_t src, rga_buffer_t dst, int x, int y, int sync) {
    return imtranslate(src, dst, x, y, sync, NULL);
}

IM_API IM_STATUS imcopy_t(const rga_buffer_t src, rga_buffer_t dst, int sync) {
    return imcopy(src, dst, sync, NULL);
}

IM_API IM_STATUS imcolorkey_t(const rga_buffer_t src, rga_buffer_t dst, im_colorkey_range range, int mode, int sync) {
    return imcolorkey(src, dst, range, mode, sync, NULL);
}

IM_API IM_STATUS imblend_t(const rga_buffer_t srcA, const rga_buffer_t srcB, rga_buffer_t dst, int mode, int sync) {
    return imcomposite(srcA, srcB, dst, mode, sync, NULL);
}

IM_API IM_STATUS imcvtcolor_t(rga_buffer_t src, rga_buffer_t dst, int sfmt, int dfmt, int mode, int sync) {
    return imcvtcolor(src, dst, sfmt, dfmt, mode, sync, NULL);
}

IM_API IM_STATUS imquantize_t(const rga_buffer_t src, rga_buffer_t dst, im_nn_t nn_info, int sync) {
    return imquantize(src, dst, nn_info, sync, NULL);
}

IM_API IM_STATUS imrop_t(const rga_buffer_t src, rga_buffer_t dst, int rop_code, int sync) {
    return imrop(src, dst, rop_code, sync, NULL);
}

IM_API IM_STATUS immosaic(const rga_buffer_t image, im_rect rect, int mosaic_mode, int sync) {
    return immosaic(image, rect, mosaic_mode, sync, NULL);
}

IM_API IM_STATUS imosd(const rga_buffer_t osd,const rga_buffer_t dst, const im_rect osd_rect,
                       im_osd_t *osd_info, int sync) {
    return imosd(osd, dst, osd_rect, osd_info, sync, NULL);
}
