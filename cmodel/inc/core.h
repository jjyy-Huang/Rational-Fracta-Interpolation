/*
 * @Description: core.h in cmodel
 * @Author: huang
 * @Date: 3/11/22 3:32 PM
 */


#ifndef CMODEL_CORE_H
#define CMODEL_CORE_H

#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/opencv.hpp"

#include "../inc/imgproc.h"
#include "../inc/levelset.h"
#include "../inc/upsampling.h"

cv::Mat core(cv::Mat &img, int height, int width);

#endif //CMODEL_CORE_H
