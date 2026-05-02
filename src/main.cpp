#include "clock_algorithm.hpp"

cv::Mat out;
std::vector<cv::Mat> images;

int main(int argc, char **argv){

    if (argc < 2){
        printf("Uso: ./app.out <numero>\n");
        return 1;
    }

    int num = atoi(argv[1]);

    char path[100];
    sprintf(path, "../images/clock_%04d.png", num);

    cv::Mat img = cv::imread(path, cv::IMREAD_GRAYSCALE);

    if (img.empty()){
        printf("No se pudo cargar la imagen\n");
        return 1;
    }

    std::vector<cv::Mat> clocks = images_reader();

    if (clocks.empty()) {
        printf("Error: No se pudieron cargar las imagenes de la base de datos (clocks)\n");
        return 1;
    }

    int rows = img.rows;
    int cols = img.cols;

    out = cv::Mat::zeros(rows, cols, CV_8U);
    outResta = cv::Mat::zeros(rows, cols, CV_8U);
    outRestaH = cv::Mat::zeros(rows, cols, CV_8U);


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