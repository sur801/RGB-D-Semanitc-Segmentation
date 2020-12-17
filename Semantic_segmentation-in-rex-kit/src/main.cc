#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include <pthread.h>
#include <getopt.h>             /* getopt_long() */
#include <fcntl.h>              /* low-level i/o */
#include <unistd.h>
#include <errno.h>
#include <stdint.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <sys/time.h>
#include <signal.h>
#include <linux/videodev2.h>

//common/include
#include "rknn_helper.h"
#include "camera_helper.h"
#include "rga_helper.h"
#include "configs.h"
#include "rknn_helper.h"
#include "pca9632.h"
//inc
//extern "C"{
#include "bisenet.h"
//};

#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>


uint8_t* camera_buf;
uint8_t* buf;
uint8_t* out;
uint8_t led_r, led_g, led_b;

bisenet_ctx ctx; //for ssd data save

pthread_cond_t thread_cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex_lock = PTHREAD_MUTEX_INITIALIZER;
uint8_t status = 0;
int cam_fd;
const char* card = CARD_DEV;

pthread_key_t a_key;

//socket config
int listenfd = 0, connfd = 0;


struct timeval tv_1; 
time_t nowtime_1;
struct tm *nowtm_1;

#define VIDEO_PACKET_SIZE CAM_H*CAM_W*CAM_CH
#define MODEL_NAME	"/AI_Example/bisenetv2_new_quant.rknn"
#define PRIOR_BOX 	"/usr/share/rknn_demo/box_priors.txt"
#define LABELS		"/AI_Example/Seg_label.txt"
#define PI 3.141592
//FILE IMAGE = fopen("mySSD/size1.png","r");

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
	std::cout << "OPENCV Version" << CV_VERSION <<std::endl;
	//thread settin
	printf("%s main pthread start \n", __func__);
	void* dst_v = NULL;
	dst_v = malloc(320*640*3);

	int ret;

	rga_info_t r_src;
	rga_info_t r_dst;

	memset(&r_src, 0, sizeof(rga_info_t));
	r_src.fd = -1;
	r_src.mmuFlag = 1;
	r_src.virAddr = camera_buf;
	memset(&r_dst, 0, sizeof(rga_info_t));
	r_dst.fd = -1;
	r_dst.mmuFlag = 1;
	r_dst.virAddr = dst_v;
	r_src.rotation = 0;
	gettimeofday(&tv_1, NULL);
	nowtime_1 = tv_1.tv_sec;
	nowtm_1 = localtime(&nowtime_1);
	printf("%d:%d:%d\n", nowtm_1->tm_min, nowtm_1->tm_sec, (int)tv_1.tv_usec/10000);
	bisenet_init(MODEL_NAME, LABELS, &ctx);
	printf("init rknn complete \n");
	bisenet_run(&ctx, (uint8_t*)dst_v, 640, 320, 320*640*3);

	return 0;
}

