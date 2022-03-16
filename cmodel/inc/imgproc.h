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
    CHANNEL_CB,
    CHANNEL_CR
};

cv::Mat downsample(cv::Mat &origin, int scale, int method);


void RGB2YCbCr(cv::Mat &src, cv::Mat &dst);
cv::Mat splitYCbCr(cv::Mat &src, int index);

cv::Mat preProcess(cv::Mat &src, int num);

int checkImage(cv::Mat &GT, cv::Mat &LR);

#endif //CMODEL_IMGPROC_H
