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

cv::Mat upsampling(cv::Mat &block, bool isTexture = false);


#endif //CMODEL_UPSAMPLING_H
