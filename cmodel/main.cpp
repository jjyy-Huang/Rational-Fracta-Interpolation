#include <iostream>
#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "pthread.h"
#include "ctime"

#include "./inc/imgproc.h"
#include "./inc/core.h"

int main() {
    int scale = 4;

    cv::Mat img_read_GT;
    cv::Mat img_read_LR;
    cv::Mat img_H;

//  test code
//    cv::Mat test(3,3, CV_8UC1);
//    for ( int i = 0; i < test.rows; i += 1) {
//        for ( int j = 0; j < test.cols; j += 1 ) {
//            test.at<uint8_t>(i,j) = i*3 +j;
//        }
//    }
//
//    cv::Mat tmp = preProcess(test, 3);

    img_read_GT = cv::imread("../img/GT/1.bmp");
//    img_read_LR = cv::imread("../img/LR/1.bmp");


//    int check = checkImage(img_read_GT, img_read_LR);
//
//    std::cout << check << std::endl;
    cv::Mat img_L = downsample(img_read_GT, scale, LINEAR);

//    int check = checkImage(img_read_GT, img_L);
//
//    std::cout << check << std::endl;
//    cv::Mat img_resize_HR;
//    cv::resize(img_read_LR, img_resize_HR, cv::Size(), scale, scale, cv::INTER_LINEAR);
//
//    cv::Mat a;
//    cv::resize(img_L, a, cv::Size(), scale, scale, cv::INTER_LINEAR);
//    cv::Mat b;
//    cv::resize(img_L, b, cv::Size(), scale, scale, cv::INTER_CUBIC);
//    std::cout << cv::PSNR(img_read_GT, a) << std::endl;
//    std::cout << cv::PSNR(img_read_GT, b) << std::endl;


//    get image channel Y
    cv::Mat img_L_ycbcr(img_L.rows, img_L.cols, CV_8UC3);
    RGB2YCbCr(img_L, img_L_ycbcr);
    cv::Mat img_L_y = splitYCbCr(img_L_ycbcr, CHANNEL_Y);
    img_L_y = preProcess(img_L_y, 1);


    clock_t start = clock();
//    core upsampling function
    cv::Mat img_H_y = core(img_L_y);

    clock_t end   = clock();
    std::cout << "cost " << (double_t)(end - start) / CLOCKS_PER_SEC << " s" << std::endl;

    cv::waitKey(0);

    return 0;
}

