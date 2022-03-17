/*
 * @Description: core.cpp in cmodel
 * @Author: huang
 * @Date: 3/11/22 3:32 PM
 */


#include "../inc/core.h"

cv::Mat core(cv::Mat &img){
    cv::Mat img_sr = cv::Mat::zeros(2160, 3840, CV_8UC1);
//    divide to 5x5 block
    for ( int i = 0; i < img.rows - 1; i += 4) {
        for ( int j = 0; j < img.cols - 1; j += 4 ) {
            cv::Mat block = cutBlock(img, i, j, 5, 5);

            double_t isTexture;
//            isTexture = level_set(block, 101);
            cv::Mat block_sr = upsampling(block, true);
            mergeBlock(block_sr, img_sr, i*4, j*4, 16, 16);
        }
    }
    return img_sr;
}
