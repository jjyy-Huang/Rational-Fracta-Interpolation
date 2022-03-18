#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "pthread.h"
#include "ctime"

#include "./inc/imgproc.h"
#include "./inc/core.h"

#define varMAME(x)  (#x)

int main() {
    const char* gt_pth = "../img/HR/";
    const char* lr_pth = "../img/LR/";
    const char* output_pth = "../img/output/";
    const char* img_format = ".bmp";
//    int i = 0;
    const char* i = "baby";
    int scale = 4;
    bool isLRImageSet = true;

    char gt_img[64] = "../img/Set5/image_SRF_4/img_004_SRF_4_HR.png";
    char lr_img[64] = "../img/Set5/image_SRF_4/img_004_SRF_4_LR.png";
    char output_img[64] = {};
//    sprintf(gt_img, "%s%s%s", gt_pth, i, img_format);
//    sprintf(lr_img, "%s%s%s", lr_pth, i, img_format);
//    sprintf(output_img, "%s%s%s", output_pth, i, img_format);

    cv::Mat img_read_GT;
    cv::Mat img_read_LR;
    cv::Mat img_LR_BGR;

    img_read_GT = cv::imread(gt_img);
    if ( isLRImageSet == true) {
        img_read_LR = cv::imread(lr_img);
        img_LR_BGR = img_read_LR;
    } else {
        img_LR_BGR = downsample(img_read_GT, scale, DIRECT);
    }

    cv::Mat img_Bilinear_SR;
    cv::Mat img_Bicubic_SR;
    cv::resize(img_LR_BGR, img_Bilinear_SR, cv::Size(), scale, scale, cv::INTER_LINEAR);
    cv::resize(img_LR_BGR, img_Bicubic_SR, cv::Size(), scale, scale, cv::INTER_CUBIC);


    cv::Mat ycrcb_channels[3];
    cv::Mat img_LR_YCbCr;
    cv::cvtColor(img_LR_BGR, img_LR_YCbCr, cv::COLOR_BGR2YCrCb);
    cv::split(img_LR_YCbCr, ycrcb_channels);
    cv::Mat img_LR_y  = ycrcb_channels[0];
    cv::Mat img_LR_cr = ycrcb_channels[1];
    cv::Mat img_LR_cb = ycrcb_channels[2];


    img_LR_y = preProcess(img_LR_y, 3);


    clock_t start = clock();
//    core upsampling function
    cv::Mat img_SR_y = core(img_LR_y, img_read_GT.rows, img_read_GT.cols, scale);
    clock_t end   = clock();

    cv::Mat img_SR_cb;
    cv::Mat img_SR_cr;
    cv::resize(img_LR_cb, img_SR_cb, cv::Size(), scale, scale, cv::INTER_CUBIC);
    cv::resize(img_LR_cr, img_SR_cr, cv::Size(), scale, scale, cv::INTER_CUBIC);

    cv::Mat img_SR_ycrcb(img_read_GT.rows, img_read_GT.cols, CV_8UC3);
    cv::Mat img_SR_bgr(img_read_GT.rows, img_read_GT.cols, CV_8UC3);
    ycrcb_channels[0] = img_SR_y;
    ycrcb_channels[1] = img_SR_cr;
    ycrcb_channels[2] = img_SR_cb;

    cv::merge(ycrcb_channels, 3, img_SR_ycrcb);
    cv::cvtColor(img_SR_ycrcb, img_SR_bgr, CV_YCrCb2BGR);

    cv::Mat sharpen_op = (cv::Mat_<int8_t>(3,3) <<
            0, -1, 0,
            -1, 5, -1,
            0, -1, 0);
    cv::Mat sharpen_img;
    cv::filter2D(img_SR_bgr, sharpen_img, CV_32F, sharpen_op);
    cv::convertScaleAbs(sharpen_img, sharpen_img);

    std::cout << "ImageSet:" << gt_img << std::endl;
    std::cout << varMAME(isLRImageSet) << ":" << std::boolalpha <<  isLRImageSet << std::endl;
    std::cout << "cost " << (double_t)(end - start) / CLOCKS_PER_SEC << " s" << std::endl;
    std::cout << "bilinear:\t" << cv::PSNR(img_read_GT, img_Bilinear_SR) << "\tdb" << std::endl;
    std::cout << "bicubic:\t" << cv::PSNR(img_read_GT, img_Bicubic_SR) << "\tdb" << std::endl;
    std::cout << "rational fractal interpolation:\t" << cv::PSNR(img_read_GT, img_SR_bgr) << "\tdb" << std::endl;
    std::cout << std::endl;


//    cv::imwrite(output_img, img_SR_bgr);


    return 0;
}

