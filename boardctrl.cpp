#include "boardctrl.h"
#include <QMetaType>

BoardCtrl::BoardCtrl()
{
    m_radius = 80;

    //----------------------注册自定义消息类型-------------------------
    qRegisterMetaType<QString>("QString");
    qRegisterMetaType<cv::Mat>("cv::Mat");
    qRegisterMetaType<cv::Mat>("cv::Mat &");
    qRegisterMetaType<Mat>("Mat &");
    qRegisterMetaType<Mat>("Mat");
    qRegisterMetaType<unsigned int>("unsigned int");
    qRegisterMetaType<int>("int");
    qRegisterMetaType<Board_A_detec::Board_A_area_item>("Board_A_detec::Board_A_area_item");
    qRegisterMetaType<Board_B_detec::Board_B_area_item>("Board_B_detec::Board_B_area_item");
}

BoardCtrl::~BoardCtrl()
{

}

/**
 * @brief BoardCtrl::boart_ctrl_main_work
 * //目标面积检测主函数，用于执行目标板面积检测
 */
void BoardCtrl::boart_ctrl_main_work()
{
    cv::Mat dst_imag;
    Point2f origin;
    unsigned int res = 0;
    int ret = 0;

    emit thread_is_running();
    //首先对输入图像进行霍夫圆孔定位，若不能检测到圆孔，则将其按照Ｂ面进行处理，若仍返回错误，则boadr_info＝（－１），表示无法识别图像信息
    ret = board_A_pre_deal(m_image_src, dst_imag, origin);
    if (ret < 0){
        ret = board_B_pre_deal(m_image_src, dst_imag, origin);
        if (ret < 0){
            m_board_info = -1;
        }
        else{
            res = m_board_B.get_board_B_res(dst_imag, origin);
            m_board_info = 1;
        }
    }
    else{
        res = m_board_A.get_board_A_res(dst_imag, origin);
        //m_board_A.read_numbers(dst_imag, board_num, origin);
        m_board_info = 0;
    }

    if (m_board_info >= 0){
        m_image_mark = Mat(dst_imag, Rect(0,0,2200, 1700));                           //裁剪出目标区域
    }

    //----------------------检测完毕，通过信号发送相应的检测记结果---------------------------
    emit board_detec_over(m_board_info, m_image_mark, res);
    if (0 == m_board_info){
        Board_A_detec::Board_A_area_item area_info;
        m_board_A.get_area(area_info);
        emit board_A_area_signal(area_info);
    }
    else if (1 == m_board_info){
        Board_B_detec::Board_B_area_item area_info;
        m_board_B.get_area(area_info);
        emit board_B_area_signal(area_info);
    }

    emit thread_is_stop();
}

/**
 * @brief BoardCtrl::get_board_A_area
 * //获取Ａ面所有区域的检测面积
 * @param area_info
 * @return
 */
int BoardCtrl::get_board_A_area(Board_A_detec::Board_A_area_item &area_info)
{
    m_board_A.get_area(area_info);
    return 0;
}

/**
 * @brief BoardCtrl::get_board_A_area
 * //获取B面所有区域的检测面积
 * @param area_info
 * @return
 */
int BoardCtrl::get_board_B_area(Board_B_detec::Board_B_area_item &area_info)
{
    m_board_B.get_area(area_info);
    return 0;
}

/**
 * @brief BoardCtrl::set_board_A_area
 * //设置A检测阈值
 * @param area_info
 */
void BoardCtrl::set_board_A_area(const Board_A_detec::Board_A_area_item &area_info)
{
    m_board_A.set_threshold(area_info);
    cout<<"m_board_A.set_threshold(area_info);"<<endl;
}

/**
 * @brief BoardCtrl::set_board_B_area
 * //设置B检测阈值
 * @param area_info
 */
void BoardCtrl::set_board_B_area(const Board_B_detec::Board_B_area_item &area_info)
{
    m_board_B.set_threshold(area_info);
}

/**
 * @brief BoardCtrl::set_match_template
 * //设置匹配模板，总共４个区域
 * @param template_src
 */
void BoardCtrl::set_match_template(const vector<Mat> &template_src)
{
    for (unsigned long i = 0; i < template_src.size(); i ++){
        m_template_arr[i] = template_src.at(i).clone();
    }
}

void BoardCtrl::set_num_template(const vector<Mat> &template_src)
{
    m_board_A.set_num_template(template_src);
}

/**
 * @brief BoardCtrl::set_rotate_template
 * //设置用于旋转矫正的模板
 * @param image_src
 */
void BoardCtrl::set_rotate_template(const Mat &image_src)
{
    m_src_template = image_src.clone();
}

/**
 * @brief BoardCtrl::set_rotate_template_A
 * @param image_src
 */
void BoardCtrl::set_rotate_template_A(const Mat &image_src)
{
    m_src_template_A = image_src.clone();
}

/**
 * @brief BoardCtrl::set_image_src
 * //设置所检测的源文件
 * @param image_src
 */
void BoardCtrl::set_image_src(const Mat &image_src)
{
    m_image_src = image_src.clone();
}

void BoardCtrl::set_best_radius(int r)
{
    m_radius = r;
}

/**
 * @brief BoardCtrl::board_B_pre_deal
 * //电路板Ｂ面图像数据预处理，主要是找出定位点并且将图像进行放射变化，便于后续定位待测区域
 * @param src_image
 * @param dst_image
 * @param origin
 * //进行放射变化后的图像坐标原点
 * @return
 */
int BoardCtrl::board_B_pre_deal(Mat &src_image, Mat &dst_image, Point2f &origin)
{
    Mat rotate_image_src;
    board_B_src_rotate(src_image, rotate_image_src);

    vector<Point> center_vec;
    Point center;
    matcher(rotate_image_src, m_template_arr[0], center);
    center_vec.push_back(center);
    matcher(rotate_image_src, m_template_arr[1], center);
    center_vec.push_back(center);
    matcher(rotate_image_src, m_template_arr[2], center);
    center_vec.push_back(center);
    matcher(rotate_image_src, m_template_arr[3], center);
    center_vec.push_back(center);

    for (unsigned long i=0; i < center_vec.size(); i ++){
        drawMarker(rotate_image_src, center_vec.at(i), Scalar(0,0,255), MarkerTypes::MARKER_CROSS, 40, 10);
    }

    //-----------------------判定模板匹配的中心点是否正常，正常顺序是左上左下右上右下---------------------------
    if ((center_vec.at(0).x > center_vec.at(2).x) || (center_vec.at(0).y > center_vec.at(1).y)) return (-1);
    if ((center_vec.at(0).x > center_vec.at(3).x) || (center_vec.at(0).y > center_vec.at(3).y)) return (-1);
    if ((center_vec.at(3).x < center_vec.at(1).x) || (center_vec.at(3).y < center_vec.at(2).y)) return (-1);

    Mat temp_disp;
//    pyrDown(rotate_image_src, temp_disp);
//    pyrDown(temp_disp, temp_disp);
//    pyrDown(temp_disp, temp_disp);
//    imshow("rotate_image_src", temp_disp);

    //----------------------------------仿射变换，矫正图片---------------------------------
    Point2f srcTri[4];
    Point2f dstTri[4];

    Mat warpPerspective_mat( 3, 3, CV_32FC1 );
    Mat src, warpPerspective_dst;

    /// Load the image
    src = rotate_image_src.clone();

    /// Set the dst image the same type and size as src
    warpPerspective_dst = Mat::zeros( src.rows, src.cols, src.type() );

    /// 设置三组点，求出变换矩阵
    srcTri[0] = Point2f( center_vec[0].x,center_vec[0].y );
    srcTri[1] = Point2f( center_vec[1].x,center_vec[1].y );
    srcTri[2] = Point2f( center_vec[2].x,center_vec[2].y );
    srcTri[3] = Point2f( center_vec[3].x,center_vec[3].y );

    dstTri[0] = Point2f( 100,100 );
    dstTri[1] = Point2f( 100,1600 );
    dstTri[2] = Point2f( 2100,100 );
    dstTri[3] = Point2f( 2100,1600 );

    //计算3个二维点对之间的仿射变换矩阵（2行x3列）
    warpPerspective_mat = getPerspectiveTransform( srcTri, dstTri );

    //应用仿射变换，可以恢复出原图
    warpPerspective( src, warpPerspective_dst, warpPerspective_mat, src.size() );

    dst_image = warpPerspective_dst.clone();
    origin = dstTri[0];

//    pyrDown(dst_image, temp_disp);
//    pyrDown(temp_disp, temp_disp);
//    pyrDown(temp_disp, temp_disp);
//    imshow("dst_image", temp_disp);

    return 0;
}

/**
 * @brief BoardCtrl::board_A_pre_deal
 * //Ａ面图像预处理，主要是检测圆心并进行放射变换
 * @param src_image
 * @param dst_image
 * @param origin
 * @return 0,OK,(-1)NG
 */
int BoardCtrl::board_A_pre_deal(Mat &src_image, Mat &dst_image, Point2f &origin)
{
    std::vector<cv::Vec3f> circles_pixel;
    Mat rotate_image_src;
    //board_A_src_rotate(src_image, rotate_image_src);
    rotate_image_src = src_image.clone();
    get_circles_pos(rotate_image_src, circles_pixel);  //霍夫圆心检测
    //检测到的圆心不为４，返回错误
    if (4 != circles_pixel.size()){
        return (-1);
    }

    std::vector<cv::Vec3f> arranged_circle;
    arrange_circle(circles_pixel, arranged_circle); //对找到的圆进行排序，顺序是左上左下右上右下

    //----------------------------------仿射变换，矫正图片---------------------------------
    Point2f srcTri[4];
    Point2f dstTri[4];

    Mat warpPerspective_mat( 3, 3, CV_32FC1 );
    Mat src, warpPerspective_dst;

    /// Load the image
    src = rotate_image_src.clone();

    /// Set the dst image the same type and size as src
    warpPerspective_dst = Mat::zeros( src.rows, src.cols, src.type() );

    /// 设置三组点，求出变换矩阵
    srcTri[0] = Point2f( arranged_circle[0][0],arranged_circle[0][1] );
    srcTri[1] = Point2f( arranged_circle[1][0],arranged_circle[1][1] );
    srcTri[2] = Point2f( arranged_circle[2][0],arranged_circle[2][1] );
    srcTri[3] = Point2f( arranged_circle[3][0],arranged_circle[3][1] );

    dstTri[0] = Point2f( 100,100 );
    dstTri[1] = Point2f( 100,1600 );
    dstTri[2] = Point2f( 2100,100 );
    dstTri[3] = Point2f( 2100,1600 );

    //计算3个二维点对之间的仿射变换矩阵（2行x3列）
    warpPerspective_mat = getPerspectiveTransform( srcTri, dstTri );

    //应用仿射变换，可以恢复出原图
    warpPerspective( src, warpPerspective_dst, warpPerspective_mat, src.size() );
    //----------------------------------------------------------------------------------

    dst_image = warpPerspective_dst.clone();
    origin = dstTri[0];
    return 0;
}

/**
 * @brief BoardCtrl::board_B_src_rotate
 * //对传入的图像进行旋转匹配，用于确定其旋转角度并摆正
 * @param src_image
 * @param dst_image
 */
void BoardCtrl::board_B_src_rotate(const Mat &src_image, Mat &dst_image)
{
    //灰度图转换
    Mat image1, image2;

    image_fileter_for_rotate(src_image, image1);
    image_fileter_for_rotate(m_src_template, image2);
    Mat temp;
//    pyrDown(image1, temp);
//    pyrDown(temp, temp);
//    pyrDown(temp, temp);
//    imshow("image1", temp);

//    pyrDown(image2, temp);
//    pyrDown(temp, temp);
//    pyrDown(temp, temp);
//    imshow("image2", temp);

    Point center;
    Point  minLoc = Point(-1, -1), maxLoc = Point(-1, -1);
    double min = 0.0, max = 0.0;
    int tmp_w = image1.cols - image2.cols + 1;
    int tmp_h = image1.rows - image2.rows + 1;

    Mat mat_mval = Mat(Size(tmp_w, tmp_h), CV_32FC1);
    double degrees = 0;

    Mat imag_src,imag_temp;

    pyrDown(image2, imag_temp);
    pyrDown(imag_temp, imag_temp);
    pyrDown(imag_temp, imag_temp);

    pyrDown(image1, imag_src);
    pyrDown(imag_src, imag_src);
    pyrDown(imag_src, imag_src);

    double match_max = 0;
    Point match_loc(-1,-1);
    double match_degree;
    Mat rotate_temp;
    for (degrees = 0; degrees < 360; degrees += 1){
        rotateImage2(imag_src, rotate_temp, degrees);
        matchTemplate(rotate_temp,imag_temp,mat_mval, TM_CCOEFF_NORMED);
        minMaxLoc(mat_mval, &min, &max, &minLoc,&maxLoc,Mat());
        if (max > match_max){
            match_max = max;
            match_loc = maxLoc;
            match_degree = degrees;
        }

    }
    cout<<match_degree<<endl;
    rotateImage2(src_image, rotate_temp, match_degree);
    dst_image = rotate_temp.clone();

    match_loc.x *= 8;
    match_loc.y *= 8;
    Point point;
    point.x = (match_loc.x - 200)>=0?(match_loc.x - 200):0;
    point.y = (match_loc.y - 200)>=0?(match_loc.y - 200):0;
    int width = (match_loc.x + m_src_template.cols + 200)<=dst_image.cols?(m_src_template.cols + 200):(dst_image.cols-match_loc.x);
    int height = (match_loc.y + m_src_template.rows + 200)<=dst_image.rows?(m_src_template.rows + 200):(dst_image.rows-match_loc.y);
    width += 200;
    height += 200;
    cv::Mat img_temp = Mat(dst_image, Rect(point.x, point.y, width, height));
    dst_image = img_temp.clone();
//    pyrDown(img_temp, img_temp);
//    pyrDown(img_temp, img_temp);
//    pyrDown(img_temp, img_temp);
//    imshow("rotateImage2", img_temp);
}

static vector<float> extract_hog_features(const Mat& image)
{
    Mat gray;
    cvtColor(image, gray, COLOR_BGR2GRAY);
    resize(gray, gray, Size(128, 64));
    HOGDescriptor hog(Size(128,64),Size(16,16),Size(8,8),Size(8,8),9);
    vector<float> descriptors;
    hog.compute(gray, descriptors);

    return descriptors;
}

float calculate_relation(vector<float> descriptor1,vector<float> descriptor2)
{
    CV_Assert(descriptor1.size() == descriptor2.size());
    // 计算内积
    float innerMul = 0.0;
    float amp1 = 0.0;
    float amp2 = 0.0;
    for (int i = 0; i < descriptor1.size(); i++)
    {
        innerMul += descriptor1[i] * descriptor2[i];
        amp1 += pow(descriptor1[i], 2);
        amp2 += pow(descriptor2[i], 2);
    }
    float correlation = innerMul / (sqrt(amp1) * sqrt(amp2));

    return correlation;
}

/**
 * @brief BoardCtrl::board_B_src_rotate
 * //对传入的图像进行旋转匹配，用于确定其旋转角度并摆正A
 * @param src_image
 * @param dst_image
 */
void BoardCtrl::board_A_src_rotate(const Mat &src_image, Mat &dst_image)
{
    //灰度图转换
    Mat image1, image2;

    Image_fileter(src_image, image1);
    Image_fileter(m_src_template_A, image2);

    Point center;
    Point  minLoc = Point(-1, -1), maxLoc = Point(-1, -1);
    double min = 0.0, max = 0.0;
    int tmp_w = image1.cols - image2.cols + 1;
    int tmp_h = image1.rows - image2.rows + 1;

    Mat mat_mval = Mat(Size(tmp_w, tmp_h), CV_32FC1);
    double degrees = 0;

    Mat imag_src,imag_temp;

    pyrDown(image2, imag_temp);
    pyrDown(imag_temp, imag_temp);
    pyrDown(imag_temp, imag_temp);
    pyrDown(imag_temp, imag_temp);

    pyrDown(image1, imag_src);
    pyrDown(imag_src, imag_src);
    pyrDown(imag_src, imag_src);
    pyrDown(imag_src, imag_src);

    double match_max = 0;
    Point match_loc(-1,-1);
    double match_degree;
    Mat rotate_temp;
    Mat rotate_temp_corr;
    for (degrees = 0; degrees < 360; degrees += 1){
        rotateImage2(imag_src, rotate_temp, degrees);
        matchTemplate(rotate_temp, imag_temp, mat_mval, TM_CCOEFF_NORMED);
        minMaxLoc(mat_mval, &min, &max, &minLoc,&maxLoc,Mat());
        if (max > match_max){
            match_max = max;
            match_loc = maxLoc;
            match_degree = degrees;
        }
    }

    cout<<"match_max"<<match_max<<endl;
    rotateImage2(src_image, rotate_temp, match_degree);
    dst_image = rotate_temp.clone();
}

/**
 * @brief BoardCtrl::get_circles_pos
 * //霍夫圆心检测
 * @param src_image
 * @param circles
 */
void BoardCtrl::get_circles_pos(const Mat &src_image, std::vector<Vec3f> &circles)
{
    cv::Mat gray_image;
    cv::Mat img_temp;
    cv::Mat src_img = src_image.clone();
    cv::cvtColor(src_image, gray_image, cv::COLOR_BGR2GRAY);
    //cv::GaussianBlur(gray_image, gray_image, cv::Size(5, 5), 1, 1);
    blur(gray_image, gray_image, cv::Size(3, 3));

    int min_radius = 60;
    int max_radis = 100;

    if (m_radius > 40){
        min_radius = m_radius - 20;
        max_radis = m_radius + 20;
    }

    int min_distance = 300*2;

    /*
    //HoughCircles函数的原型为：
    //void HoughCircles(InputArray image,OutputArray circles, int method, double dp, double minDist, double param1=100, double param2=100, int minRadius=0,int maxRadius=0 )
    //image为输入图像，要求是灰度图像
    //circles为输出圆向量，每个向量包括三个浮点型的元素——圆心横坐标，圆心纵坐标和圆半径
    //method为使用霍夫变换圆检测的算法，Opencv2.4.9只实现了2-1霍夫变换，它的参数是CV_HOUGH_GRADIENT
    //dp为第一阶段所使用的霍夫空间的分辨率，dp=1时表示霍夫空间与输入图像空间的大小一致，dp=2时霍夫空间是输入图像空间的一半，以此类推
    //minDist为圆心之间的最小距离，如果检测到的两个圆心之间距离小于该值，则认为它们是同一个圆心
    //param1、param2为阈值
    //minRadius和maxRadius为所检测到的圆半径的最小值和最大值
    */
    cv::HoughCircles(gray_image, circles, cv::HOUGH_GRADIENT, 1, min_distance, 150, 80, min_radius, max_radis);

    cv::Mat dst_image;
    dst_image = src_image.clone();
    //在原图中画出圆心和圆
    for (size_t i = 0; i < circles.size(); i++){
        //提取出圆心坐标
        Point center(round(circles[i][0]), round(circles[i][1]));
        //提取出圆半径
        int radius = round(circles[i][2]);
        //圆心
        circle(dst_image, center, 3, Scalar(0, 255, 0), -1, 4, 0);
        //圆
        circle(dst_image, center, radius, Scalar(0, 0, 255), 3, 4, 0);
    }


//    pyrDown(dst_image, img_temp);
//    pyrDown(img_temp, img_temp);
//    pyrDown(img_temp, img_temp);
//    imshow("img_temp", img_temp);
}

void BoardCtrl::arrange_circle(const std::vector<Vec3f> &input_circle, std::vector<Vec3f> &out_circle)
{
    if (4 != input_circle.size()) return;

    if (! out_circle.empty()) out_circle.clear();

    out_circle = input_circle;
    cv::Vec3f vec3f_temp;

    unsigned int circle_size = 4;
    for (unsigned int i = 0;i < (circle_size-1); i ++){
        for (unsigned int j = 0; j < (circle_size-1)-i; j++){
            if (out_circle[j][0] > out_circle[j+1][0]){
                vec3f_temp = out_circle[j];
                out_circle[j] = out_circle[j+1];
                out_circle[j+1] = vec3f_temp;
            }
        }
    }

    if (out_circle[0][1] > out_circle[1][1]){
        vec3f_temp = out_circle[0];
        out_circle[0] = out_circle[1];
        out_circle[1] = vec3f_temp;
    }

    if (out_circle[2][1] > out_circle[3][1]){
        vec3f_temp = out_circle[2];
        out_circle[2] = out_circle[3];
        out_circle[3] = vec3f_temp;
    }
}

/**
 * @brief BoardCtrl::matcher
 * //模板匹配并找出模板所在的中心点
 * @param researchImg
 * @param tmpImg
 * @param center
 */
void BoardCtrl::matcher(Mat researchImg, Mat tmpImg, Point &center)
{
    Mat Resch_img, tmp_img;
    Mat new_researchImg, new_tmpImg;
    double min = 0.0, max = 0.0;
    Point  minLoc = Point(-1, -1), maxLoc = Point(-1, -1);
    Point new_loc;

    pyrDown(researchImg, new_researchImg);
    pyrDown(new_researchImg, new_researchImg);

    pyrDown(tmpImg, new_tmpImg);
    pyrDown(new_tmpImg, new_tmpImg);


    int tmp_w = new_researchImg.cols - new_tmpImg.cols + 1;
    int tmp_h = new_researchImg.rows - new_tmpImg.rows + 1;

    Mat mat_mval = Mat(Size(tmp_w, tmp_h), CV_32FC1);

    image_fileter_for_match(new_researchImg, Resch_img);
    image_fileter_for_match(new_tmpImg, tmp_img);

    Mat rotate_image;

    matchTemplate(Resch_img, tmp_img, mat_mval, TM_CCOEFF_NORMED);

    minMaxLoc(mat_mval, &min, &max, &minLoc,&maxLoc,Mat());


    center.x = (maxLoc.x + tmp_img.rows/2)*4;
    center.y = (maxLoc.y + tmp_img.cols/2)*4;
}

/**
 * @brief BoardCtrl::Image_fileter
 * //图像滤波，用于后续模板识别所使用
 * @param resImg
 * @param output_img
 */
void BoardCtrl::Image_fileter(Mat resImg, Mat &output_img)
{
    Mat _resImg;
    Mat img_tmp;
    cv::Mat gray_image;
    cv::Mat canny_image;
    cv::Mat bin_image;
    cv::Mat blur_image;
    //white_balance(resImg, resImg);
    cvtColor(resImg, _resImg, COLOR_BGR2GRAY);
    //deal_with_sobel(_resImg, output_img);
    /*均衡化处理*/
    equalizeHist(_resImg, output_img);

    /*高斯滤波*/
    Mat Kernel = getStructuringElement(cv::MORPH_RECT, Size(3, 3), Point(-1, -1));
    GaussianBlur(output_img, output_img, Size(3, 3),0);
    /*求梯度*/
    //morphologyEx(output_img, output_img, cv::MORPH_GRADIENT, Kernel);
    //deal_with_sobel(output_img, output_img);
}

void BoardCtrl::image_fileter_for_rotate(Mat resImg, Mat &output_img)
{
    Mat _resImg;
    Mat img_tmp;
    cv::Mat gray_image;
    cv::Mat canny_image;
    cv::Mat bin_image;
    cv::Mat blur_image;
    //white_balance(resImg, resImg);
    cvtColor(resImg, _resImg, COLOR_BGR2GRAY);
    //deal_with_sobel(_resImg, output_img);
    /*均衡化处理*/
    //equalizeHist(_resImg, _resImg);
    //threshold(_resImg, bin_image, 170,255, THRESH_BINARY);

    /*高斯滤波*/
    Mat Kernel = getStructuringElement(cv::MORPH_RECT, Size(3, 3), Point(-1, -1));
    GaussianBlur(_resImg, output_img, Size(3, 3),0);
    /*求梯度*/
    morphologyEx(output_img, output_img, cv::MORPH_GRADIENT, Kernel);
    //deal_with_sobel(output_img, output_img);
}

void BoardCtrl::image_fileter_for_match(Mat resImg, Mat &output_img)
{
    Mat _resImg;
    Mat img_tmp;
    cv::Mat gray_image;
    cv::Mat canny_image;
    cv::Mat bin_image;
    cv::Mat blur_image;
    //white_balance(resImg, resImg);
    cvtColor(resImg, _resImg, COLOR_BGR2GRAY);
    //deal_with_sobel(_resImg, output_img);
    /*均衡化处理*/
    equalizeHist(_resImg, output_img);
    //deal_with_sobel(output_img, output_img);

    /*高斯滤波*/
    Mat Kernel = getStructuringElement(cv::MORPH_RECT, Size(3, 3), Point(-1, -1));
    GaussianBlur(output_img, output_img, Size(3, 3),0);
    /*求梯度*/
    morphologyEx(output_img, output_img, cv::MORPH_GRADIENT, Kernel);
    //deal_with_sobel(output_img, output_img);
}
