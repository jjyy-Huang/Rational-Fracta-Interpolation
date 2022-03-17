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
//    cv::Mat test(2,2, CV_8UC1);
//    for ( int i = 0; i < test.rows; i += 1) {
//        for ( int j = 0; j < test.cols; j += 1 ) {
//            test.at<uint8_t>(i,j) = i*4 + j;
//        }
//    }
//
//    cv::Mat upscale;
//    cv::resize(test, upscale, cv::Size(), scale, scale, cv::INTER_CUBIC);


//    cv::Mat tmp = preProcess(test, 3);

    img_read_GT = cv::imread("../img/myImageSet/baby.bmp");

    cv::Mat testrgb(img_read_GT.rows, img_read_GT.cols, CV_8UC3);
    cv::Mat testycbcr(img_read_GT.rows, img_read_GT.cols, CV_8UC3);
    RGB2YCbCr(img_read_GT, testycbcr);
    YCbCr2RGB(testycbcr, testrgb);
//    img_read_LR = cv::imread("../img/LR/1.bmp");


//    int check = checkImage(img_read_GT, img_read_LR);
//
//    std::cout << check << std::endl;
    cv::Mat img_L = downsample(img_read_GT, scale, DIRECT);

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
    cv::Mat img_L_cb = splitYCbCr(img_L_ycbcr, CHANNEL_CB);
    cv::Mat img_L_cr = splitYCbCr(img_L_ycbcr, CHANNEL_CR);

    img_L_y = preProcess(img_L_y, 1);


    clock_t start = clock();
//    core upsampling function
    cv::Mat img_H_y = core(img_L_y);
    clock_t end   = clock();

    cv::Mat img_H_cb;
    cv::Mat img_H_cr;
    cv::resize(img_L_cb, img_H_cb, cv::Size(), scale, scale, cv::INTER_CUBIC);
    cv::resize(img_L_cr, img_H_cr, cv::Size(), scale, scale, cv::INTER_CUBIC);

    cv::Mat img_SR_ycbcr(img_read_GT.rows, img_read_GT.cols, CV_8UC3);
    cv::Mat img_SR_rgb(img_read_GT.rows, img_read_GT.cols, CV_8UC3);
    cv::Mat channels_sr[3];
    channels_sr[0] = img_H_y;
    channels_sr[1] = img_H_cb;
    channels_sr[2] = img_H_cr;

    cv::merge(channels_sr, 3, img_SR_ycbcr);
//    cv::cvtColor(img_SR_ycbcr, img_SR_rgb, CV_YCrCb2BGR);
    YCbCr2RGB(img_SR_ycbcr, img_SR_rgb);

//    cv::imshow("sr", img_SR_rgb);

    std::cout << "cost " << (double_t)(end - start) / CLOCKS_PER_SEC << " s" << std::endl;

    cv::imwrite("../img/output/baby.bmp", img_SR_rgb);

    return 0;
}

