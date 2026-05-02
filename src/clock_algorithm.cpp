#include "clock_algorithm.hpp"
#include <utility>
#include <math.h>

cv::Mat outResta;
cv::Mat outRestaH;

std::vector<cv::Mat> images_reader() {

    std::vector<cv::Mat> clocks;

    for (int i = 200; i < NUMCLOCKS; i++)
    {
        char pathn[100];
        sprintf(pathn,"../images/clock_%04d.png",i);

        cv::Mat img = cv::imread(pathn, cv::IMREAD_GRAYSCALE);

        if (!img.empty()){
            clocks.push_back(img);
        }
    }
    return clocks;
}

std::pair<int, int> get_center(const cv::Mat &matrix)
{
    return std::pair<int,int>(416, 253);
}

std::pair<int, int> get_longer(const cv::Mat &matrix, const std::pair<int, int> &center){

    int rows = matrix.rows;
    int cols = matrix.cols;

    double max_distance = 0; 
    std::pair<int,int> max_point = {0,0};

    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){

            if (matrix.at<uchar>(i,j) == WHITE){

                double dx = j - center.first;
                double dy = i - center.second;

                double dist = sqrt(pow(dx,2) + pow(dy,2));

                if (dist > max_distance){
                    max_distance = dist;
                    max_point = std::pair<int,int>(j, i); 
                }
            }
        }
    }

    return max_point;
}

float angulo_calcular(const std::pair<int, int> &longer, const std::pair<int, int> &center){
    float dx = longer.first - center.first;
    float dy = center.second - longer.second; 

    float angulo = atan2(dy, dx) * 180.0 / M_PI;

    if (angulo < 0) angulo += 360;
    angulo = 90 - angulo;
    if (angulo < 0) angulo += 360;

    return angulo;
}

void remove_angle(float angulo, const cv::Mat &matrix, const std::pair<int, int> &center)
{
    int rows = matrix.rows;
    int cols = matrix.cols;

    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){

            if (matrix.at<uchar>(i,j) == 255){

                std::pair posPix(j,i);
                float ang= angulo_calcular(posPix,center);

                float diff = abs(ang - angulo);
                if (diff > 180) diff = 360 - diff;

                if (diff <= 12.0){
                    outRestaH.at<uchar>(i,j) = 0;
                }else{
                    outRestaH.at<uchar>(i,j) = 255;

                }
                    
            }
        }
    }
}
