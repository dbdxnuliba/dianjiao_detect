#ifndef COMMONFUNC_H
#define COMMONFUNC_H
#include <opencv2/opencv.hpp>
#include <opencv2/ml.hpp>

#include <QString>

using namespace cv;
using namespace std;



double calc_max_area(const cv::Mat & src_image);
void white_balance(const cv::Mat & src_image, cv::Mat & dst_image);
void deal_with_sobel(cv::Mat & src, cv::Mat & dst);
Rect get_roi_and_draw_rect(cv::Mat & src_image, cv::Mat & dst_image, Point2f & origin, Point &left_behind_pt, Point &right_below_pt);
float get_angel(const cv::Mat & src_image);
int rotateImage2(Mat img,Mat & imgout, double degree);
void get_text_origin(const Point2f & origin, const Point &left_behind_pt, const Point &right_below_pt, Point & text_origin);
void image_threshold_filter(cv::Mat src_image, cv::Mat & dst_image);
void src_image_deal_for_calc_area(const cv::Mat & src_image, cv::Mat & dst_image);
void src_image_deal_for_calc_area_debug(const cv::Mat & src_image, cv::Mat & dst_image);
void feature_match(const Mat &src_image, Mat &dst_image);
int highlight_remove_Chi(cv::Mat* src,cv::Mat* dst,double Re);

#endif // COMMONFUNC_H
