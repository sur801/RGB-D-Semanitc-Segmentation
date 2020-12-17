#ifndef __bisenet_H__
#define __bisenet_H__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

#include <rknn_api.h>
#include "rknn_helper.h"
#include "rga_helper.h"
#include "configs.h"

/*
	Detection Classes 91
*/

#define NUM_CLASS 2

/*
	(Grid 19x19, 3 Boxes) + (Grid 10x10, 6 Boxes) + (Grid 5x5, 6 Boxes)
	(Grid 3x3, 6 Boxes) + (Grid 2x2, 6 Boxes) + (Grid 1x1, 6 Boxes)
	= 1917
*/
#define NUM_RESULTS 1917

#define MIN_SCORE	0.85f
#define NMS_THRESHOLD 0.3f

#define MAX_COUNT	100

#define Y_SCALE	10.0f
#define X_SCALE	10.0f
#define H_SCALE	5.0f
#define W_SCALE	5.0f


typedef struct 
{
	rknn_context rknn;


} bisenet_ctx;




int bisenet_init(const char *model_name, const char *label_path, bisenet_ctx *ctx);
int bisenet_run(bisenet_ctx *ctx, uint8_t *img, int w, int h, ssize_t size);
void bisenet_post(float *outputs,int w, int h, bisenet_ctx *ctx, int index);
#endif
