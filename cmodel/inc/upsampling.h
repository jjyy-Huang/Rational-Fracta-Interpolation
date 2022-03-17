/*
 * @Description: upsampling.h in cmodel
 * @Author: huang
 * @Date: 3/13/22 4:03 PM
 */


#ifndef CMODEL_UPSAMPLING_H
#define CMODEL_UPSAMPLING_H

#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"

#include "math.h"


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

static double_t alphaX[4][4] = {
        0.8, 0.8, 0.8, 0.8,
        0.8, 0.8, 0.8, 0.8,
        0.8, 0.8, 0.8, 0.8,
        0.8, 0.8, 0.8, 0.8
};
static double_t betaX[4][4] = {
        0.8, 0.8, 0.8, 0.8,
        0.8, 0.8, 0.8, 0.8,
        0.8, 0.8, 0.8, 0.8,
        0.8, 0.8, 0.8, 0.8
};
static double_t gamaX[4][4] = {
        1.2, 1.2, 1.2, 1.2,
        1.2, 1.2, 1.2, 1.2,
        1.2, 1.2, 1.2, 1.2,
        1.2, 1.2, 1.2, 1.2
};
static double_t alphaY[4][4] = {
        0.2, 0.2, 0.6, 0.6,
        0.2, 0.2, 0.6, 0.6,
        0.6, 0.6, 0.6, 0.6,
        0.6, 0.6, 0.6, 0.6
};
static double_t betaY[4][4] = {
        0.2, 0.2, 0.6, 0.6,
        0.2, 0.2, 0.6, 0.6,
        0.6, 0.6, 0.6, 0.6,
        0.6, 0.6, 0.6, 0.6
};

static double_t gamaY[4][4] = {
        2.0, 2.0, 1.5, 1.5,
        2.0, 2.0, 1.5, 1.5,
        1.5, 1.5, 1.5, 1.5,
        1.5, 1.5, 1.5, 1.5
};


cv::Mat upsampling(cv::Mat &block, bool isTexture = false);


#endif //CMODEL_UPSAMPLING_H
