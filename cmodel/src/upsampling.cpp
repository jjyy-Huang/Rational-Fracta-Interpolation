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

#define THETA(n)    ((X(n) - X(0)) / (X(N-1) - X(0)))
#define ETA(n)      ((Y(n) - Y(0)) / (Y(M-1) - Y(0)))

#define ALPHAX(x,y)     (alphaX[x][y])
#define BETAX(x,y)      (betaX[x][y])
#define GAMAX(x,y)      (gamaX[x][y])
#define ALPHAY(x,y)     (alphaY[x][y])
#define BETAY(x,y)      (betaY[x][y])
#define GAMAY(x,y)      (gamaY[x][y])

#define SQR(n)          (pow(n,2))

#define MAT(b, i, j)    (b.at<double_t>(i,j))

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
//            s.at<double_t>(patch_x*2 + i, patch_y*2 + j) = s_init[patch_x*2][patch_y*2] *
//                    abs(block.at<double_t>(patch_x*2 + i*2, patch_y*2 + j*2) - aver) / sum;
            s.at<double_t>(patch_x*2 + i, patch_y*2 + j) = s_init[patch_x*2][patch_y*2];
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

static void calIFS(double_t &a, double_t &b, double_t &c, double_t &d, int i, int j){
//              here can be optimized as a constant matrix
    a = (X(i+1) - X(i)) / (X(N-1) - X(0));
    b = (X(N-1) * X(i) - X(0) * X(i+1)) / (X(N-1) - X(0));
    c = (Y(j+1) - Y(j)) / (Y(M-1) - Y(0));
    d = (Y(M-1) * Y(j) - Y(0) * Y(j+1)) / (Y(M-1) - Y(0));
}

//   4x4 -> 16x16
static cv::Mat calFIFs(cv::Mat &z, cv::Mat &s) {
    int flag = 0;
    cv::Mat fz = cv::Mat::zeros(17, 17, CV_64FC1);
    for ( int i = 0; i < N - 1; ++i ) {
        for ( int k = 0; k < N - 1; ++k ) {
            for ( int j = 0; j < M - 1; ++j ) {
                double_t a, b, c, d;
                calIFS(a, b, c, d, i, j);
                for ( int t = 0; t < M - 1; ++t ) {
                    double_t w = (SQR(1 - THETA(k)) * ALPHAX(i, j) + THETA(k) * (1 - THETA(k)) * GAMAX(i, j) + SQR(THETA(k)) * BETAX(i, j)) *
                                    (SQR(1 - ETA(t)) * ALPHAY(i, j) + ETA(t) * (1 - ETA(t)) * GAMAY(i, j) + SQR(ETA(t)) * BETAY(i, j));

                    double_t a00 = (SQR(1 - THETA(k)) * (ALPHAX(i, j) + THETA(k) * GAMAX(i, j))  * SQR(1 - ETA(t)) * (ALPHAY(i, j) + ETA(t) * GAMAY(i, j))) / w;
                    double_t a01 = (SQR(1 - THETA(k)) * (ALPHAX(i, j) + THETA(k) * GAMAX(i, j))  * SQR(ETA(t)) * (BETAY(i, j) + (1 - ETA(t)) * GAMAY(i, j))) / w;
                    double_t a10 = (SQR(THETA(k)) * (BETAX(i, j) + (1 - THETA(k)) * GAMAX(i, j)) * SQR(1 - ETA(t)) * (ALPHAY(i, j) + ETA(t) * GAMAY(i, j))) / w;
                    double_t a11 = (SQR(THETA(k)) * (BETAX(i, j) + (1 - THETA(k)) * GAMAX(i, j)) * SQR(ETA(t)) * (BETAY(i, j) + (1 - ETA(t)) * GAMAY(i, j))) / w;

                    double_t b00 =  (THETA(k) * SQR(1- THETA(k)) * ALPHAX(i, j)   * SQR(1 - ETA(t)) * (ALPHAY(i, j) + ETA(t) * GAMAY(i, j))) / w;
                    double_t b01 =  (THETA(k) * SQR(1- THETA(k)) * ALPHAX(i, j)   * SQR(ETA(t)) * (BETAY(i, j) + (1 - ETA(t)) * GAMAY(i, j))) / w;
                    double_t b10 = -(SQR(THETA(k)) * (1 - THETA(k)) * BETAX(i, j) * SQR(1 - ETA(t)) * (ALPHAY(i, j) + ETA(t) * GAMAY(i, j))) / w;
                    double_t b11 = -(SQR(THETA(k)) * (1 - THETA(k)) * BETAX(i, j) * SQR(ETA(t)) * (BETAY(i, j) + (1 - ETA(t)) * GAMAY(i, j))) / w;

                    double_t c00 =  (SQR(1 - THETA(k)) * (ALPHAX(i, j) + THETA(k)  * GAMAX(i, j))  * ETA(t) * SQR(1 - ETA(t)) * ALPHAY(i, j)) / w;
                    double_t c01 = -(SQR(1 - THETA(k)) * (ALPHAX(i, j) + THETA(k)  * GAMAX(i, j))  * SQR(ETA(t)) * (1 - ETA(t)) * BETAY(i, j)) / w;
                    double_t c10 =  (SQR(THETA(k)) * (BETAX(i, j) + (1 - THETA(k)) * GAMAX(i, j)) * ETA(t) * SQR(1 - ETA(t)) * ALPHAY(i, j)) / w;
                    double_t c11 = -(SQR(THETA(k)) * (BETAX(i, j) + (1 - THETA(k)) * GAMAX(i, j)) * SQR(ETA(t)) * (1 - ETA(t)) * BETAY(i, j)) / w;

                    fz.at<double_t>(i * 4 + k, j * 4 + t) = MAT(s, i, j) * MAT(z, k, t)
                            + a00 * (MAT(z, i, j) - MAT(s, i, j) * MAT(z, 0, 0))
                              + a10 * (MAT(z, i+1, j) - MAT(s, i, j) * MAT(z, N-1, 0))
                                + a01 * (MAT(z, i, j+1) - MAT(s, i, j) * MAT(z, 0, N-1))
                                  + a11 * (MAT(z, i+1, j+1) - MAT(s, i, j) * MAT(z, N-1, N-1))
                            + b00 * ((X(i+1) - X(i)) * dx(z, i, j, N) - MAT(s, i, j) * (X(N-1) - X(0)) * dx(z, 0, 0, N))
                              + b10 * ((X(i+1) - X(i)) * dx(z, i+1, j, N) - MAT(s, i, j) * (X(N-1) - X(0)) * dx(z, N-1, 0, N))
                                + b01 * ((X(i+1) - X(i)) * dx(z, i, j+1, N) - MAT(s, i, j) * (X(N-1) - X(0)) * dx(z, 0, N-1, N))
                                     + b11 * ((X(i+1) - X(i)) * dx(z, i+1, j+1, N) - MAT(s, i, j) * (X(N-1) - X(0)) * dx(z, N-1, N-1, N))
                            + c00 * ((Y(j+1) - Y(j)) * dy(z, i, j, M) - MAT(s, i, j) * (X(M-1) - X(0)) * dy(z, 0, 0, M))
                              + c10 * ((Y(j+1) - Y(j)) * dy(z, i+1, j, M) - MAT(s, i, j) * (X(M-1) - X(0)) * dy(z, M-1, 0, M))
                                + c01 * ((Y(j+1) - Y(j)) * dy(z, i, j+1, M) - MAT(s, i, j) * (X(M-1) - X(0)) * dy(z, 0, M-1, M))
                                  + c11 * ((Y(j+1) - Y(j)) * dy(z, i+1, j+1, M) - MAT(s, i, j) * (X(M-1) - X(0)) * dy(z, M-1, M-1, M));

                }
            }
        }
    }
    return fz;
}

cv::Mat upsampling(cv::Mat &block, bool isTexture) {
    cv::Mat s = initVerticalScalingFactor(block, isTexture);
    cv::Mat fz = calFIFs(block, s);
    return fz;
}