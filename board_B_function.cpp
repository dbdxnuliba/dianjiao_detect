#include "board_B_function.h"
#include <iostream>


/**
 * @brief Board_B_detec::Board_B_detec
 * //构造函数，其中主要完成设置面积检测初始阈值
 */
Board_B_detec::Board_B_detec()
{
}

Board_B_detec::~Board_B_detec()
{

}

/**
 * @brief Board_B_detec::get_board_B_res
 * //根据输入的标准图像和坐标原点来对各区域进行检测
 * @param src_image
 * //经过放射变换后的图像
 * @param origin
 * //相对坐标原点
 * @return
 * //0,OK;-1,NG
 */
unsigned int Board_B_detec::get_board_B_res(Mat &src_image, Point2f &origin)
{
    unsigned int res[5] = {0};
    m_area_detec.threshold_100.clear();
    m_area_detec.threshold_121.clear();
    m_area_detec.threshold_GH137S.clear();
    m_area_detec.threshold_JT6H281.clear();
    m_area_detec.threshold_SM1040.clear();

    m_rect_vec.clear();
    res[0] = get_B_GH137S_res(src_image, origin);
    res[1] = get_B_JT6H281_res(src_image, origin);
    res[2] = get_B_SM1040_res(src_image, origin);
    res[3] = get_B_121_res(src_image, origin);
    res[4] = get_B_100_res(src_image, origin);

    for (unsigned int i = 0; i < m_rect_vec.size(); i ++){
        rectangle(src_image, m_rect_vec.at(i), Scalar(0, 0, 255),2, LINE_8,0);  //图像上画出矩形框
        Point origin_text = Point(m_rect_vec.at(i).x, m_rect_vec.at(i).y);
        putText(src_image, QString::number(i+1).toStdString(), origin_text, cv::FONT_HERSHEY_SIMPLEX, 2, Scalar(255,0,0), 2);
    }

    unsigned int ret = 0;
    ret |= (res[0] & 0x1f);        //5
    ret |= (res[1] & 0xff) << 5;   //2
    ret |= (res[2] & 0x1f) << 7;   //2
    ret |= (res[3] & 0x1f) << 9;   //2
    ret |= (res[4] & 0x1f) << 11;   //2

    return ret;
}

/**
 * @brief Board_B_detec::set_threshold
 * //设置面积检测阈值
 * @param para
 */
void Board_B_detec::set_threshold(const Board_B_detec::Board_B_area_item &para)
{
    m_threshold_para = para;
}

/**
 * @brief Board_B_detec::get_area
 * //获取各区域检测面积
 * @param para
 */
void Board_B_detec::get_area(Board_B_detec::Board_B_area_item &para)
{
    para = m_area_detec;
}


unsigned int Board_B_detec::get_B_GH137S_res(Mat &src_image, Point2f &origin)
{
    Mat roi_image;
    Mat canny_image;
    double maxArea = 0;
    unsigned int res = 0;

    Point left_behind_pt_arr[] = {Point(-50, 930), Point(90, 930), Point(260, 930), Point(690, 1170), Point(860, 1170)};
    Point right_below_pt_arr[] = {Point(50, 1130), Point(220, 1130), Point(390, 1130), Point(790, 1380), Point(970, 1380)};

//    get_roi_and_draw_rect(src_image, roi_image, origin, left_behind_pt_arr[1], right_below_pt_arr[1]);
//    src_image_deal_for_calc_area(roi_image, canny_image);
//    maxArea = calc_max_area(canny_image);
//
//    return 0;

    for (int i = 0; i < 5; i ++){
        Rect rect = get_roi_and_draw_rect(src_image, roi_image, origin, left_behind_pt_arr[i], right_below_pt_arr[i]);
        src_image_deal_for_calc_area(roi_image, canny_image);
        maxArea = calc_max_area(canny_image);
        m_area_detec.threshold_GH137S.push_back(maxArea);
        m_rect_vec.push_back(rect);

        if (maxArea < m_threshold_para.threshold_GH137S[i]) (res |= (1<<i));
    }

    return res;
}

unsigned int Board_B_detec::get_B_SM1040_res(Mat &src_image, Point2f &origin)
{
    Mat roi_image;
    Mat canny_image;
    double maxArea = 0;
    unsigned int res = 0;

    Point left_behind_pt_arr[] = {Point(50, 1290), Point(200, 1290)};
    Point right_below_pt_arr[] = {Point(150, 1450), Point(300, 1450)};

//    get_roi_and_draw_rect(src_image, roi_image, origin, left_behind_pt_arr[0], right_below_pt_arr[0]);
//    src_image_deal_for_calc_area(roi_image, canny_image);
//    maxArea = calc_max_area(canny_image);
//
//    return 0;

    for (int i = 0; i < 2; i ++){
        Rect rect = get_roi_and_draw_rect(src_image, roi_image, origin, left_behind_pt_arr[i], right_below_pt_arr[i]);
        src_image_deal_for_calc_area(roi_image, canny_image);
        maxArea = calc_max_area(canny_image);

        m_area_detec.threshold_SM1040.push_back(maxArea);
        m_rect_vec.push_back(rect);
        if (maxArea < m_threshold_para.threshold_SM1040[i]) (res |= (1<<i));
    }

    return res;
}

unsigned int Board_B_detec::get_B_JT6H281_res(Mat &src_image, Point2f &origin)
{
    Mat roi_image;
    Mat canny_image;
    double maxArea = 0;
    unsigned int res = 0;

    Point left_behind_pt_arr[] = {Point(340, 1190), Point(620, 1190)};
    Point right_below_pt_arr[] = {Point(480, 1350), Point(730, 1350)};

//    get_roi_and_draw_rect(src_image, roi_image, origin, left_behind_pt_arr[1], right_below_pt_arr[1]);
//    src_image_deal_for_calc_area(roi_image, canny_image);
//    maxArea = calc_max_area(canny_image);
//
//    return 0;

    for (int i = 0; i < 2; i ++){
        Rect rect = get_roi_and_draw_rect(src_image, roi_image, origin, left_behind_pt_arr[i], right_below_pt_arr[i]);
        src_image_deal_for_calc_area(roi_image, canny_image);
        maxArea = calc_max_area(canny_image);

        m_area_detec.threshold_JT6H281.push_back(maxArea);
        m_rect_vec.push_back(rect);
        if (maxArea < m_threshold_para.threshold_JT6H281[i]) (res |= (1<<i));
    }

    return res;
}

unsigned int Board_B_detec::get_B_121_res(Mat &src_image, Point2f &origin)
{
    Mat roi_image;
    Mat canny_image;
    double maxArea = 0;
    unsigned int res = 0;

    Point left_behind_pt_arr[] = {Point(950, 1350), Point(950, 1440)};
    Point right_below_pt_arr[] = {Point(1130, 1430), Point(1130, 1520)};

//    get_roi_and_draw_rect(src_image, roi_image, origin, left_behind_pt_arr[1], right_below_pt_arr[1]);
//    src_image_deal_for_calc_area(roi_image, canny_image);
//    maxArea = calc_max_area(canny_image);
//
//    return 0;

    for (int i = 0; i < 2; i ++){
        Rect rect = get_roi_and_draw_rect(src_image, roi_image, origin, left_behind_pt_arr[i], right_below_pt_arr[i]);
        src_image_deal_for_calc_area(roi_image, canny_image);
        maxArea = calc_max_area(canny_image);

        m_area_detec.threshold_121.push_back(maxArea);
        m_rect_vec.push_back(rect);
        if (maxArea < m_threshold_para.threshold_121[i]) (res |= (1<<i));
    }

    return res;
}

unsigned int Board_B_detec::get_B_100_res(Mat &src_image, Point2f &origin)
{
    Mat roi_image;
    Mat canny_image;
    double maxArea = 0;
    unsigned int res = 0;

    Point left_behind_pt_arr[] = {Point(1750, 1520), Point(1750, 1620)};
    Point right_below_pt_arr[] = {Point(1950, 1600), Point(1950, 1700)};

//    get_roi_and_draw_rect(src_image, roi_image, origin, left_behind_pt_arr[0], right_below_pt_arr[0]);
//    src_image_deal_for_calc_area(roi_image, canny_image);
//    maxArea = calc_max_area(canny_image);
//
//    return 0;

    for (int i = 0; i < 2; i ++){
        Rect rect = get_roi_and_draw_rect(src_image, roi_image, origin, left_behind_pt_arr[i], right_below_pt_arr[i]);
        src_image_deal_for_calc_area(roi_image, canny_image);
        maxArea = calc_max_area(canny_image);

        m_area_detec.threshold_100.push_back(maxArea);
        m_rect_vec.push_back(rect);
        if (maxArea < m_threshold_para.threshold_100[i]) (res |= (1<<i));
    }

    return res;
}
