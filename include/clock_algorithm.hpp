#include <vector>
#include <opencv2/opencv.hpp>

#define NUMCLOCKS 1401
#define WHITE 255
#define UMBRAL 50
#define UMBRAL_MANECILLA 50

extern cv::Mat outResta;
extern cv::Mat outRestaH;

std::vector<cv::Mat> images_reader();
std::pair<int,int> get_center(const cv::Mat& matrix);
std::pair<int,int> get_longer(const cv::Mat& matrix, const std::pair<int,int>& center);
float angulo_calcular( const std::pair<int,int>& longer, const std::pair<int,int>& center);
void remove_angle(float angulo,const cv::Mat& matrix, const std::pair<int,int>& center);