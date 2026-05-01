#include <opencv2/opencv.hpp> 
#include <stdlib.h> 
#include <utility>
#include <math.h>

cv::Mat out;
cv::Mat outResta;
cv::Mat outRestaH;

std::pair<int,int> get_center(const cv::Mat& matrix);
std::pair<int,int> get_longer(const cv::Mat& matrix, const std::pair<int,int>& center);
float angulo_calcular( const std::pair<int,int>& longer, const std::pair<int,int>& center);
void remove_angle(float angulo,const cv::Mat& matrix, const std::pair<int,int>& center);

#define NUMCLOCKS 1401
#define WHITE 255
#define UMBRAL 50
#define UMBRAL_MANECILLA 50


int main(int argc, char **argv){

    if (argc < 2){
        printf("Uso: ./app.out <numero>\n");
        return 1;
    }

    int num = atoi(argv[1]);

    char path[100];
    sprintf(path, "./images/data-resampled/clock_%04d.png", num);

    cv::Mat img = cv::imread(path, cv::IMREAD_GRAYSCALE);

    if (img.empty()){
        printf("No se pudo cargar la imagen\n");
        return 1;
    }

    std::vector<cv::Mat> clocks;

    for (int i = 200; i < NUMCLOCKS; i++)
    {
        char pathn[100];

        if (i < 1000)
            sprintf(pathn,"./images/data-resampled/clock_0%d.png",i);
        else
            sprintf(pathn,"./images/data-resampled/clock_%d.png",i);

        cv::Mat img = cv::imread(pathn, cv::IMREAD_GRAYSCALE);

        if (!img.empty()){
            clocks.push_back(img);
        }
    }

    
    int rows = img.rows;
    int cols = img.cols;

    out = cv::Mat(rows, cols, CV_8U);
    outResta = cv::Mat(rows, cols, CV_8U);
    outRestaH = cv::Mat(rows, cols, CV_8U);


    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){
            int sum = 0;

            for (int k = 0; k < clocks.size(); k++)
                sum += clocks[k].at<uchar>(i,j);

            out.at<uchar>(i,j) = sum / clocks.size();
        }
    }

    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){

            uchar a = out.at<uchar>(i,j);
            uchar b = img.at<uchar>(i,j);

            uchar d = abs(a - b);

            outResta.at<uchar>(i,j) = (d > UMBRAL) ? 255 : 0;
        }
    }

    std::pair<int,int> center = get_center(outResta); 
    std::pair<int,int> longer = get_longer(outResta, center);

    float angulo= angulo_calcular(longer,center);

    int minutos = round(angulo / 6.0);

    printf(" %f\n %d,%d\n %d \n", angulo, longer.first, longer.second, minutos);

    remove_angle(angulo,outResta,center);
    std::pair<int,int> longerH = get_longer(outRestaH, center);

    float anguloH= angulo_calcular(longerH,center);

    int hora = floor(anguloH / 30.0);


    printf(" %f\n %d,%d\n %d\n", anguloH, longerH.first, longerH.second, hora);

    printf(" Hora: %d:%d \n", hora,minutos);
    

    cv::imshow("Original", img);
    cv::imshow("Media", out);
    cv::imshow("Resta", outResta);
    cv::imshow("RestaH", outRestaH);


    cv::waitKey(0);
    return EXIT_SUCCESS;
}

std::pair<int, int> get_center(const cv::Mat &matrix){
    return std::pair<int,int>(416,253);
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
