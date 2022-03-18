/*
 * @Description: core.cpp in cmodel
 * @Author: huang
 * @Date: 3/11/22 3:32 PM
 */


#include "../inc/core.h"

cv::Mat core(cv::Mat &img, int height, int width, int scale){
    cv::Mat img_sr = cv::Mat::zeros(height, width, CV_8UC1);
//    divide to 5x5 block
    for ( int i = 0; i < height/scale; i += scale/2) {
        for ( int j = 0; j < width/scale; j += scale/2 ) {
            cv::Mat block = cutBlock(img, i, j, 5, 5);

            double_t isTexture;
//            isTexture = level_set(block, 101);
            cv::Mat block_sr = upsampling(block, true);
            mergeBlock(block_sr, img_sr, i*scale, j*scale, 2*scale, 2*scale);
        }
    }
    return img_sr;
}
