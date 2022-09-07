package com.rockchip.gpadc.demo.rga;

public class HALDefine {
    public static final int RK_FORMAT_RGBA_8888    = (0x0 << 8);
    public static final int RK_FORMAT_RGB_888      = (0x2 << 8);
    public static final int RK_FORMAT_YCrCb_420_SP = (0xe << 8);
//    public static final int HAL_PIXEL_FORMAT_RGB_565 = 4;
//    public static final int HAL_PIXEL_FORMAT_BGRA_8888 = 5;
//    public static final int HAL_PIXEL_FORMAT_YCrCb_NV12 = 	0x15;	/* YUY2 */
//    public static final int HAL_PIXEL_FORMAT_YCRCB_420_SP =  17;

//    public static final int HAL_TRANSFORM_FLIP_H = 1; // 0x01
//    public static final int HAL_TRANSFORM_FLIP_V = 2; // 0x02
//    public static final int HAL_TRANSFORM_ROT_90 = 4; // 0x04
//    public static final int HAL_TRANSFORM_ROT_180 = 3; // 0x03
//    public static final int HAL_TRANSFORM_ROT_270 = 7; // 0x07

    // for rga
    public static final int IM_HAL_TRANSFORM_FLIP_H = (1 << 3); // 0x08

    // for camera comes with RK3588
    public static final int CAMERA_PREVIEW_WIDTH = 1280;
    public static final int CAMERA_PREVIEW_HEIGHT = 720;
}
