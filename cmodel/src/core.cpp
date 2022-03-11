/*
 * @Description: core.cpp in cmodel
 * @Author: huang
 * @Date: 3/11/22 3:32 PM
 */


#include "../inc/core.h"
#include "../inc/imgproc.h"

cv::Mat core(cv::Mat &img){
//    divide to 5x5 block
    for ( int i = 0; i < img.rows - 4; i += 4) {
        for ( int j = 0; j < img.cols - 4; j += 4 ) {
            cv::Mat block = img(cv::Rect(i, j, 5, 5));
            level_set(block);
        }
    }
    return img;
}
