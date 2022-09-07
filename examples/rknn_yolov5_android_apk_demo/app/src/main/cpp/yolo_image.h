/**
  * @ClassName yolo_image
  * @Description TODO
  * @Author raul.rao
  * @Date 2022/5/23 11:43
  * @Version 1.0
  */
#ifndef RK_YOLOV5_DEMO_YOLO_IMAGE_H
#define RK_YOLOV5_DEMO_YOLO_IMAGE_H

#include <android/log.h>

#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, "rkyolo4j", ##__VA_ARGS__);
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, "rkyolo4j", ##__VA_ARGS__);

#define SPAN 3
#define LIST_SIZE 85
#define STRIDE0 8
#define STRIDE1 16
#define STRIDE2 32

int create(int im_height, int im_width, int im_channel, char *model_path);
void destroy();
bool run_yolo(char *inDataRaw, char *y0, char *y1, char *y2);
int yolo_post_process(char *grid0_buf, char *grid1_buf, char *grid2_buf,
                      int *ids, float *scores, float *boxes);
int colorConvert(void *src, int srcFmt, void *dst,  int dstFmt, int width, int height, int flip);

#endif //RK_YOLOV5_DEMO_YOLO_IMAGE_H
