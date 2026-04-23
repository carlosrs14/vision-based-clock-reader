#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>
#include <chrono>

#define N_IMG 1401

int main(int argc, char **argv) {
    // cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_ERROR);
    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_SILENT);
    std::vector<cv::Mat> images = std::vector<cv::Mat>();

    auto start_clock = std::chrono::steady_clock::now();

    // abrir imagenes
    for (int i = 0; i < N_IMG; i++) {
        char pathnew [30];
        if (i < 999) {
            sprintf(pathnew, "./images/clock_0%d.png", i);
        } else {
            sprintf(pathnew, "./images/clock_%d.png", i);
        }
        cv::Mat img = cv::imread(pathnew, cv::IMREAD_GRAYSCALE);
        
        if (!img.empty()) {
            images.push_back(img);
        }
    }
    auto open_img_clock = std::chrono::steady_clock::now();

    cv::Size size = images[0].size();
    int rows = size.height;
    int cols = size.width;
    int n_img = images.size();
    cv::Mat media = cv::Mat(rows, cols, CV_8U);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int sum = 0;
            for (int k = 0; k < n_img; k++) {
                int value = images[k].at<uchar>(i, j); 
                sum += value;
            }
            media.at<uchar>(i, j) = (unsigned char) (sum / n_img);
        }
    }
    auto media_clock = std::chrono::steady_clock::now();

    cv::Mat out = cv::Mat(rows, cols, CV_8U);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            out.at<uchar>(i, j) = images[0].at<uchar>(i, j) - media.at<uchar>(i, j);
        }
    }
    auto rest_clock = std::chrono::steady_clock::now();
    

    printf("rows %d, cols %d, imgs %d \n", rows, cols, n_img);
    auto duration_total = std::chrono::duration_cast<std::chrono::milliseconds>(rest_clock - media_clock);
    std::cout << "Tiempo de resta: " << duration_total.count() << " ms" << std::endl;

    auto duration_media = std::chrono::duration_cast<std::chrono::milliseconds>(media_clock - open_img_clock);
    std::cout << "Tiempo de sacando el promedio: " << duration_media.count() << " ms" << std::endl;

    auto duration_open = std::chrono::duration_cast<std::chrono::milliseconds>(open_img_clock - start_clock);
    std::cout << "Tiempo abriendo: " << duration_open.count() << " ms" << std::endl;

    cv::imshow("Imagen", images[0]);
    cv::imshow("Promedio de imagenes", media);
    cv::imshow("Resta de imagenes", out);
    cv::waitKey(0);

    return EXIT_SUCCESS;
}