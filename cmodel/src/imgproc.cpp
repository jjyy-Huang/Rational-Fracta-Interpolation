/*
 * @Description: imgproc.cpp in cmodel
 * @Author: huang
 * @Date: 3/9/22 3:36 PM
 */


#include "../inc/imgproc.h"


static void MatFormat_Int2Double(cv::Mat &src, cv::Mat &dst){
    src.convertTo(dst, CV_64FC1);
}

// just use the IP core
void RGB2YCbCr(cv::Mat &src, cv::Mat &dst){
    for ( int i = 0; i < src.rows; ++i) {
        for ( int j = 0; j < src.cols; ++j ) {
            dst.at<cv::Vec3b>(i, j)[0] = 16 +
                                         0.257 * (double_t) src.at<cv::Vec3b>(i, j)[2] +
                                         0.504 * (double_t) src.at<cv::Vec3b>(i, j)[1] +
                                         0.098 * (double_t) src.at<cv::Vec3b>(i, j)[0];
            dst.at<cv::Vec3b>(i, j)[1] = 128 +
                                         -0.148 * (double_t) src.at<cv::Vec3b>(i, j)[2] +
                                         0.291 * (double_t) src.at<cv::Vec3b>(i, j)[1] +
                                         0.439 * (double_t) src.at<cv::Vec3b>(i, j)[0];
            dst.at<cv::Vec3b>(i, j)[2] = 128 +
                                         0.439 * (double_t) src.at<cv::Vec3b>(i, j)[2] -
                                         0.368 * (double_t) src.at<cv::Vec3b>(i, j)[1] +
                                         0.071 * (double_t) src.at<cv::Vec3b>(i, j)[0];
        }
    }
}

cv::Mat splitYCbCr(cv::Mat &src, int index){
    cv::Mat channels[3];
    split(src,channels);
    return channels[index];
}


cv::Mat downsample(cv::Mat &origin, int scale, int method){
    cv::Mat img(origin.rows/scale, origin.cols/scale, CV_8UC3);

    switch (method) {
        case DIRECT:

            for ( int i = 0; i < origin.rows / scale; ++i ){
                for ( int j = 0; j < origin.cols / scale; ++j ){
                    img.at<cv::Vec3b>(i, j)[0] = origin.at<cv::Vec3b>(scale * i, scale * j)[0];
                    img.at<cv::Vec3b>(i, j)[1] = origin.at<cv::Vec3b>(scale * i, scale * j)[1];
                    img.at<cv::Vec3b>(i, j)[2] = origin.at<cv::Vec3b>(scale * i, scale * j)[2];
                }
            }
            break;

        case LINEAR:
            cv::resize(origin, img, img.size(), 0, 0, cv::INTER_LINEAR);
            break;

        case BICUBIC:
            cv::resize(origin, img, img.size(), 0, 0, cv::INTER_CUBIC);
            break;
    }

    return img;
}

