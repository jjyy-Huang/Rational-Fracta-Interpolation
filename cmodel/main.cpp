#include <iostream>
#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "pthread.h"

#include "./inc/imgproc.h"
#include "./inc/core.h"

int main() {
    int scale = 4;

    cv::Mat img_read;
    cv::Mat img_H;

    img_read = cv::imread("../../8.bmp");

    cv::Mat img_L = downsample(img_read, scale, DIRECT);

//    get image channel Y
    cv::Mat img_L_ycbcr(img_L.rows, img_L.cols, CV_8UC3);
    RGB2YCbCr(img_L, img_L_ycbcr);
    cv::Mat img_L_y = splitYCbCr(img_L_ycbcr, CHANNEL_Y);

    cv::Mat img_H_y = core(img_L_y);

    cv::waitKey(0);

    return 0;
}

