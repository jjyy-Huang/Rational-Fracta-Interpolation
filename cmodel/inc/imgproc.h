/*
 * @Description: imgproc.h in cmodel
 * @Author: huang
 * @Date: 3/9/22 3:36 PM
 */


#ifndef CMODEL_IMGPROC_H
#define CMODEL_IMGPROC_H

#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"


enum dmethod {
    DIRECT = 0,
    LINEAR = 1,
    BICUBIC = 2,
};

enum channel_num{
    CHANNEL_Y = 0,
    CHANNEL_CB = 1,
    CHANNEL_CR = 2
};

cv::Mat downsample(cv::Mat &origin, int scale, int method);


void RGB2YCbCr(cv::Mat &src, cv::Mat &dst);
void YCbCr2RGB(cv::Mat &src, cv::Mat &dst);
cv::Mat splitYCbCr(cv::Mat &src, int index);

cv::Mat preProcess(cv::Mat &src, int num);
cv::Mat cutBlock(cv::Mat &src, int row, int col, int height, int width);
void mergeBlock(cv::Mat &block, cv::Mat &img_sr, int row, int col, int height, int width);


int checkImage(cv::Mat &GT, cv::Mat &LR);

#endif //CMODEL_IMGPROC_H
