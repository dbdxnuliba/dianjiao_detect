#ifndef BOARDCTRL_H
#define BOARDCTRL_H

#include "board_A_function.h"
#include "board_B_function.h"
#include "commonFunc.h"

#include "QObject"
using namespace cv;
using namespace std;

class BoardCtrl:public QObject
{
    Q_OBJECT

public:
    explicit BoardCtrl();
    ~BoardCtrl();
    unsigned int get_board_res(int &boadr_info, QString & board_num, Mat & image_mark);
    int get_board_A_area(Board_A_detec::Board_A_area_item & area_info);
    int get_board_B_area(Board_B_detec::Board_B_area_item & area_info);
    void set_board_A_area(const Board_A_detec::Board_A_area_item & area_info);
    void set_board_B_area(const Board_B_detec::Board_B_area_item & area_info);
    void set_match_template(const vector<cv::Mat> & template_src);
    void set_num_template(const vector<cv::Mat> & template_src);
    void set_rotate_template(const cv::Mat & image_src);
    void set_rotate_template_A(const cv::Mat & image_src);
    void set_image_src(const cv::Mat & image_src);
    void set_best_radius(int r);
private:
    int board_B_pre_deal(Mat &src_image, Mat &dst_image, Point2f &origin);
    int board_A_pre_deal(Mat &src_image, Mat &dst_image, Point2f &origin);
    void board_B_src_rotate(const Mat &src_image, Mat &dst_image);
    void board_A_src_rotate(const Mat &src_image, Mat &dst_image);
    void get_circles_pos(const cv::Mat & src_image, std::vector<cv::Vec3f> & circles);
    void arrange_circle(const std::vector<cv::Vec3f> & input_circle, std::vector<cv::Vec3f> & out_circle);
    void matcher(Mat researchImg, Mat tmpImg, Point & center);
    void Image_fileter(Mat resImg, Mat &output_img);
    void image_fileter_for_rotate(Mat resImg, Mat &output_img);
    void image_fileter_for_match(Mat resImg, Mat &output_img);
private:
    Board_A_detec m_board_A;
    Board_B_detec m_board_B;
    cv::Mat m_template_arr[4];              //template
    cv::Mat m_image_src;
    cv::Mat m_src_template;
    cv::Mat m_src_template_A;
    double m_temp_ratio;            //模板缩放比例
    int m_radius;       //最佳半径
};

#endif // BOARDCTRL_H
