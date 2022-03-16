/*
 * @Description: upsampling.cpp in cmodel
 * @Author: huang
 * @Date: 3/13/22 4:03 PM
 */


#include "../inc/upsampling.h"

#define N       5
#define M       5

#define X(n)    x_init[n]
#define Y(n)    y_init[n]

#define Theta(n)    ((X(n) - X(0)) / (X(N-1) - X(0)))
#define ETA(n)      ((Y(n) - Y(0)) / (Y(M-1) - Y(0)))

//  initial vertical scaling factor
static double_t s_init[4][4] = {
        0.02, 0.02, 0.02, 0.02,
        0.02, 0.02, 0.02, 0.02,
        0.02, 0.02, 0.02, 0.02,
        0.02, 0.02, 0.02, 0.02
};

static double_t x_init[5] = {
        0, 0.5, 1.0, 1.5, 2
};
static double_t y_init[5] = {
        0, 0.5, 1.0, 1.5, 2
};

static double_t getAveragePixel(cv::Mat &block, int fx, int fy, int width) {
    double_t aver = 0.0;
    for ( int i = 0; i < width; ++i ){
        for ( int j = 0; j < width; ++j ){
            aver += block.at<double_t>(fx + i, fy + j);
        }
    }
    return aver / pow(width, 2);
}

static double_t getSumPixel(cv::Mat &block, int fx, int fy, double aver) {
    double_t sum = 0.0;
    sum = abs(block.at<double_t>(fx, fy) - aver) +
            abs(block.at<double_t>(fx, fy + 2) - aver) +
            abs(block.at<double_t>(fx + 2, fy) - aver) +
            abs(block.at<double_t>(fx + 2, fy + 2) - aver);
    return sum;
}

static void calVerticalScalingFactor(cv::Mat &block, cv::Mat &s, double_t aver, double_t sum, int patch_x, int patch_y) {
    for ( int i = 0; i < 2; ++i ) {
        for ( int j = 0; j < 2; ++j ) {
            s.at<double_t>(patch_x*2 + i, patch_y*2 + j) = s_init[patch_x*2][patch_y*2] *
                    abs(block.at<double_t>(patch_x*2 + i*2, patch_y*2 + j*2) - aver) / sum;
        }
    }
    return;
}

static cv::Mat initVerticalScalingFactor(cv::Mat &block, bool isTexture) {
    cv::Mat s = cv::Mat::zeros(4, 4, CV_64FC1);

    if ( isTexture == false ){
//      non-texture region just return 0
        ;
    }
    else {
        for ( int patch_x = 0; patch_x < 2; ++patch_x ) {
            for ( int patch_y = 0; patch_y < 2; ++patch_y ) {
                double_t aver = getAveragePixel(block, patch_x * 2, patch_y * 2, 3);
                double_t sum = getSumPixel(block, patch_x, patch_y, aver);
                calVerticalScalingFactor(block, s, aver, sum, patch_x, patch_y);
            }
        }
    }
    return s;
}

static double_t dx(cv::Mat &block, int i, int j, int size) {
    double_t res;
    if ( i == 0 ) {
        res = (-3.0 * block.at<double_t>(i, j) + 4.0 * block.at<double_t>(i + 1, j)  - block.at<double_t>(i + 2, j)) / (2.0 * (X(i+1) - X(i)));
    }
    else if ( i == size - 1 ) {
        res = (3.0 * block.at<double_t>(i, j) - 4.0 * block.at<double_t>(i - 1, j) + block.at<double_t>(i - 2, j)) / (2.0 * (X(i) - X(i - 1)));
    }
    else {
        res = (block.at<double_t>(i + 1, j) - block.at<double_t>(i - 1, j)) / (2.0 * (X(i+1) - X(i)));
    }
    return res;
}
static double_t dy(cv::Mat &block, int i, int j, int size) {
    double_t res;
    if ( j == 0 ) {
        res = (-3.0 * block.at<double_t>(i, j) + 4.0 * block.at<double_t>(i, j + 1) - block.at<double_t>(i, j + 2)) / (2.0 * (Y(j+1) - Y(j)));
    }
    else if ( j == size - 1 ) {
        res = (3.0 * block.at<double_t>(i, j) - 4.0 * block.at<double_t>(i, j - 1) + block.at<double_t>(i, j - 2)) / (2.0 * (Y(j) - Y(j - 1)));
    }
    else {
        res = (block.at<double_t>(i, j + 1) - block.at<double_t>(i, j - 1)) / (2.0 * (Y(j+1) - Y(j)));
    }
    return res;
}

//   4x4 -> 16x16
static double_t calIFS(cv::Mat &block, cv::Mat &s) {
    for ( int i = 0; i < N - 1; ++i ) {
        for ( int k = 0; k < N - 1; ++k ) {
            for ( int j = 0; j < M - 1; ++j ) {
                double_t a = (X(i+1) - X(i)) / (X(N-1) - X(0));
                double_t b = (X(N-1) * X(i) - X(0) * X(i+1)) / (X(N-1) - X(0));
                double_t c = (Y(j+1) - Y(j)) / (Y(M-1) - Y(0));
                double_t d = (Y(M-1) * Y(j) - Y(0) * Y(j+1)) / (Y(M-1) - Y(0));
                for ( int t = 0; t < M - 1; ++t ) {

                }
            }
        }
    }
}

cv::Mat upsampling(cv::Mat &block, bool isTexture) {
    cv::Mat s = initVerticalScalingFactor(block, isTexture);
    calIFS(block, s);
    return block;
}