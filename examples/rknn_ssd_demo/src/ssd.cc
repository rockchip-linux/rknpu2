// Copyright (c) 2021 by Rockchip Electronics Co., Ltd. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "ssd.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/time.h>

#define BOX_PRIORS_TXT_PATH "./model/box_priors.txt"
#define LABEL_NALE_TXT_PATH "./model/coco_labels_list.txt"

float MIN_SCORE     = 0.4f;
float NMS_THRESHOLD = 0.45f;

static char* labels[NUM_CLASS];
static float box_priors[4][NUM_RESULTS];

int64_t getCurrentTimeUs()
{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec * 1000000 + tv.tv_usec;
}

char* readLine(FILE* fp, char* buffer, int* len)
{
  int    ch;
  int    i        = 0;
  size_t buff_len = 0;

  buffer = (char*)malloc(buff_len + 1);
  if (!buffer)
    return NULL; // Out of memory

  while ((ch = fgetc(fp)) != '\n' && ch != EOF) {
    buff_len++;
    void* tmp = realloc(buffer, buff_len + 1);
    if (tmp == NULL) {
      free(buffer);
      return NULL; // Out of memory
    }
    buffer = (char*)tmp;

    buffer[i] = (char)ch;
    i++;
  }
  buffer[i] = '\0';

  *len = buff_len;

  // Detect end
  if (ch == EOF && (i == 0 || ferror(fp))) {
    free(buffer);
    return NULL;
  }
  return buffer;
}

int readLines(const char* fileName, char* lines[], int max_line)
{
  FILE* file = fopen(fileName, "r");
  char* s;
  int   i = 0;
  int   n = 0;

  if (file == NULL) {
    printf("Open %s fail!\n", fileName);
    return -1;
  }

  while ((s = readLine(file, s, &n)) != NULL) {
    lines[i++] = s;
    if (i >= max_line)
      break;
  }

  fclose(file);
  return i;
}

int loadLabelName(const char* locationFilename, char* label[])
{
  printf("ssd - loadLabelName %s\n", locationFilename);
  return readLines(locationFilename, label, NUM_CLASS);
}

int loadBoxPriors(const char* locationFilename, float (*boxPriors)[NUM_RESULTS])
{
  const char* d = " ";
  char*       lines[4];
  int         count = readLines(locationFilename, lines, 4);

  for (int i = 0; i < 4; i++) {
    char* line_str = lines[i];
    char* p;
    p              = strtok(line_str, d);
    int priorIndex = 0;

    while (p) {
      float number               = (float)(atof(p));
      boxPriors[i][priorIndex++] = number;
      p                          = strtok(NULL, d);
    }

    if (priorIndex != NUM_RESULTS) {
      printf("error\n");
      return -1;
    }
  }

  for (int i = 0; i < 4; i++) {
    free(lines[i]);
  }

  return 0;
}

float CalculateOverlap(float xmin0, float ymin0, float xmax0, float ymax0, float xmin1, float ymin1, float xmax1,
                       float ymax1)
{
  float w = fmax(0.f, fmin(xmax0, xmax1) - fmax(xmin0, xmin1));
  float h = fmax(0.f, fmin(ymax0, ymax1) - fmax(ymin0, ymin1));
  float i = w * h;
  float u = (xmax0 - xmin0) * (ymax0 - ymin0) + (xmax1 - xmin1) * (ymax1 - ymin1) - i;
  return u <= 0.f ? 0.f : (i / u);
}

float unexpit(float y) { return -1.0 * logf((1.0 / y) - 1.0); }

float expit(float x) { return (float)(1.0 / (1.0 + expf(-x))); }

void decodeCenterSizeBoxes(float* predictions, float (*boxPriors)[NUM_RESULTS])
{
  for (int i = 0; i < NUM_RESULTS; ++i) {
    float ycenter = predictions[i * 4 + 0] / Y_SCALE * boxPriors[2][i] + boxPriors[0][i];
    float xcenter = predictions[i * 4 + 1] / X_SCALE * boxPriors[3][i] + boxPriors[1][i];
    float h       = (float)expf(predictions[i * 4 + 2] / H_SCALE) * boxPriors[2][i];
    float w       = (float)expf(predictions[i * 4 + 3] / W_SCALE) * boxPriors[3][i];

    float ymin = ycenter - h / 2.0f;
    float xmin = xcenter - w / 2.0f;
    float ymax = ycenter + h / 2.0f;
    float xmax = xcenter + w / 2.0f;

    predictions[i * 4 + 0] = ymin;
    predictions[i * 4 + 1] = xmin;
    predictions[i * 4 + 2] = ymax;
    predictions[i * 4 + 3] = xmax;
  }
}

int filterValidResult(float* outputClasses, int (*output)[NUM_RESULTS], int numClasses, float* props)
{
  int   validCount = 0;
  float min_score  = unexpit(MIN_SCORE);

  // Scale them back to the input size.
  for (int i = 0; i < NUM_RESULTS; ++i) {
    float topClassScore      = (float)(-1000.0);
    int   topClassScoreIndex = -1;

    // Skip the first catch-all class.
    for (int j = 1; j < numClasses; ++j) {
      // x and expit(x) has same monotonicity
      // so compare x and comare expit(x) is same
      // float score = expit(outputClasses[i*numClasses+j]);
      float score = outputClasses[i * numClasses + j];

      if (score > topClassScore) {
        topClassScoreIndex = j;
        topClassScore      = score;
      }
    }

    if (topClassScore >= min_score) {
      output[0][validCount] = i;
      output[1][validCount] = topClassScoreIndex;
      props[validCount]     = expit(outputClasses[i * numClasses + topClassScoreIndex]);
      ++validCount;
    }
  }

  return validCount;
}

int nms(int validCount, float* outputLocations, int (*output)[NUM_RESULTS])
{
  for (int i = 0; i < validCount; ++i) {
    if (output[0][i] == -1) {
      continue;
    }
    int n = output[0][i];
    for (int j = i + 1; j < validCount; ++j) {
      int m = output[0][j];
      if (m == -1) {
        continue;
      }
      float xmin0 = outputLocations[n * 4 + 1];
      float ymin0 = outputLocations[n * 4 + 0];
      float xmax0 = outputLocations[n * 4 + 3];
      float ymax0 = outputLocations[n * 4 + 2];

      float xmin1 = outputLocations[m * 4 + 1];
      float ymin1 = outputLocations[m * 4 + 0];
      float xmax1 = outputLocations[m * 4 + 3];
      float ymax1 = outputLocations[m * 4 + 2];

      float iou = CalculateOverlap(xmin0, ymin0, xmax0, ymax0, xmin1, ymin1, xmax1, ymax1);

      if (iou >= NMS_THRESHOLD) {
        output[0][j] = -1;
      }
    }
  }
  return 0;
}

void sort(int output[][1917], float* props, int sz)
{
  int i = 0;
  int j = 0;

  if (sz < 2) {
    return;
  }

#if 1
  for (i = 0; i < sz - 1; i++) {
    int top = i;
    for (j = i + 1; j < sz; j++) {
      if (props[top] < props[j]) {
        top = j;
      }
    }

    if (i != top) {
      int   tmp1     = output[0][i];
      int   tmp2     = output[1][i];
      float prop     = props[i];
      output[0][i]   = output[0][top];
      output[1][i]   = output[1][top];
      props[i]       = props[top];
      output[0][top] = tmp1;
      output[1][top] = tmp2;
      props[top]     = prop;
    }
  }
#endif

#if 0
    for(i = 0; i < sz-1; i++) {
        for(j = 0; j < sz-i-1; j++) {
            if(props[j] < props[j+1]) {
                int tmp1 = output[0][j];
                int tmp2 = output[1][j];
                float prop = props[j];
                output[0][j] = output[0][j+1];
                output[1][j] = output[1][j+1];
                props[j] = props[j+1];
                output[0][j+1] = tmp1;
                output[1][j+1] = tmp2;
                props[j+1] = prop;
            }
        }
    }
#endif
}

int postProcessSSD(float* predictions, float* output_classes, int width, int heigh, detect_result_group_t* group)
{
  static int init = -1;
  if (init == -1) {
    int ret = 0;

    printf("loadLabelName\n");
    ret = loadLabelName(LABEL_NALE_TXT_PATH, labels);
    if (ret < 0) {
      return -1;
    }

    printf("loadBoxPriors\n");
    ret = loadBoxPriors(BOX_PRIORS_TXT_PATH, box_priors);
    if (ret < 0) {
      return -1;
    }

    init = 0;
  }

  int   output[2][NUM_RESULTS];
  float props[NUM_RESULTS];
  memset(output, 0, 2 * NUM_RESULTS);
  memset(props, 0, sizeof(float) * NUM_RESULTS);

  decodeCenterSizeBoxes(predictions, box_priors);

  int validCount = filterValidResult(output_classes, output, NUM_CLASS, props);

  if (validCount > OBJ_NUMB_MAX_SIZE) {
    printf("validCount too much !!\n");
    return -1;
  }

  sort(output, props, validCount);

  /* detect nest box */
  nms(validCount, predictions, output);

  int last_count = 0;
  group->count   = 0;
  /* box valid detect target */
  for (int i = 0; i < validCount; ++i) {
    if (output[0][i] == -1) {
      continue;
    }
    int n                  = output[0][i];
    int topClassScoreIndex = output[1][i];

    int x1 = (int)(predictions[n * 4 + 1] * width);
    int y1 = (int)(predictions[n * 4 + 0] * heigh);
    int x2 = (int)(predictions[n * 4 + 3] * width);
    int y2 = (int)(predictions[n * 4 + 2] * heigh);
    // There are a bug show toothbrush always
    if (x1 == 0 && x2 == 0 && y1 == 0 && y2 == 0)
      continue;
    char* label = labels[topClassScoreIndex];

    group->results[last_count].box.left   = x1;
    group->results[last_count].box.top    = y1;
    group->results[last_count].box.right  = x2;
    group->results[last_count].box.bottom = y2;
    group->results[last_count].prop       = props[i];
    memcpy(group->results[last_count].name, label, OBJ_NAME_MAX_SIZE);

    // printf("ssd result %2d: (%4d, %4d, %4d, %4d), %4.2f, %s\n", i, x1, y1, x2, y2, props[i], label);
    last_count++;
  }

  group->count = last_count;
  return 0;
}

void deinitPostProcessSSD()
{
  for (int i = 0; i < NUM_CLASS; i++) {
    if (labels[i] != nullptr) {
      free(labels[i]);
      labels[i] = nullptr;
    }
  }
}