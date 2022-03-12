/*
 * @Description: levelset.h in cmodel
 * @Author: huang
 * @Date: 3/11/22 6:08 PM
 */


#ifndef CMODEL_LEVELSET_H
#define CMODEL_LEVELSET_H


#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"

#include "myalgorithm.h"
#include "math.h"

double_t level_set(cv::Mat &block, int epoch);

#endif //CMODEL_LEVELSET_H
