#ifndef BOARD_A_FUNCTION_H
#define BOARD_A_FUNCTION_H

#include <opencv2/opencv.hpp>
#include <opencv2/ml.hpp>

#include <QString>

#include "commonFunc.h"
using namespace cv;
using namespace std;

class Board_A_detec
{
public:
    explicit Board_A_detec();
    ~Board_A_detec();
    typedef struct _Board_A_threshold_para{
        vector<double> threshold_JALVC164245;
        vector<double> threshold_GH137S;
        vector<double> threshold_JDSPF28335;
        vector<double> threshold_SM1040;
        vector<double> threshold_J16H281;
        vector<double> threshold_SM490;
        vector<double> threshold_100;
    }Board_A_area_item;

    unsigned int get_board_A_res(cv::Mat & src_image, Point2f & origin);
    int read_numbers(const cv::Mat & src_imag, QString & num, Point2f & origin);
    void set_threshold(const Board_A_detec::Board_A_area_item & para);
    void get_threshold(Board_A_detec::Board_A_area_item & para);
    void get_area(Board_A_detec::Board_A_area_item & para);
    void set_num_template(const vector<Mat> &template_vec);

private:
    QString get_numbers_area(const cv::Mat & src_image, cv::Mat & dst_image, Point2f & origin);
    void numbers_identify(const cv::Mat & src_image, QString & num_string);
    unsigned int get_JALVC164245_res(cv::Mat & src_image, Point2f & origin);
    unsigned int get_GH137S_res(cv::Mat & src_image, Point2f & origin);
    unsigned int get_JDSPF28335_res(cv::Mat & src_image, Point2f & origin);
    unsigned int get_SM1040_res(cv::Mat & src_image, Point2f & origin);
    unsigned int get_J16H281_res(cv::Mat & src_image, Point2f & origin);
    unsigned int get_SM490_res(cv::Mat & src_image, Point2f & origin);
    unsigned int get_100_res(cv::Mat & src_image, Point2f & origin);
    void src_image_deal_with_red_ch(const cv::Mat & src_image, cv::Mat & dst_image);
    void image_deal_for_num_read(cv::Mat src_image, cv::Mat & dst_image);
    int read_num(cv::Mat & src_image);
private:
    Board_A_area_item m_threshold_para;
    Board_A_area_item m_area_detec;         //测量得到的面积
    vector<Rect> m_rect_vec;
    vector<Mat> m_num_template_vec;
};


#endif // BOARD_A_FUNCTION_H
