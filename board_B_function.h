#ifndef BOARD_B_FUNCTION_H
#define BOARD_B_FUNCTION_H

#include <opencv2/opencv.hpp>
#include <opencv2/ml.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <QString>

#include "commonFunc.h"
using namespace cv;
using namespace std;

class Board_B_detec
{
public:
    explicit Board_B_detec();
    ~Board_B_detec();
    typedef struct _Board_B_threshold_para{
        vector<double> threshold_GH137S;
        vector<double> threshold_SM1040;
        vector<double> threshold_JT6H281;
        vector<double> threshold_121;
        vector<double> threshold_100;
    }Board_B_area_item;

    unsigned int get_board_B_res(cv::Mat & src_image, Point2f & origin);
    void set_threshold(const Board_B_detec::Board_B_area_item & para);
    void get_area(Board_B_detec::Board_B_area_item & para);

private:
    unsigned int get_B_GH137S_res(cv::Mat & src_image, Point2f & origin);
    unsigned int get_B_SM1040_res(cv::Mat & src_image, Point2f & origin);
    unsigned int get_B_JT6H281_res(cv::Mat & src_image, Point2f & origin);
    unsigned int get_B_121_res(cv::Mat & src_image, Point2f & origin);
    unsigned int get_B_100_res(cv::Mat & src_image, Point2f & origin);
private:
    Board_B_area_item m_threshold_para;     //面积阈值设定值
    Board_B_area_item m_area_detec;         //测量得到的面积
    cv::Mat m_image_src;
    vector<Rect> m_rect_vec;
};

#endif // BOARD_B_FUNCTION_H
