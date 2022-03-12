/*
 * @Description: levelset.cpp in cmodel
 * @Author: huang
 * @Date: 3/11/22 6:08 PM
 */


#include "../inc/levelset.h"


//                          ┌──┐
//                       ┌──┼──┼──┐
//                       └──┼──┼──┘
//                          └──┘
//    Get the MAX or MIN of each pixel++/-- and the nearest
static void calPixel(cv::Mat* inc, cv::Mat* dec, int row, int col, int epoch){
    int a, b, c, d;
    uint16_t tmp;
    for ( int times = 1; times < epoch - 1; ++times ) {
        for ( int i = 0; i < row; ++i ) {
            for ( int j = 0; j < col; ++j ) {
                if ( i == 0 )       a = i;
                else                a = i - 1;
                if ( j == 0 )       b = j;
                else                b = j - 1;
                if ( i == row - 1 ) c = i;
                else                c = i + 1;
                if ( j == col - 1 ) d = j;
                else                d = j + 1;
                inc[times].at<int16_t>(i, j) = MAX(inc[times-1].at<int16_t>(i, j)+1,
                                                 MAX(inc[times-1].at<int16_t>(i,j),
                                                     MAX(MAX(inc[times-1].at<int16_t>(a, j), inc[times-1].at<int16_t>(i, b)),
                                                            MAX(inc[times-1].at<int16_t>(c, j), inc[times-1].at<int16_t>(i, d))))
                );
                dec[times].at<int16_t>(i, j) = MIN(dec[times-1].at<int16_t>(i, j)-1,
                                                      MIN(dec[times-1].at<int16_t>(i,j),
                                                          MIN(MIN(dec[times-1].at<int16_t>(a, j), dec[times-1].at<int16_t>(i, b)),
                                                                 MIN(dec[times-1].at<int16_t>(c, j), dec[times-1].at<int16_t>(i, d))))
                );
            }
        }
    }
}

static void calDiff(std::vector<double_t> &vec, cv::Mat *inc, cv::Mat *dec, int epoch){
    for ( int times = epoch/2 - 1; times < epoch - 1; ++times ) {
        int16_t sum = 0;
        for ( int i = 0; i < inc[0].rows; ++i ) {
            for ( int j = 0; j < inc[0].cols; ++j ) {
                sum += inc[times].at<int16_t>(i, j) - dec[times].at<int16_t>(i, j);
            }
        }
        vec.push_back(sum/(2.0*(times+1)));
    }
}

static double_t 

double_t level_set(cv::Mat &block, int epoch){
//    cv::Mat block_d(block.rows, block.cols, CV_64FC1);
//    MatFormat_Int2Double(block, block_d);
    cv::Mat increase[epoch];
    cv::Mat decrease[epoch];
    block.convertTo(increase[0], CV_16SC1);
    block.convertTo(decrease[0], CV_16SC1);
    for ( int i = 1; i < epoch; ++i ) {
        increase[i] = cv::Mat::zeros(block.rows, block.cols, CV_16SC1);
        decrease[i] = cv::Mat::zeros(block.rows, block.cols, CV_16SC1);
    }

    calPixel(increase, decrease, block.rows, block.cols, epoch);

    std::vector<double_t> sum;
    calDiff(sum, increase, decrease, epoch);


}