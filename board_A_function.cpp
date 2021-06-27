#include "board_A_function.h"
#include "commonFunc.h"

/**
 * @brief Board_B_detec::Board_B_detec
 * //构造函数，其中主要完成设置面积检测初始阈值
 */
Board_A_detec::Board_A_detec()
{
}

Board_A_detec::~Board_A_detec()
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
unsigned int Board_A_detec::get_board_A_res(Mat &src_image, Point2f &origin)
{
    cv::Mat JALVC164245_imag;
    cv::Mat GH137S_imag;
    cv::Mat JDSPF28335_imag;
    unsigned int res[7] = {0};
    unsigned int ret = 0;

    m_area_detec.threshold_JALVC164245.clear();
    m_area_detec.threshold_JDSPF28335.clear();
    m_area_detec.threshold_GH137S.clear();
    m_area_detec.threshold_SM1040.clear();
    m_area_detec.threshold_J16H281.clear();
    m_area_detec.threshold_SM490.clear();
    m_area_detec.threshold_100.clear();

    m_rect_vec.clear();
    res[0] = get_JALVC164245_res(src_image, origin);
    res[1] = get_GH137S_res(src_image, origin);
    res[2] = get_JDSPF28335_res(src_image, origin);
    res[3] = get_SM1040_res(src_image, origin);
    res[4] = get_J16H281_res(src_image, origin);
    res[5] = get_SM490_res(src_image, origin);
    res[6] = get_100_res(src_image, origin);

    ret |= (res[0] & 0x1f);         //5
    ret |= (res[1] & 0x1f) << 5;    //5
    ret |= (res[2] & 0xff) << 10;   //8
    ret |= (res[3] & 0x1f) << 18;   //2
    ret |= (res[4] & 0x1f) << 20;   //2
    ret |= (res[5] & 0x1f) << 22;   //2
    ret |= (res[6] & 0x1f) << 24;   //2

    for (unsigned int i = 0; i < m_rect_vec.size(); i ++){
        rectangle(src_image, m_rect_vec.at(i), Scalar(0, 0, 255),2, LINE_8,0);  //图像上画出矩形框
        Point origin_text = Point(m_rect_vec.at(i).x, m_rect_vec.at(i).y);
        putText(src_image, QString::number(i+1).toStdString(), origin_text, cv::FONT_HERSHEY_SIMPLEX, 2, Scalar(255,0,0), 2);
    }
    return ret;
}

int Board_A_detec::read_numbers(const Mat &src_imag, QString &num, Point2f & origin)
{
    Mat image_num;
    num = get_numbers_area(src_imag, image_num, origin);
    //numbers_identify(image_num, num);
    return 0;
}

/**
 * @brief Board_B_detec::set_threshold
 * //设置面积检测阈值
 * @param para
 */
void Board_A_detec::set_threshold(const Board_A_detec::Board_A_area_item &para)
{
    m_threshold_para = para;
}

void Board_A_detec::get_threshold(Board_A_detec::Board_A_area_item &para)
{
    para = m_threshold_para;
}

/**
 * @brief Board_B_detec::get_area
 * //获取各区域检测面积
 * @param para
 */
void Board_A_detec::get_area(Board_A_detec::Board_A_area_item &para)
{
    para = m_area_detec;
}

/**
 * @brief Board_A_detec::set_num_template
 * //设置数字检测模板
 * @param template_vec
 */
void Board_A_detec::set_num_template(const vector<Mat> &template_vec)
{
    m_num_template_vec = template_vec;
}

QString Board_A_detec::get_numbers_area(const Mat &src_image, Mat &dst_image, Point2f &origin)
{
    const Point2f left_behind_pt(-80, 190);
    const Point2f right_below_pt(20, 580);

    Point pt_1, pt_2;
    pt_1.y = origin.y + left_behind_pt.y;
    pt_1.x = origin.x + left_behind_pt.x;
    pt_2.y = origin.y + right_below_pt.y;
    pt_2.x = origin.x + right_below_pt.x;

    Rect rect(pt_1.x, pt_1.y, pt_2.x-pt_1.x, pt_2.y-pt_1.y); //裁剪的左上角坐标(x,y) 大小 w,h

    cv::Mat gray_image;
    cv::Mat bin_image;      //二值图
    cv::Mat img1;
    cv::Mat canny_image;    //
    cv::Mat sobel_image;
    img1 = Mat(src_image, rect);        //找到包含一条边的图形，用于后续做倾斜矫正

    rotate(img1, dst_image, ROTATE_90_CLOCKWISE);//摆正图片
    //imshow("img1", dst_image);

    Mat element = getStructuringElement(MORPH_RECT, Size(5, 5)); //第一个参数MORPH_RECT表示矩形的卷积核，当然还可以选择椭圆形的、交叉型的
    cv::cvtColor(dst_image, gray_image, cv::COLOR_BGR2GRAY); //转换为灰度图
    cv::GaussianBlur(gray_image, gray_image, Size(3, 3), 0);
    equalizeHist(gray_image, gray_image);
    threshold(gray_image, bin_image, 150, 255, THRESH_BINARY);

    //imshow("bin_image", bin_image);
    morphologyEx(bin_image, bin_image, MORPH_CLOSE, element);
    dilate(bin_image, bin_image, element);
    //imshow("morphologyEx", bin_image);

    //遍历每一列的图像灰度值，查找每一行255的值
    int width = bin_image.cols;
    int height = bin_image.rows;
    int val_cnt = 0;;
    int start_index = -1;
    int end_index = -1;
    int upper_index = -1;
    int down_index = -1;

    //找到左边边界
    for (int col = 0; col < width; ++col){
        val_cnt = 0;
        for (int row = 0; row < height; ++row){
            if (255 == bin_image.at<uchar>(row, col)){
                val_cnt++;
            }
        }
        if (val_cnt >= (height/3)){
            start_index = col;
            break;
        }
    }

    //找到右边边界
    for (int col = (width-1); col > 0; --col){
        val_cnt = 0;
        for (int row = 0; row < height; ++row){
            if (255 == bin_image.at<uchar>(row, col)){
                val_cnt++;
            }
        }
        if (val_cnt >= (height/3)){
            end_index = col;
            break;
        }
    }

    //找到上边边界
    for (int row = 0; row < height; ++row){
        val_cnt = 0;
        for (int col = 0; col < width; ++col){
            if (255 == bin_image.at<uchar>(row, col)){
                val_cnt++;
            }
        }
        if (val_cnt >= (width/2)){
            upper_index = row;
            break;
        }
    }

    //找到下边边界
    for (int row = (height-1); row > 0; --row){
        val_cnt = 0;
        for (int col = 0; col < width; ++col){
            if (255 == bin_image.at<uchar>(row, col)){
                val_cnt++;
            }
        }
        if (val_cnt >= (width/2)){
            down_index = row;
            break;
        }
    }



    vector<Mat> num_image_vec;
    if ((end_index > start_index) && (down_index > upper_index)){
        int num_roi_width = end_index - start_index;
        int num_roi_height = down_index - upper_index;
        for (int i = 0; i < 9; i ++){
            Rect rect = Rect(start_index+num_roi_width/9*i, upper_index, num_roi_width/9, num_roi_height);
            Mat temp_img = Mat(dst_image, rect);
            num_image_vec.push_back(temp_img);
        }
    }

//    imshow("roiImg0", num_image_vec.at(0));
//    imshow("roiImg1", num_image_vec.at(1));
//    imshow("roiImg2", num_image_vec.at(2));
//    imshow("roiImg3", num_image_vec.at(3));
//    imshow("roiImg4", num_image_vec.at(4));
//    imshow("roiImg5", num_image_vec.at(5));
//    imshow("roiImg6", num_image_vec.at(6));
//    imshow("roiImg7", num_image_vec.at(7));
//    imshow("roiImg8", num_image_vec.at(8));

    int num = 0;
    QString str_ret;

    for (unsigned int i = 0; i < num_image_vec.size(); i ++){
        num = read_num(num_image_vec.at(i));
        str_ret.append(QString::number(num));
    }
    return str_ret;
}

void Board_A_detec::numbers_identify(const Mat &src_image, QString &num_string)
{
    Mat grayImage;
    Mat canny_image;

    Mat imageRGB[3];

    Mat src_img = src_image.clone();

    //white_balance(src_img, src_img);
    cvtColor(src_image, grayImage, COLOR_BGR2GRAY);
    cv::GaussianBlur(grayImage, grayImage, Size(3, 3), 0);
    Mat binImage;
    equalizeHist(grayImage, grayImage);
    //deal_with_sobel(grayImage, canny_image);
    //Canny(grayImage, canny_image, 70, 200);
    //cv::GaussianBlur(canny_image, canny_image, Size(3, 3), 0);

    //threshold(canny_image, binImage, 40, 255, THRESH_BINARY);

    Mat element = getStructuringElement(MORPH_RECT, Size(3, 3)); //第一个参数MORPH_RECT表示矩形的卷积核，当然还可以选择椭圆形的、交叉型的
//    //高级形态学处理，调用这个函数就可以了，具体要选择哪种操作，就修改第三个参数就可以了。这里演示的是形态学梯度处理


    threshold(grayImage, binImage, 90, 255, THRESH_BINARY_INV);
    erode(binImage, binImage, element, Point(-1,-1), 2);
    dilate(binImage, binImage, element, Point(-1,-1), 1);

    imshow("binImage", binImage);

    {
        int width = binImage.cols;
        int height = binImage.rows;
        int max_word_len = width / 9;
        int cnt = 0;
        for (int j = 0; j < height; j ++){
            cnt = 0;
            for (int i = 0; i < width; i ++){
                if (255 == binImage.at<uchar>(j, i)) cnt += 1;
                else cnt = 0;

                if (cnt == max_word_len){
                    cnt = 0;
                    for (int k = 0; k < width; k ++){
                        binImage.at<uchar>(j, k) = 0;
                    }
                    break;
                }
            }
        }

    }

    {
        int max_word_len = binImage.rows * 0.8;
        int cnt = 0;
        for (int j = 0; j < binImage.cols / 9; j ++){
            cnt = 0;
            for (int i = 0; i < binImage.rows; i ++){
                if (255 == binImage.at<uchar>(i, j)) cnt += 1;

                if (cnt == max_word_len){
                    cnt = 0;
                    for (int k = 0; k < binImage.rows; k ++){
                        binImage.at<uchar>(k, j) = 0;
                    }
                }
            }
        }
        for (int j = binImage.cols/9*8; j < binImage.cols; j ++){
            cnt = 0;
            for (int i = 0; i < binImage.rows; i ++){
                if (255 == binImage.at<uchar>(i, j)) cnt += 1;

                if (cnt == max_word_len){
                    cnt = 0;
                    for (int k = 0; k < binImage.rows; k ++){
                        binImage.at<uchar>(k, j) = 0;
                    }
                }
            }
        }
    }


    morphologyEx(binImage, binImage, MORPH_OPEN, element);
    imshow("MORPH_OPEN", binImage);


    vector<Mat> num_img_vec;
    int width = binImage.cols;
    int height = binImage.rows;
    int perPixelValue;//每个像素的值
    int* projectValArry = new int[width];//创建一个用于储存每列白色像素个数的数组
    memset(projectValArry, 0, width*4);//必须初始化数组

    //遍历每一列的图像灰度值，查找每一行255的值
    for (int col = 0; col < width; ++col){
        for (int row = 0; row < height; ++row){
            perPixelValue = binImage.at<uchar>(row, col);
            if (perPixelValue == 255)//如果是黑底白字
            {
                projectValArry[col]++;
            }
        }
    }

    /*新建一个Mat用于储存投影直方图并将背景置为白色*/
    Mat verticalProjectionMat(height, width, CV_8UC1);
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            perPixelValue = 255;  //背景设置为白色。
            verticalProjectionMat.at<uchar>(i, j) = perPixelValue;
        }
    }

    /*将直方图的曲线设为黑色*/
    for (int i = 0; i < width; i++){
        for (int j = 0; j < projectValArry[i]; j++){
            perPixelValue = 0;  //直方图设置为黑色
            verticalProjectionMat.at<uchar>(height - 1 - j, i) = perPixelValue;
        }
    }

    imshow("【投影】",verticalProjectionMat);
    int startIndex = 0;//记录进入字符区的索引
    int endIndex = 0;//记录进入空白区域的索引
    bool inBlock = false;//是否遍历到了字符区内
    for (int i = 0; i < binImage.cols; ++i){
        if (!inBlock && projectValArry[i] != 0)//进入字符区了
        {
            inBlock = true;
            startIndex = i;
            cout << "startIndex is " << startIndex << endl;
        }
        else if (projectValArry[i] == 0 && inBlock)//进入空白区了
        {
            endIndex = i;
            cout << "endIndex is " << endIndex << endl;
            if ((endIndex - startIndex) < 5) continue;
            inBlock = false;
            Mat roiImg = src_image(Range(0,binImage.rows),Range(startIndex,endIndex+1));
            num_img_vec.push_back(roiImg);
        }
    }

    QString num;
    //read_numbers(num_img_vec.at(0), num);

    imshow("roiImg0", num_img_vec.at(0));
    imshow("roiImg1", num_img_vec.at(1));
    imshow("roiImg2", num_img_vec.at(2));
    imshow("roiImg3", num_img_vec.at(3));
    imshow("roiImg4", num_img_vec.at(4));
    imshow("roiImg5", num_img_vec.at(5));
    imshow("roiImg6", num_img_vec.at(6));
    imshow("roiImg7", num_img_vec.at(7));
    imshow("roiImg8", num_img_vec.at(8));

    read_num(num_img_vec.at(0));
}

unsigned int Board_A_detec::get_JALVC164245_res(cv::Mat & src_image, Point2f & origin)
{
    Mat roi_image;
    Mat canny_image;
    double maxArea = 0;
    unsigned int res = 0;


//    Point left_behind_pt_arr[] = {Point(100, 1400), Point(440, 1400), Point(800, 1400), Point(1170, 1400), Point(1540, 1400)};
//    Point right_below_pt_arr[] = {Point(170, 1570), Point(540, 1570), Point(900, 1570), Point(1270, 1570), Point(1610, 1570)};

    Point left_behind_pt_arr[] = {Point(1830, -70), Point(1460, -70), Point(1100, -70), Point(730, -70), Point(390, -70)};
    Point right_below_pt_arr[] = {Point(1900, 100), Point(1560, 100), Point(1200, 100), Point(830, 100), Point(460, 100)};

    for (int i = 0; i < 5; i ++){
        Rect rect = get_roi_and_draw_rect(src_image, roi_image, origin, left_behind_pt_arr[i], right_below_pt_arr[i]);
        src_image_deal_for_calc_area(roi_image, canny_image);
        maxArea = calc_max_area(canny_image);

        m_area_detec.threshold_JALVC164245.push_back(maxArea);
        m_rect_vec.push_back(rect);
        if (maxArea < m_threshold_para.threshold_JALVC164245[i]) (res |= (1<<i));
    }
    return res;
}

unsigned int Board_A_detec::get_GH137S_res(cv::Mat & src_image, Point2f & origin)
{
    Mat roi_image;
    Mat canny_image;
    double maxArea = 0;
    unsigned int res = 0;

//    Point left_behind_pt_arr[] = {Point(60, 430), Point(190, 430), Point(360, 430), Point(740, 210), Point(870, 210)};
//    Point right_below_pt_arr[] = {Point(150, 600), Point(300, 600), Point(450, 600), Point(820, 380), Point(950, 380)};

    Point left_behind_pt_arr[] = {Point(1850, 900), Point(1700, 900), Point(1550, 900), Point(1180, 1120), Point(1050, 1120)};
    Point right_below_pt_arr[] = {Point(1940, 1070), Point(1810, 1070), Point(1640, 1070), Point(1260, 1290), Point(1130, 1290)};

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

unsigned int Board_A_detec::get_JDSPF28335_res(cv::Mat & src_image, Point2f & origin)
{
    Mat roi_image;
    Mat canny_image;
    double maxArea = 0;
    unsigned int res = 0;

//    Point left_behind_pt_arr[] = {Point(820, 700), Point(900, 630), Point(1350, 700), Point(900, 1125)};
//    Point right_below_pt_arr[] = {Point(920, 1150), Point(1370, 720), Point(1450, 1150), Point(1370, 1225)};

    Point left_behind_pt_arr[] = {Point(550, 380), Point(650, 275), Point(1080, 380), Point(650, 780), Point(590, 310), Point(1070, 310), Point(590, 780), Point(1070, 780)};
    Point right_below_pt_arr[] = {Point(650, 780), Point(1090, 375), Point(1180, 780), Point(1090, 870),  Point(670, 390), Point(1150, 390), Point(670, 860), Point(1150, 860)};


    for (int i = 0; i < 8; i ++){
        Rect rect = get_roi_and_draw_rect(src_image, roi_image, origin, left_behind_pt_arr[i], right_below_pt_arr[i]);
        src_image_deal_for_calc_area(roi_image, canny_image);
        maxArea = calc_max_area(canny_image);

        m_area_detec.threshold_JDSPF28335.push_back(maxArea);
        m_rect_vec.push_back(rect);
        if (maxArea < m_threshold_para.threshold_JDSPF28335[i]) (res |= (1<<i));
    }

//    get_roi_and_draw_rect(src_image, roi_image, origin, left_behind_pt_arr[5], right_below_pt_arr[5]);
//    src_image_deal_for_calc_area_debug(roi_image, canny_image);

    return res;
}

unsigned int Board_A_detec::get_SM1040_res(Mat &src_image, Point2f &origin)
{
    Mat roi_image;
    Mat canny_image;
    double maxArea = 0;
    unsigned int res = 0;

//    Point left_behind_pt_arr[] = {Point(150, 140), Point(290, 140)};
//    Point right_below_pt_arr[] = {Point(220, 260), Point(360, 260)};


    Point left_behind_pt_arr[] = {Point(1640, 1240), Point(1780, 1240)};
    Point right_below_pt_arr[] = {Point(1710, 1360), Point(1850, 1360)};

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

unsigned int Board_A_detec::get_J16H281_res(Mat &src_image, Point2f &origin)
{
    Mat roi_image;
    Mat canny_image;
    double maxArea = 0;
    unsigned int res = 0;

//    Point left_behind_pt_arr[] = {Point(440, 230), Point(680, 230)};
//    Point right_below_pt_arr[] = {Point(510, 360), Point(750, 360)};


    Point left_behind_pt_arr[] = {Point(1250, 1140), Point(1490, 1140)};
    Point right_below_pt_arr[] = {Point(1320, 1270), Point(1560, 1270)};


    for (int i = 0; i < 2; i ++){
        Rect rect = get_roi_and_draw_rect(src_image, roi_image, origin, left_behind_pt_arr[i], right_below_pt_arr[i]);
        src_image_deal_for_calc_area(roi_image, canny_image);
        maxArea = calc_max_area(canny_image);

        m_area_detec.threshold_J16H281.push_back(maxArea);
        m_rect_vec.push_back(rect);
        if (maxArea < m_threshold_para.threshold_J16H281[i]) (res |= (1<<i));
    }
    return res;
}



unsigned int Board_A_detec::get_SM490_res(Mat &src_image, Point2f &origin)
{
    Mat roi_image;
    Mat canny_image;
    double maxArea = 0;
    unsigned int res = 0;

//    Point left_behind_pt_arr[] = {Point(1070, 350), Point(1190, 350)};
//    Point right_below_pt_arr[] = {Point(1150, 480), Point(1270, 480)};


    Point left_behind_pt_arr[] = {Point(730, 1020), Point(850, 1020)};
    Point right_below_pt_arr[] = {Point(810, 1150), Point(930, 1150)};

//    get_roi_and_draw_rect(src_image, roi_image, origin, left_behind_pt_arr[0], right_below_pt_arr[0]);
//    src_image_deal_for_calc_area(roi_image, canny_image);
//    maxArea = calc_max_area(canny_image);
//
//    return 0;

    for (int i = 0; i < 2; i ++){
        Rect rect = get_roi_and_draw_rect(src_image, roi_image, origin, left_behind_pt_arr[i], right_below_pt_arr[i]);
        src_image_deal_for_calc_area(roi_image, canny_image);
        maxArea = calc_max_area(canny_image);

        m_area_detec.threshold_SM490.push_back(maxArea);
        m_rect_vec.push_back(rect);
        if (maxArea < m_threshold_para.threshold_SM490[i]) (res |= (1<<i));
    }
    return res;
}

unsigned int Board_A_detec::get_100_res(Mat &src_image, Point2f &origin)
{
    Mat roi_image;
    Mat canny_image;
    double maxArea = 0;
    unsigned int res = 0;

//    Point left_behind_pt_arr[] = {Point(1700, -90), Point(1700, 0)};
//    Point right_below_pt_arr[] = {Point(1850, -20), Point(1850, 70)};


    Point left_behind_pt_arr[] = {Point(150, 1430), Point(150, 1520)};
    Point right_below_pt_arr[] = {Point(300, 1500), Point(300, 1590)};

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

void Board_A_detec::image_deal_for_num_read(Mat src_image, Mat &dst_image)
{
    cv::Mat gray_image;
    cv::Mat bin_image;      //二值图
    cv::Mat img1;
    cv::Mat canny_image;    //
    cv::Mat sobel_image;

    Mat element = getStructuringElement(MORPH_RECT, Size(3, 3)); //第一个参数MORPH_RECT表示矩形的卷积核，当然还可以选择椭圆形的、交叉型的
    cv::cvtColor(src_image, gray_image, cv::COLOR_BGR2GRAY); //转换为灰度图
    cv::GaussianBlur(gray_image, gray_image, Size(3, 3), 0);
    equalizeHist(gray_image, gray_image);
    threshold(gray_image, bin_image, 180, 255, THRESH_BINARY_INV);
    //morphologyEx(bin_image, dst_image, MORPH_OPEN, element);
    erode(bin_image, dst_image, element);
    //imshow("bin_image", dst_image);
}

int Board_A_detec::read_num(Mat &src_image)
{
    Mat num_template;

    Mat new_researchImg, new_tmpImg;
    double match_max = 0;
    int match_index = 0;
    Point  minLoc = Point(-1, -1), maxLoc = Point(-1, -1);

    for (unsigned int i = 0; i < m_num_template_vec.size(); i++){
        Mat mat_mval = Mat(Size(1, 1), CV_32FC1);
        Mat num_imag = src_image.clone();
        num_template = m_num_template_vec.at(i).clone();
        cv::resize(num_imag, num_imag, Size(num_template.cols, num_template.rows));

        image_deal_for_num_read(num_template, num_template);
        image_deal_for_num_read(num_imag, num_imag);
        matchTemplate(num_imag, num_template, mat_mval, TM_CCOEFF_NORMED);
        double min = 0.0, max = 0.0;
        minMaxLoc(mat_mval, &min, &max, &minLoc,&maxLoc,Mat());


        if (match_max < max){
            match_max = max;
            match_index = i;
        }
    }
//    cout<<"match_index is:"<<match_index<<endl;
    num_template = m_num_template_vec.at(9).clone();
    image_deal_for_num_read(num_template, num_template);
//    imshow("num_template", num_template);
    return match_index;
}



