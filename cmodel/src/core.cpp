/*
 * @Description: core.cpp in cmodel
 * @Author: huang
 * @Date: 3/11/22 3:32 PM
 */


#include "../inc/core.h"

cv::Mat core(cv::Mat &img){
//    divide to 5x5 block
    for ( int i = 0; i < img.rows - 4; i += 4) {
        for ( int j = 0; j < img.cols - 4; j += 4 ) {
            cv::Mat block = img(cv::Rect(i, j, 5, 5));

            double_t isTexture;
            isTexture = level_set(block, 101);
        }
    }
    return img;
}
