#include "commonFunc.h"

#include <opencv2/features2d.hpp>
/**
 * @brief calc_max_area
 * //计算轮廓的包围的最大面积
 * @param src_image
 * //源文件，一般为canny处理过的二值图像
 * @return
 */
double calc_max_area(const cv::Mat & src_image)
{
    std::vector<std::vector<Point>> contours;    //储存轮廓
    std::vector<Vec4i> hierarchy;

    //检测到的轮廓都存在contours里，每个轮廓保存为一个vector<Point>
    findContours(src_image, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);    //获取轮廓

    //反复调用drawContours函数将轮廓画出
    cv::Mat line_image = Mat::zeros(src_image.rows, src_image.cols, CV_8UC1);
    for (unsigned long index = 0; index < contours.size(); index++){
        drawContours(line_image, contours, index, Scalar(255), 1, 8/*, hierarchy*/);
    }

    //提取面积最大的轮廓并用多边形将轮廓包围
    //contourArea用来计算轮廓的面积
    //approxPolyDP的作用是用多边形包围轮廓，可以得到严格的矩形，有助于找到角点
    std::vector<std::vector<Point>> polyContours(contours.size());
    int maxArea = 0;
    for (unsigned long index = 0; index < contours.size(); index++){
        if (contourArea(contours[index]) > contourArea(contours[maxArea]))
            maxArea = index;
        approxPolyDP(contours[index], polyContours[index], 20, true);
    }

    if (0 == contours.size()) return 0;
    return contourArea(contours[maxArea]);
}

/**
 * @brief white_balance
 * //白平衡
 * @param src_image
 * @param dst_image
 */
void white_balance(const cv::Mat & src_image, cv::Mat & dst_image)
{
    vector<Mat> g_vChannels;
    //分离通道
    split(src_image,g_vChannels);
    Mat imageBlueChannel = g_vChannels.at(0);
    Mat imageGreenChannel = g_vChannels.at(1);
    Mat imageRedChannel = g_vChannels.at(2);

    double imageBlueChannelAvg=0;
    double imageGreenChannelAvg=0;
    double imageRedChannelAvg=0;

    //求各通道的平均值
    imageBlueChannelAvg = mean(imageBlueChannel)[0];
    imageGreenChannelAvg = mean(imageGreenChannel)[0];
    imageRedChannelAvg = mean(imageRedChannel)[0];

    //求出个通道所占增益
    double K = (imageRedChannelAvg+imageGreenChannelAvg+imageRedChannelAvg)/3;
    double Kb = K/imageBlueChannelAvg;
    double Kg = K/imageGreenChannelAvg;
    double Kr = K/imageRedChannelAvg;

    //更新白平衡后的各通道BGR值
    addWeighted(imageBlueChannel,1,0,0,0,imageBlueChannel);
    addWeighted(imageGreenChannel,0,0,0,0,imageGreenChannel);
    addWeighted(imageRedChannel,0,0,0,0,imageRedChannel);

    merge(g_vChannels, dst_image);//图像各通道合并

    //dst_image = g_vChannels.at(0);
}

/**
 * @brief deal_with_sobel
 * //sobel算子处理
 * @param src
 * @param dst
 */
void deal_with_sobel(cv::Mat & src, cv::Mat & dst)
{
    Mat grad_x, grad_y;
    Mat abs_grad_x, abs_grad_y;

    //求x方向梯度
    Sobel(src, grad_x, CV_16S, 1, 0, 3, 1, 1,BORDER_DEFAULT);
    convertScaleAbs(grad_x, abs_grad_x);
    //imshow("x方向soble", abs_grad_x);

    //求y方向梯度
    Sobel(src, grad_y,CV_16S,0, 1,3, 1, 1, BORDER_DEFAULT);
    convertScaleAbs(grad_y,abs_grad_y);
    //imshow("y向soble", abs_grad_y);

    //合并梯度
    addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, dst);
}

/**
 * @brief get_roi_and_draw_rect
 * //根据坐标原点和相对坐标提取出ＲＯＩ并在原图上画出相应的矩形
 * @param src_image
 * @param dst_image
 * @param origin
 * @param left_behind_pt
 * @param right_below_pt
 */
Rect get_roi_and_draw_rect(cv::Mat & src_image, cv::Mat & dst_image, Point2f & origin, Point &left_behind_pt, Point &right_below_pt)
{
    Mat mew_img = src_image.clone();
    Point pt_1, pt_2;
    pt_1.y = origin.y + left_behind_pt.y;
    pt_1.x = origin.x + left_behind_pt.x;
    pt_2.y = origin.y + right_below_pt.y;
    pt_2.x = origin.x + right_below_pt.x;

    Rect rect(pt_1.x, pt_1.y, pt_2.x-pt_1.x, pt_2.y-pt_1.y);    //裁剪的左上角坐标(x,y) 大小 w,h
    dst_image = Mat(mew_img, rect);                           //裁剪出目标区域
    //rectangle(src_image, rect, Scalar(0, 0, 255),2, LINE_8,0);  //图像上画出矩形框
    return rect;
}

float get_angel(const cv::Mat & src_image)
{
    cv::Mat gray;
    cv::cvtColor(src_image, gray, COLOR_BGR2GRAY);
    cv::GaussianBlur(gray, gray, Size(5, 5), 0);

    Mat element = getStructuringElement(MORPH_RECT, Size(5, 5)); //第一个参数MORPH_RECT表示矩形的卷积核，当然还可以选择椭圆形的、交叉型的
    //高级形态学处理，调用这个函数就可以了，具体要选择哪种操作，就修改第三个参数就可以了。这里演示的是形态学梯度处理
    morphologyEx(gray, gray, MORPH_GRADIENT, element);

    cv::Mat cannyimage;
    //Canny(gray, cannyimage, 30, 80);
    deal_with_sobel(gray, cannyimage);
    threshold(cannyimage, cannyimage, 10, 255, THRESH_BINARY);
    cv::GaussianBlur(cannyimage, cannyimage, Size(3, 3), 0);

    vector<Vec4i> result;

    cv::HoughLinesP(cannyimage, result, 1, CV_PI / 180, 150, 1000);

    Mat houghDst;
    src_image.copyTo(houghDst);
    double meanAngle = 0.0;
    int numCnt = 0;
    for (size_t i = 0; i < result.size(); i++)
    {
        float startx = result[i][0];
        float starty = result[i][1];
        float endx = result[i][2];
        float endy = result[i][3];

        float kLine1 = (endy - starty)/(endx - startx);
        float lines_arctan = atan(kLine1) * 180.0 / CV_PI;            //反正切获取夹角

        line(houghDst, Point(startx,starty),Point(endx,endy), Scalar(0, 0, 255), 8);


        //if (45 < abs(lines_arctan)) continue;
        meanAngle += lines_arctan;
        numCnt ++;
    }

    meanAngle = meanAngle / numCnt;

    return meanAngle;
}



//图像旋转2：扩充图像边缘full
//         Mat img ：图像输入，单通道或者三通道
//         Mat & imgout ：图像输出
//         int degree ：图像要旋转的角度
//         int border_value：图像旋转填充值
int rotateImage2(Mat img,Mat & imgout, double degree)
{
//    if(img.empty())
//        return 1;
//    //degree = degree;//warpAffine默认的旋转方向是逆时针，所以加负号表示转化为顺时针
//    double angle = degree  * CV_PI / 180.; // 弧度
//    double a = sin(angle), b = cos(angle);
//    int width = img.cols;
//    int height = img.rows;
//    int width_rotate = int(width * fabs(b)+height * fabs(a));
//    int height_rotate = int(height * fabs(b)+width * fabs(a));
//    if(width_rotate<=20||height_rotate<=20)
//    {
//        width_rotate = 20;
//        height_rotate = 20;
//    }
//    //旋转数组map
//    // [ m0  m1  m2 ] ===>  [ A11  A12   b1 ]
//    // [ m3  m4  m5 ] ===>  [ A21  A22   b2 ]
//    float map[6];
//    Mat map_matrix = Mat(2, 3, CV_32F, map);
//    // 旋转中心
//    Point2f center = Point2f(width / 2, height / 2);
//    Mat map_matrix2 = getRotationMatrix2D(center, degree, 1.0);

//    //Mat img_rotate;
//    //对图像做仿射变换
//    //CV_WARP_FILL_OUTLIERS - 填充所有输出图像的象素。
//    //如果部分象素落在输入图像的边界外，那么它们的值设定为 fillval.
//    //CV_WARP_INVERSE_MAP - 指定 map_matrix 是输出图像到输入图像的反变换，
//    warpAffine(img, imgout, map_matrix2, Size(width, height), 1, 0);
//    return 0;
    float radian = (float) (degree /180.0 * CV_PI);

        //填充图像
        int maxBorder =(int) (max(img.cols, img.rows)* 1.414 ); //即为sqrt(2)*max
        int dx = (maxBorder - img.cols)/2;
        int dy = (maxBorder - img.rows)/2;
        copyMakeBorder(img, imgout, dy, dy, dx, dx, BORDER_CONSTANT);

        //旋转
        Point2f center( (float)(imgout.cols/2) , (float) (imgout.rows/2));
        Mat affine_matrix = getRotationMatrix2D( center, degree, 1.0 );//求得旋转矩阵
        warpAffine(imgout, imgout, affine_matrix, imgout.size());

        //计算图像旋转之后包含图像的最大的矩形
        float sinVal = abs(sin(radian));
        float cosVal = abs(cos(radian));
        Size targetSize( (int)(img.cols * cosVal +img.rows * sinVal),
                         (int)(img.cols * sinVal + img.rows * cosVal) );

        //剪掉多余边框
        int x = (imgout.cols - targetSize.width) / 2;
        int y = (imgout.rows - targetSize.height) / 2;
        Rect rect(x, y, targetSize.width, targetSize.height);
        imgout = Mat(imgout,rect);

        return 0;
}

void get_text_origin(const Point2f &origin, const Point &left_behind_pt, const Point &right_below_pt, Point &text_origin)
{
    int width = right_below_pt.x - left_behind_pt.x;
    int height = right_below_pt.y - left_behind_pt.y;
    text_origin.x = origin.x + left_behind_pt.x;
    text_origin.y = origin.y + left_behind_pt.y;
}

void image_threshold_filter(Mat src_image, Mat &dst_image)
{
    dst_image = src_image.clone();

    for (int i = 0; i < dst_image.cols; i ++){
        dst_image.at<uchar>(0, i) = 0;
        dst_image.at<uchar>(1, i) = 0;
        dst_image.at<uchar>(2, i) = 0;
        dst_image.at<uchar>((dst_image.rows - 1), i) = 0;
        dst_image.at<uchar>((dst_image.rows - 2), i) = 0;
        dst_image.at<uchar>((dst_image.rows - 3), i) = 0;
    }

    for (int i = 0; i < dst_image.rows; i ++){
        dst_image.at<uchar>(i, 0) = 0;
        dst_image.at<uchar>(i, 1) = 0;
        dst_image.at<uchar>(i, 2) = 0;
        dst_image.at<uchar>(i, (dst_image.cols - 1)) = 0;
        dst_image.at<uchar>(i, (dst_image.cols - 2)) = 0;
        dst_image.at<uchar>(i, (dst_image.cols - 3)) = 0;
    }

}

/**
 * @brief src_image_deal
 * //点胶检测器件图像处理，用于后续计算点胶面积
 * @param src_image
 * @param dst_image
 */
void src_image_deal_for_calc_area(const Mat &src_image, Mat &dst_image)
{
    cv::Mat balance_imag;
    Mat gray_imag;
    Mat bin_imag;
    Mat sobel_imag;
    Mat canny_image;
    Mat element = getStructuringElement(MORPH_RECT, Size(3, 3)); //第一个参数MORPH_RECT表示矩形的卷积核，当然还可以选择椭圆形的、交叉型的
    white_balance(src_image, balance_imag);
    cvtColor(balance_imag, gray_imag, COLOR_BGR2GRAY);
    cv::GaussianBlur(gray_imag, gray_imag, Size(3, 3), 0);
    equalizeHist(gray_imag, gray_imag);
    threshold(gray_imag, bin_imag, 180, 255, THRESH_BINARY);
    image_threshold_filter(bin_imag, bin_imag);
    erode(bin_imag, bin_imag, element, Point(-1,-1), 3);
    Canny(bin_imag, canny_image, 70, 200);
    cv::GaussianBlur(canny_image, dst_image, Size(3, 3), 0);
}

void src_image_deal_for_calc_area_debug(const Mat &src_image, Mat &dst_image)
{
    cv::Mat balance_imag;
    Mat gray_imag;
    Mat bin_imag;
    Mat sobel_imag;
    Mat canny_image;
    Mat element = getStructuringElement(MORPH_RECT, Size(3, 3)); //第一个参数MORPH_RECT表示矩形的卷积核，当然还可以选择椭圆形的、交叉型的
    white_balance(src_image, balance_imag);
    cvtColor(balance_imag, gray_imag, COLOR_BGR2GRAY);
    cv::GaussianBlur(gray_imag, gray_imag, Size(3, 3), 0);
    equalizeHist(gray_imag, gray_imag);
    imshow("gray_imag", gray_imag);
    threshold(gray_imag, bin_imag, 180, 255, THRESH_BINARY);
    imshow("threshold", bin_imag);
    image_threshold_filter(bin_imag, bin_imag);
    imshow("image_threshold_filter", bin_imag);
    erode(bin_imag, bin_imag, element, Point(-1,-1), 3);
    imshow("erode", bin_imag);
    Canny(bin_imag, canny_image, 70, 200);
    cv::GaussianBlur(canny_image, dst_image, Size(3, 3), 0);
    imshow("canny_image", canny_image);
}

void feature_match(const Mat &src_image, Mat &dst_image)
{
    cv::Mat image0 = cv::imread("/home/panhan/Desktop/board_B/202101002 (2).jpg", 1);
    cv::Mat image1 = cv::imread("/home/panhan/Desktop/board_B/202101007 (1).jpg", 1);



    cv::Mat temp = image0.clone();
    deal_with_sobel(temp, image0);
    temp = image1.clone();
    deal_with_sobel(temp, image1);
    /*
    step1:特征检测器
    */
//    cv::Ptr<cv::xfeatures2d::SURF> detector;
//    detector = cv::xfeatures2d::SURF::create(800);  //800为海塞矩阵阈值，越大越精准

    cv::Ptr<SiftFeatureDetector> detector;
    detector = SiftFeatureDetector::create(800);//800为保留的点数
    /*
    -----SURF----
    cv::Ptr<cv::xfeatures2d::SURF> detector;
    detector = cv::xfeatures2d::SURF::create(800);  //800为海塞矩阵阈值，越大越精准
    -----SIFT-----
    cv::Ptr<cv::xfeatures2d::SIFT> detector;
    detector = cv::xfeatures2d::SIFT::create(800);//800为保留的点数
    -----ORB------
    cv::Ptr<cv::ORB> detector;
    detector  = cv::ORB::create(800);//保留点数
    -----STAR-----
    cv::Ptr<cv::xfeatures2d::StarDetector> detector;
    detector = cv::xfeatures2d::StarDetector::create();
    -----MSD-----
    cv::Ptr<cv::xfeatures2d::MSDDetector> detector;
    detector = cv::xfeatures2d::MSDDetector::create();
    */
    std::vector <cv::KeyPoint > key0;
    std::vector <cv::KeyPoint > key1;
    detector->detect(image0,key0,cv::noArray());
    detector->detect(image1, key1, cv::noArray());

    /*
    step2:描述子提取器
    */
    cv::Ptr<SiftFeatureDetector> Extractor;
    Extractor = SiftFeatureDetector::create(800);
    /*
       以下都是xfeature2d中的提取器
    -----SURF-----
    -----SIFT-----
    -----LUCID----
    -----BriefDescriptorExtractor----
    -----VGG-----
    -----BoostDesc-----
    */
    cv::Mat descriptor0, descriptor1;
    Extractor->compute(image0, key0, descriptor0);
    Extractor->compute(image1, key1, descriptor1);

    /*
    step3:匹配器
    */
    cv::BFMatcher matcher;//暴力匹配器
    std::vector<cv::DMatch> matches; // 存放匹配结果
    std::vector<cv::DMatch> good_matches; //存放好的匹配结果

    matcher.match(descriptor0, descriptor1, matches);
    std::sort(matches.begin(), matches.end());     //筛选匹配点，根据match里面特征对的距离从小到大排序

    int ptsPairs = std::min(50, (int)(matches.size() * 0.15));
    std::cout << "匹配点数为" << ptsPairs << std::endl;
    for (int i = 0; i < ptsPairs; i++)
    {
        good_matches.push_back(matches[i]);              //距离最小的50个压入新的DMatch
    }

    cv::Mat result;

    cv::drawMatches(image0, key0,
        image1, key1,
        good_matches, result,
        cv::Scalar::all(-1), cv::Scalar::all(-1),
        std::vector<char>(),
        cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);  //绘制匹配点

    pyrDown(result,result);
    pyrDown(result,result);
    cv::imshow("result", result);
}


int highlight_remove_Chi(cv::Mat* src,cv::Mat* dst,double Re)
{
    int height=src->rows;
    int width=src->cols;
    int step=src->step;
    int i=0,j=0;
    unsigned char R,G,B,MaxC;
    double alpha,beta,alpha_r,alpha_g,alpha_b,beta_r,beta_g,beta_b,temp=0,realbeta=0,minalpha=0;
    double gama,gama_r,gama_g,gama_b;
    unsigned char* srcData;
    unsigned char* dstData;
    for (i=0;i<height;i++)
    {
        srcData=(unsigned char*)src->data+i*step;
        dstData=(unsigned char*)dst->data+i*step;
        for (j=0;j<width;j++){
            R=srcData[j*3];
            G=srcData[j*3+1];
            B=srcData[j*3+2];

            alpha_r=(double)R/(double)(R+G+B);
            alpha_g=(double)G/(double)(R+G+B);
            alpha_b=(double)B/(double)(R+G+B);
            alpha=max(max(alpha_r,alpha_g),alpha_b);
            MaxC=max(max(R,G),B);// compute the maximum of the rgb channels
            minalpha=min(min(alpha_r,alpha_g),alpha_b);                beta_r=1-(alpha-alpha_r)/(3*alpha-1);
            beta_g=1-(alpha-alpha_g)/(3*alpha-1);
            beta_b=1-(alpha-alpha_b)/(3*alpha-1);
            beta=max(max(beta_r,beta_g),beta_b);//将beta当做漫反射系数，则有                // gama is used to approximiate the beta
            gama_r=(alpha_r-minalpha)/(1-3*minalpha);
            gama_g=(alpha_g-minalpha)/(1-3*minalpha);
            gama_b=(alpha_b-minalpha)/(1-3*minalpha);
            gama=max(max(gama_r,gama_g),gama_b);

            temp=(gama*(R+G+B)-MaxC)/(3*gama-1);
            //beta=(alpha-minalpha)/(1-3*minalpha)+0.08;
            //temp=(gama*(R+G+B)-MaxC)/(3*gama-1);
            dstData[j*3]=R-(unsigned char)(temp+0.5);
            dstData[j*3+1]=G-(unsigned char)(temp+0.5);
            dstData[j*3+2]=B-(unsigned char)(temp+0.5);
        }
   }
    return 0;
}
