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
                                         65.738/256 * (double_t) src.at<cv::Vec3b>(i, j)[2] +
                                         129.057/256 * (double_t) src.at<cv::Vec3b>(i, j)[1] +
                                         25.064/256 * (double_t) src.at<cv::Vec3b>(i, j)[0];
            dst.at<cv::Vec3b>(i, j)[1] = 128 -
                                         37.945/256 * (double_t) src.at<cv::Vec3b>(i, j)[2] -
                                         74.494/256 * (double_t) src.at<cv::Vec3b>(i, j)[1] +
                                         112.439/256 * (double_t) src.at<cv::Vec3b>(i, j)[0];
            dst.at<cv::Vec3b>(i, j)[2] = 128 +
                                         112.439/256 * (double_t) src.at<cv::Vec3b>(i, j)[2] -
                                         94.154/256 * (double_t) src.at<cv::Vec3b>(i, j)[1] -
                                         18.285/256 * (double_t) src.at<cv::Vec3b>(i, j)[0];
        }
    }
}
void YCbCr2RGB(cv::Mat &src, cv::Mat &dst){
    for ( int i = 0; i < src.rows; ++i) {
        for ( int j = 0; j < src.cols; ++j ) {
            dst.at<cv::Vec3b>(i, j)[0] = (uint8_t)(1.164 * (double_t)(src.at<cv::Vec3b>(i, j)[0] - 16) +
                                                    2.017 * (double_t)((src.at<cv::Vec3b>(i, j)[1] - 128)));
            dst.at<cv::Vec3b>(i, j)[1] = (uint8_t)(1.164 * (double_t)(src.at<cv::Vec3b>(i, j)[0] - 16) -
                                                    0.392 * (double_t)(src.at<cv::Vec3b>(i, j)[1] - 128) -
                                                    0.813 * (double_t)(src.at<cv::Vec3b>(i, j)[2] - 128));
            dst.at<cv::Vec3b>(i, j)[2] = (uint8_t)(1.164 * (double_t)(src.at<cv::Vec3b>(i, j)[0] - 16) +
                                                    1.596 * (double_t)(src.at<cv::Vec3b>(i, j)[2] - 128));
        }
    }
}

//  insert N rows/cols at the edge
cv::Mat preProcess(cv::Mat &src, int num){
    cv::Mat tmp(src.rows + num, src.cols + num, CV_8UC1);
    for ( int i = 0; i < tmp.rows; ++i ){
        for ( int j = 0; j < tmp.cols; ++j ){
            int row, col;
            if ( j < src.cols )     col = j;
            else                    col = src.cols - 1;
            if ( i < src.rows)      row = i;
            else                    row = src.rows - 1;
            tmp.at<int8_t>(i, j) = src.at<int8_t>(row, col);
        }
    }
    return tmp;
}

cv::Mat splitYCbCr(cv::Mat &src, int index){
    cv::Mat channels[3];
    split(src,channels);
    return channels[index];
}

cv::Mat cutBlock(cv::Mat &src, int row, int col, int height, int width) {
    cv::Mat dst(height, width, CV_64FC1);
    for ( int i = 0; i < height; ++i ) {
        for ( int j = 0; j < height; ++j ) {
            dst.at<double_t>(i, j) = (double_t)src.at<uint8_t>(row + i, col + j);
        }
    }
    return dst;
}

void mergeBlock(cv::Mat &block, cv::Mat &img_sr, int row, int col, int height, int width) {
    for ( int i = 0; i < height; ++i ) {
        for ( int j = 0; j < width; ++j ) {
            uint8_t tmp = 0;
            if ( block.at<double_t>(i, j) >= UINT8_MAX )    tmp = UINT8_MAX;
            else if ( block.at<double_t>(i, j) <= 0 )       tmp = 0;
            else                                            tmp = (uint8_t)block.at<double_t>(i, j);
            img_sr.at<uint8_t>(row+i, col+j) = tmp;
        }
    }
    return;
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
//  check the downsampling method
int checkImage(cv::Mat &GT, cv::Mat &LR){
    for ( int i = 0; i < LR.rows; ++i ){
        for ( int j = 0; j < LR.cols; ++j ){
            if ( GT.at<cv::Vec3b>(4*i, 4*j)[0] != LR.at<cv::Vec3b>(i, j)[0] ){
                std::cout << i << "," << j << std::endl;
                return 1;
            }
            if ( GT.at<cv::Vec3b>(4*i, 4*j)[1] != LR.at<cv::Vec3b>(i, j)[1] ){
                std::cout << i << "," << j << std::endl;
                return 2;
            }
            if ( GT.at<cv::Vec3b>(4*i, 4*j)[2] != LR.at<cv::Vec3b>(i, j)[2] ){
                std::cout << i << "," << j << std::endl;
                return 3;
            }
        }
    }
    return 0;
}

