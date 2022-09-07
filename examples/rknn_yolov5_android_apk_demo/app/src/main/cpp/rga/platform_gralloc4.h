/*
 * Copyright (C) 2017-2018 RockChip Limited. All rights reserved.
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

/*  --------------------------------------------------------------------------------------------------------
 *  File:   platform_gralloc4.h
 *
 *  Desc:   声明对 buffer_handle_t 实例的 get metadata, import_buffer/free_buffer, lock_buffer/unlock_buffer 等接口.
 *          这些接口都将基于 IMapper 4.0 (gralloc 4.0) 实现.
 *
 *          -----------------------------------------------------------------------------------
 *          < 习语 和 缩略语 > :
 *
 *          -----------------------------------------------------------------------------------
 *  Usage:        
 *
 *  Note:
 *
 *  Author: ChenZhen
 *
 *  Log:
 *      init.
    ----Fri Aug 28 10:10:14 2020
 *      
 *  --------------------------------------------------------------------------------------------------------
 */


#ifndef __PLATFORM_GRALLOC4_H__
#define __PLATFORM_GRALLOC4_H__


/* ---------------------------------------------------------------------------------------------------------
 *  Include Files
 * ---------------------------------------------------------------------------------------------------------
 */
// #include <linux/kernel.h>

#include <stdint.h>

#include <cutils/native_handle.h>
#include <utils/Errors.h>

#include <ui/PixelFormat.h>

/* ---------------------------------------------------------------------------------------------------------
 *  Macros Definition
 * ---------------------------------------------------------------------------------------------------------
 */


namespace gralloc4 {
/* ---------------------------------------------------------------------------------------------------------
 *  Types and Structures Definition
 * ---------------------------------------------------------------------------------------------------------
 */


/* ---------------------------------------------------------------------------------------------------------
 *  Global Functions' Prototype
 * ---------------------------------------------------------------------------------------------------------
 */

int get_width(buffer_handle_t handle, uint64_t* width);

int get_height(buffer_handle_t handle, uint64_t* height);

int get_pixel_stride(buffer_handle_t handle, int* pixel_stride);

int get_byte_stride(buffer_handle_t handle, int* byte_stride);

int get_format_requested(buffer_handle_t handle, int* format_requested);

int get_usage(buffer_handle_t handle, uint64_t* usage);

int get_allocation_size(buffer_handle_t handle, uint64_t* usage);

int get_share_fd(buffer_handle_t handle, int* share_fd);

using android::status_t;

status_t importBuffer(buffer_handle_t rawHandle, buffer_handle_t* outHandle);

void freeBuffer(buffer_handle_t handle);

status_t lock(buffer_handle_t bufferHandle,
              uint64_t usage,
              int x,
              int y,
              int w,
              int h,
              void** outData);

void unlock(buffer_handle_t bufferHandle);

/* ---------------------------------------------------------------------------------------------------------
 *  Inline Functions Implementation
 * ---------------------------------------------------------------------------------------------------------
 */

}

#endif /* __PLATFORM_GRALLOC4_H__ */

