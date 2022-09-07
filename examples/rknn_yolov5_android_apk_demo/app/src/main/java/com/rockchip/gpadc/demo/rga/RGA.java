package com.rockchip.gpadc.demo.rga;

import android.util.Log;

public class RGA {
    static {
        System.loadLibrary("rknn4j");
    }

    public static int colorConvert(byte[] src, int srcFmt, byte[] dst, int dstFmt,
                                   int width, int height, int flip) {

        if (src == null || dst == null) {
            Log.w("rkyolo.RGA", "src or dst is null");
            return -1;
        }

        return color_convert(src, srcFmt, dst, dstFmt, width, height, flip);
    }

    private static native int color_convert(byte[] src, int srcFmt, byte[] dst, int dstFmt,
                                            int width, int height, int flip);
}
