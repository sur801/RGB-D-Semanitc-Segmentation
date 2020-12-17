#include "bisenet.h"
#include "track_c_link_c++.h"
#include <algorithm>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <opencv2/core.hpp>
#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <string>


#define MAX_OUTPUT_NUM 200
#define FRAME_SIZE 20

object_T object_input[MAX_OUTPUT_NUM];
object_T object_output[MAX_OUTPUT_NUM];

char *labels[NUM_CLASS];
struct timeval tv; 
time_t nowtime;
struct tm *nowtm;
using namespace cv;
using namespace std;
std::ofstream outFile;
int colormap[19][3] ={
	{0, 0, 0},
	{179, 168, 168},
	{255, 241, 38},
	{217, 119, 252},
	{173, 109, 49},
	{61, 41, 21},
	{224, 22, 96},
	{255, 255, 255},
	{46, 255, 70},
	{165, 196, 169},
	{89, 255, 249},
	{255, 168, 5},
	{219, 143, 0},
	{219, 55, 0},
	{247, 131, 92},
	{7, 171, 83},
	{94, 7, 171},
	{102, 5, 76},
	{185, 230, 62},
};

void bisenet_post(float *outputs,int w, int h, bisenet_ctx *ctx,int index){
	int i;
	int j;
	float *seg_score = outputs;
	
	printf("size : %ld\n", sizeof(seg_score));
	printf("1\n");
	Mat seg_img(320,640,CV_32FC1,seg_score);
	printf("2\n");

	// for (i=0;i<320*640;i++){
	// 	outFile << seg_score[i] << " ";
	// 	if((i+1)%640==0){
	// 		outFile << "\n";
	// 	}
	// }


	/*
	for (i=0;i<320*640;i++){
		float max = -123456;
		int max_id = -1;
		for(j=1; j<20 ; j++) {
			if(max < seg_score[19*i+(j-1)]) {
				max = seg_score[19*i+(j-1)];
				max_id = (j-1);
			}

		}
		outFile << max_id;
		if((i+1)%640==0){
			outFile << "\n";
		}
	}



	for (i=1;i<19*8;i++){
		printf("%f\t",seg_score[i-1]);
		if(i%19==0){
			printf("\n\n\n");
		}
	}*/
	// for(i=1 ; i<320*640+1 ; i++) {
	//  	outFile << (int)seg_score[i-1] << " ";
	//  	if(i%640==0) {
	//  		outFile << "\n";
	//  	}
	// }

	
	Mat c_img; // display buf는 rgba 4채널 형태
	cvtColor(seg_img, c_img, CV_GRAY2RGB); // GRAY -> RGB를 하면 기본적으로 CV_8UC3 형태로 만들어짐
	c_img.convertTo(c_img, CV_8UC3);


	int rows = c_img.rows;
	int cols = c_img.cols;

	for (int i = 0; i < rows; i++) {
       for (int j = 0; j < cols; j++  ){ 
		   // 이미지 클래스 라벨 가져옴.
			int label = c_img.at<Vec3b>(i, j)[0];
			c_img.at<Vec3b>(i, j)[2] = colormap[label][0];
			c_img.at<Vec3b>(i, j)[1] = colormap[label][1];
			c_img.at<Vec3b>(i, j)[0] = colormap[label][2];
		   
       }
	}
	printf("3\n");
	imwrite("/Output_img/test_output_"+to_string(index)+".jpg",c_img);

	//outFile<<"\nscore map "<< to_string(index)<<endl;
}

int bisenet_init(const char *model_name, const char *label_path, bisenet_ctx *ctx)
{
	int ret;

	ret = rknn_init_helper(model_name, &ctx->rknn);
	if (ret != 0) {
		fprintf(stderr, "%s : Failed to load model", __func__);
		return -1;
	}
	return 0;
}

int bisenet_run(bisenet_ctx *ctx, uint8_t *img, int w, int h, ssize_t size)
{
	//outFile.open("output.txt",std::ios_base::app);
	for(int i=0 ; i<9 ; i++){

		gettimeofday(&tv, NULL);
		nowtime = tv.tv_sec;
		nowtm = localtime(&nowtime);
		printf("%d:%d:%d\n", nowtm->tm_min, nowtm->tm_sec, (int)tv.tv_usec/10000);
		int ret;
		// input 2개. rgb 이미지 읽어옴.
		rknn_input inputs[2];
		rknn_output outputs[1];
		std::string input_image ="/rgb_img/"+ to_string(i) + ".png";
		cout << input_image<<endl;
		cv::Mat image = cv::imread(input_image);
		cv::Mat resized_img;
		resize(image, resized_img, Size(640, 320),0, 0, CV_INTER_NN);
		cvtColor(resized_img, resized_img, CV_BGR2RGB);

		// input 2개. detph 이미지 읽어옴.
		std::string input_image2 ="/depth_img/d"+ to_string(i) + ".png";
		cout << input_image2<<endl;
		cv::Mat image2 = cv::imread(input_image2);
		cv::Mat resized_img2;
		resize(image2, resized_img2, Size(640, 320),0, 0, CV_INTER_NN);
		cvtColor(resized_img2, resized_img2, CV_BGR2RGB);

		// input for rgb
		memset(inputs, 0x00, sizeof(inputs));
		inputs[0].index = 0;
		inputs[0].type = RKNN_TENSOR_UINT8;
		inputs[0].size = size; // 320*640*3
		inputs[0].fmt = RKNN_TENSOR_NHWC;
		inputs[0].buf = resized_img.data;

		// input for depth map
		inputs[1].index = 1;
		inputs[1].type = RKNN_TENSOR_UINT8;
		inputs[1].size = size; // 320*640*3
		inputs[1].fmt = RKNN_TENSOR_NHWC;
		inputs[1].buf = resized_img2.data;

		printf("input set start \n");
		ret = rknn_inputs_set(ctx->rknn, 2, inputs);
		if (ret < 0) {
			fprintf(stdout, "%s fail\n", __func__);
			return -1;
		}
		printf("input set \n");
		ret = rknn_run(ctx->rknn, NULL);
		if (ret < 0) {
			fprintf(stdout, "%s fail\n", __func__);
			return -1;
		}

		printf("rknn_run done\n");

		memset(outputs, 0x00, sizeof(outputs));
		outputs[0].want_float = 1;

		ret = rknn_outputs_get(ctx->rknn, 1, outputs, NULL);
		if (ret < 0) {
			fprintf(stdout, "%s fail\n", __func__);
			return -1;
		}
		printf("size : %d",outputs[0].size);

		bisenet_post((float *)outputs[0].buf, w, h, ctx,i);
		printf("get output done\n");

		//SSD 후처리코드
		rknn_outputs_release(ctx->rknn, 1, outputs);

		printf("ssd post done\n");
	}

	
	return 0;
}
