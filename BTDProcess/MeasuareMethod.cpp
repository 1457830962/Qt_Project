#include "MeasuareMethod.h"
#include <algorithm>
#include <vector>
#include <string>
#include <iostream>
#include <list>
#include <map>
#include <stack>
using namespace std;

class CompGreater
{
public:
    bool operator()(const vector<Point> &v1, const vector<Point> &v2)
    {
        return v1.size() > v2.size();
    }
};

class CompXPosGreater
{
public:
    bool operator()(const vector<Point> &v1, const vector<Point> &v2)
    {
        return (*v1.begin()).x < (*v2.begin()).x;
    }
};

bool PtCompareFun(Point2f p1,Point2f p2)
{
    return p1.x<p2.x;
}


MeasuareMethod::MeasuareMethod()
{
    //x1 = 0.0;
    //y1 = 0.0;
    x0 = 0.0;
    y0 = 0.0;
    x01= y01=0.0;
    x02= y02=0.0;
    x03= y03=0.0;
    x00 = y00 = 0.0;

    x11 = y11 = 0.0;
    x12 = y12 = 0.0;
    x13 = y13 = 0.0;
    x10 = y10 = 0.0;
    ///////////////////////////////////////
    x1=0.0;
    y1=0.0;
    y2 = 0.0;
    ///////////////////////////////////////
    //Angle = 0.0;
    IsShowImg = false;

    for (int i = 0; i<3 ;i++)
        RotorCode[i] = 0;

    ptkeyUpRotor = Point2f(0.0,0.0);
    ptkeyDownRotor = Point2f(0.0,0.0);
}

MeasuareMethod::~MeasuareMethod()
{

}

/**
                  * @brief  process funtion
                  * @author zwg
                  * @param  bin_type: -value shunshizhen  +value nishizhen
                  * @date
                  */
bool MeasuareMethod::GetKeyPt(const String& img_path, double rotate_angle, int bin_type , int pos_type ,int rotor_type)
{

    Mat src = imread(img_path,IMREAD_COLOR);


    if (src.empty())
    {
        cout <<"----- Img file is not exist! -----"<< endl;
        return false;
    }
    if (rotate_angle>0.0001)
    {
        Point2f center(src.cols/2,src.rows/2);    //获取图像中心坐标
        Mat rot = getRotationMatrix2D(center,rotate_angle,1);
        Rect2f bbox = RotatedRect(center,src.size(),rotate_angle).boundingRect();  //最小矩形框
        rot.at<double>(0,2) += bbox.width/2.0 -center.x;
        rot.at<double>(1,2) += bbox.height/2.0 -center.y;
        warpAffine(src,src,rot,bbox.size());
    }


    Mat gray,dst;
    cvtColor(src, gray, COLOR_RGB2GRAY);
    showImg("src",src,0.5);
    showImg("gray",gray,0.5);
    //cvtColor(src, gray, COLOR_RGB2GRAY);   //原图拷贝转为灰度图
    //blur(gray, gray, Size(3, 3));
    BinImg(gray, dst, bin_type);

    switch (pos_type) {
    case 1:
    {
        Mat bin;
        dst.copyTo(bin);
        cvtColor(gray, gray, COLOR_GRAY2BGR);
        GetRectPos(gray, dst, 10);

        GetCode(bin,ptkey,RotorCode);
        break;
    }
    case 2:
    {
        switch (rotor_type) {
        case 1:  //下旋翼
        case 2:  //上旋翼
        {
            Mat bin;
            dst.copyTo(bin);

            cvtColor(dst, dst, COLOR_GRAY2BGR);

            vector<vector<Point>> contours;
            vector<Vec4i> hierarcy;

            findContours(bin, contours, hierarcy, CV_RETR_TREE, CV_CHAIN_APPROX_NONE);
            cout<<"num = "<<contours.size()<<endl;

            Point2f center;  //定义圆中心坐标
            float radius = 0.0l;  //定义圆半径
            sort(contours.begin(), contours.end(), CompGreater());

            //minEnclosingCircle(Mat(contours[0]), center, radius);
            //drawContours(dst, contours, 0, Scalar(0, 0, 255), 1, 8);

            int rotor_index = 0;

            for(size_t i=0; i<contours.size(); i++)  //依次遍历每个轮廓
            {
                minEnclosingCircle(Mat(contours[i]), center, radius);
                if (radius>30&& radius <55)
                {
                    if (rotor_index==0)   ///?????
                    {
                        rotor_index = i;
                        break;
                    }
                }
                radius = 0;
            }

            if (radius <= 0.001)
            {
                cout << "Failed:----- Rotor Circle Not Found! -----" << endl;
                return false;
            }

            Mat rotor_area = Mat::zeros(dst.size(),CV_8UC1);
            Mat rotor_cirle = Mat::zeros(dst.size(),CV_8UC1);
            Mat cross_pts = Mat::zeros(dst.size(),CV_8UC1);

            drawContours(rotor_area, contours, rotor_index, 255, -1, 8,vector<Vec4i>(), 0, Point());
            circle(rotor_cirle, center, radius-5, 255, 1, 8); //绘制最小外接圆
            circle(dst, center, radius-5, 255, 1, 8);
            showImg("rotor_cirle",rotor_cirle,0.5);
            // showImg("rotor_cirle11",dst,0.5); //直接在桨尖标记圆
            //取交点
            for (int i = 0; i < rotor_area.rows;i++)
            {
                for (int j = 0; j < rotor_area.cols;j++)
                {
                    if (rotor_area.at<uchar>(i,j) == 255 && rotor_cirle.at<uchar>(i,j) == 255)
                        cross_pts.at<uchar>(i,j) =255;
                }
            }
            //drawContours(dst, contours, rotor_index, Scalar(0, 0, 255), -1, 8,vector<Vec4i>(), 0, Point());
            //circle(dst, center, radius, Scalar(0, 255, 0), 1, 8);
            cout << "radius = " << radius << endl;
            // imshow("rotor_area", rotor_area);
            // imshow("rotor_cirle", rotor_cirle);
            // imshow("cross_pts",cross_pts);
            vector<vector<Point>> v_crosspts;
            vector<Vec4i> hierarcyedge;
            findContours(cross_pts, v_crosspts, hierarcyedge, CV_RETR_TREE, CV_CHAIN_APPROX_NONE);
            cout << "crosspts:" <<v_crosspts.size()<<endl;    //输出交点的个数
            showImg("cross_pts",cross_pts, 0.5);
            Point2d p_start,p_end;
            if (v_crosspts.size() >= 2)
            {
                sort(v_crosspts.begin(), v_crosspts.end(), CompXPosGreater());
                if (v_crosspts.size() > 2)
                {
                    cout <<"v_crosspts.size() is more than 2 element!" <<endl;
                    p_start = AverVecPt(v_crosspts[0]);
                    p_end = AverVecPt(v_crosspts[v_crosspts.size()-1]);
                }
                else
                {
                    p_start= AverVecPt(v_crosspts.at(0));
                    p_end= AverVecPt(v_crosspts.at(1));
                }
                cout <<"p_start:(" <<p_start.x <<","<<p_start.y<<")"<<endl;
                cout <<"p_end:(" <<p_end.x <<","<<p_end.y<<")"<<endl;
                Point2f p_start1,p_end1;
                p_start1.x = p_start.x;
                p_start1.y = p_start.y;
                p_end1.x = p_end.x;
                p_end1.y = p_end.y;

                Point2f keypt;
                switch (rotor_type) {
                case 1:  //下旋翼
                    keypt = GetDefiniteProportion(p_start1,p_end1,1.0/3.0);//取桨尖大头四分之一处
                    ptkey[0] = GetDefiniteProportion(p_start1,p_end1,1.0/3.0);
                    ptkey[1] = GetDefiniteProportion(p_start1,p_end1,1.0);
                    break;
                case 2:  //上旋翼
                    keypt = GetDefiniteProportion(p_start1,p_end1,3.0);
                    ptkey[0] = GetDefiniteProportion(p_start1,p_end1,1.0);
                    ptkey[1] = GetDefiniteProportion(p_start1,p_end1,3.0);
                    break;
                default:
                    break;
                }

                x0=keypt.x;
                y0=keypt.y;
                cout <<"keypt:(" <<keypt.x <<","<<keypt.y<<")"<<endl;
                // ptkey[2] = GetDefiniteProportion(p_start1,p_end1,3.0);
                cout << "1/4 x=" <<ptkey[0].x <<" y="<<ptkey[0].y <<endl;
                cout << "1/2 x=" <<ptkey[1].x <<" y="<<ptkey[1].y <<endl;
                // cout << "3/4 x=" <<ptkey[2].x <<" y="<<ptkey[2].y <<endl;
                GetCode1(bin,ptkey,RotorCode);   //注意单旋翼和双旋翼区别
                if (RotorCode[0] == false)
                {
                    if (RotorCode[1] == false)
                    {
                        x00 = keypt.x;
                        y00 = keypt.y;
                    }
                    else
                    {
                        x01 = keypt.x;
                        y01 = keypt.y;
                    }
                }
                else{
                    if (RotorCode[1] == false)
                    {
                        x02 = keypt.x;
                        y02 = keypt.y;
                    }
                    else
                    {
                        x03 = keypt.x;
                        y03 = keypt.y;
                    }
                }

                circle(dst, ptkey[0], 3, Scalar(255, 0, 0), -1, 8);  //绘制最小外接圆
                circle(dst, ptkey[1], 3, Scalar(255, 0, 0), -1, 8);  //绘制最小外接圆
                //  circle(dst, ptkey[2], 3, Scalar(255, 0, 0), -1, 8);  //绘制最小外接圆
                circle(dst, p_start1, 3, Scalar(0, 0, 255), -1, 8);  //绘制最小外接圆
                circle(dst, p_end1, 3, Scalar(0, 0, 255), -1, 8);  //绘制最小外接圆
                circle(dst, keypt, 3, Scalar(0, 255, 0), -1, 8);  //绘制最小外接圆
                //imshow("dst",dst);
            }
            else
                cout <<"v_crosspts.size() is less than 2 element!" <<endl;
        }
            break;
        case 3:   // up  and down rotor
        {
            Mat bin;
            dst.copyTo(bin);
            cvtColor(dst, dst, COLOR_GRAY2BGR);
            // imshow("bin",bin);
            //imshow("dst",dst);
            vector<vector<Point>> contours;
            vector<Vec4i> hierarcy;
            findContours(bin, contours, hierarcy, CV_RETR_TREE, CV_CHAIN_APPROX_NONE);
            cout<<"num = "<<contours.size()<<endl;
            sort(contours.begin(), contours.end(), CompGreater());
            Point2f center;  //定义圆中心坐标
            float radius = 0.0l;  //定义圆半径
            Point2f center1;  //定义圆中心坐标
            Point2f center2;  //定义圆中心坐标
            float radius1 = 0.0l;
            float radius2 = 0.0l;
            int rotor_index1 = -1;
            int rotor_index2 = -1;

            for(size_t i=0; i<contours.size(); i++)  //依次遍历每个轮廓
            {
                minEnclosingCircle(Mat(contours[i]), center, radius);
                if (radius>25&& radius <55)
                {
                    if (rotor_index1 == -1)
                    {
                        rotor_index1 = i;
                        radius1 = radius;
                        center1 = center;
                    }
                    else if(rotor_index2 == -1)
                    {
                        rotor_index2 = i;
                        radius2 = radius;
                        center2 = center;
                        break;
                    }
                }
                radius = 0;
            }

            if (radius1 <= 0 || radius2 <=0)
            {
                y2 = 0.1;
                cout << "Failed:----- Rotor Circle Not Found! -----" << endl;
                return false;
            }

            Point2f pt_start1,pt_end1;
            Point2f pt_start2,pt_end2;
            GetTwoEndPt(dst, contours, rotor_index1, center1, radius1, pt_start1, pt_end1);
            GetTwoEndPt1(dst, contours, rotor_index2, center2, radius2, pt_start2, pt_end2);
            Point2f pt_up_start,pt_up_end;
            Point2f pt_down_start,pt_down_end;
            if(pt_start1.y<pt_start2.y)
            {
                pt_up_start = pt_start1;
                pt_up_end = pt_end1;
                pt_down_start = pt_start2;
                pt_down_end = pt_end2;
            }
            else
            {
                pt_up_start = pt_start2;
                pt_up_end = pt_end2;
                pt_down_start = pt_start1;
                pt_down_end = pt_end1;
            }
            ptkeyUpRotor = GetDefiniteProportion(pt_up_start,pt_up_end,3.0);       //上旋翼3/4处
            ptkeyDownRotor = GetDefiniteProportion(pt_down_start,pt_down_end,1.0/3.0);   //下旋翼1/4处
            //            ptkeyDownRotor = GetDefiniteProportion(pt_down_start,pt_down_end,3.0);     //下旋翼3/4处
            ///////////////////////////////////////////////////////////
            x0=ptkeyUpRotor.x;
            y0=ptkeyUpRotor.y;
            x1=ptkeyDownRotor.x;
            y1=ptkeyDownRotor.y;
            y2 = y1 - y0;  //输出上下旋翼桨尖间距
            //提取桨尖标记点
            ptkey[0] = GetDefiniteProportion(pt_up_start,pt_up_end,1.0);
            ptkey[1] = GetDefiniteProportion(pt_up_start,pt_up_end,3.0);
            ptkey[2] = GetDefiniteProportion(pt_down_start,pt_down_end,1.0/3.0);
            ptkey[3] = GetDefiniteProportion(pt_down_start,pt_down_end,1.0);
            //            ptkey[4] = GetDefiniteProportion(pt_down_start,pt_down_end,1.0);
            //            ptkey[5] = GetDefiniteProportion(pt_down_start,pt_down_end,3.0);
            //        case 1:  //下旋翼
            //            keypt = GetDefiniteProportion(p_start1,p_end1,1.0/3.0);//取桨尖大头四分之一处
            //            ptkey[0] = GetDefiniteProportion(p_start1,p_end1,1.0/3.0);
            //            ptkey[1] = GetDefiniteProportion(p_start1,p_end1,1.0);
            //            break;
            //        case 2:  //上旋翼
            //            keypt = GetDefiniteProportion(p_start1,p_end1,3.0);
            //            ptkey[0] = GetDefiniteProportion(p_start1,p_end1,1.0);
            //            ptkey[1] = GetDefiniteProportion(p_start1,p_end1,3.0);

            //            cout << "1/4 x=" <<ptkey[0].x <<" y="<<ptkey[0].y <<endl;
            //            cout << "1/2 x=" <<ptkey[1].x <<" y="<<ptkey[1].y <<endl;
            //            cout << "3/4 x=" <<ptkey[2].x <<" y="<<ptkey[2].y <<endl;
            GetCode(bin,ptkey,RotorCode);
            //上旋翼
            if (RotorCode[0] == false)
            {
                if (RotorCode[1] == false)
                {
                    x00 = ptkeyUpRotor.x;
                    y00 = ptkeyUpRotor.y;
                }
                else
                {
                    x01 = ptkeyUpRotor.x;
                    y01 = ptkeyUpRotor.y;
                }
            }
            else{
                if (RotorCode[1] == false)
                {
                    x02 = ptkeyUpRotor.x;
                    y02 = ptkeyUpRotor.y;
                }
                else
                {
                    x03 = ptkeyUpRotor.x;
                    y03 = ptkeyUpRotor.y;
                }
            }

            //下旋翼
            if (RotorCode[2] == false)
            {
                if (RotorCode[3] == false)
                {
                    x10 = ptkeyDownRotor.x;
                    y10 = ptkeyDownRotor.y;
                }
                else
                {
                    x11 = ptkeyDownRotor.x;
                    y11 = ptkeyDownRotor.y;
                }
            }
            else{
                if (RotorCode[3] == false)
                {
                    x12 = ptkeyDownRotor.x;
                    y12 = ptkeyDownRotor.y;
                }
                else
                {
                    x13 = ptkeyDownRotor.x;
                    y13 = ptkeyDownRotor.y;
                }
            }
            ////////////////////////////////////////////////////////
            //             cout <<"ptkeyUpRotor:(" <<ptkeyUpRotor.x <<","<<ptkeyUpRotor.y<<")"<<endl;
            //             cout <<"ptkeyDownRotor:(" <<ptkeyDownRotor.x <<","<<ptkeyDownRotor.y<<")"<<endl;
            //////////////////////////////////////////////////////////////////////////////
            circle(dst, ptkeyUpRotor, 3, Scalar(0, 255, 0), -1, 8);  //绘制最小外接圆
            circle(dst, ptkeyDownRotor, 3, Scalar(0, 255, 0), -1, 8);  //绘制最小外接圆
            //imshow("color_dst",dst);
        }
            break;
        default:
            break;
        }
        //imwrite("rotor_area.bmp",rotor_area);
        break;
    }
    default:
        cout <<"pos_type is not exist!" <<endl;
        break;
    }
    return true;
}

void MeasuareMethod::showImg(const String &window_name, const Mat &mat, double scale)
{
    if (IsShowImg == true)
    {
        Mat scale_mat;
        namedWindow(window_name, CV_WINDOW_AUTOSIZE);
        resize(mat, scale_mat, Size(), scale, scale);
        imshow(window_name,scale_mat);
    }
}

void MeasuareMethod::MatDrawRect(Mat &mat, const vector<Point2f> &rect_points,const Scalar &color)
{
    line(mat, rect_points.at(0), rect_points.at(1), color, 1, 8);
    line(mat, rect_points.at(0), rect_points.at(2), color, 1, 8);
    line(mat, rect_points.at(3), rect_points.at(1), color, 1, 8);
    line(mat, rect_points.at(3), rect_points.at(2), color, 1, 8);
}

void MeasuareMethod::GetRectPos(Mat &src, Mat& src_gray, int thresh)
{
    Mat threshold_output;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

    /// 阈值化检测边界
    threshold(src_gray, threshold_output, thresh, 255, THRESH_BINARY);
    imshow("threshold_output",threshold_output);

    /// 寻找轮廓
    findContours(threshold_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
    /*---------------------------------- 轮廓大小自定义排序 --------------------------------*/
    sort(contours.begin(), contours.end(), CompGreater());
    /*------------------------------------------------------------------------------*/
    /// 对每个找到的轮廓创建可倾斜的边界框
    if (contours.size()<1)
        return;

    vector<RotatedRect> minRect(contours.size());

    minRect[0] = minAreaRect(Mat(contours[0]));
    double RotatedRectAngle = minRect[0].angle;
    cout <<"RotatedRectAngle = "<<RotatedRectAngle<< endl;

    //    for (size_t i = 0; i < contours.size(); i++)
    //	{
    //		minRect[i] = minAreaRect(Mat(contours[i]));
    //		//if (contours[i].size() > 5)
    //		//{
    //		//	minEllipse[i] = fitEllipse(Mat(contours[i]));
    //		//}
    //	}

    /// 绘出轮廓及其可倾斜的边界框和边界椭圆
    Mat drawing = Mat::zeros(threshold_output.size(), CV_8UC3);


    RNG rng(12345);
    Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));

    /// 桨尖轮廓
    drawContours(drawing, contours, 0, color, 1, 8, vector<Vec4i>(), 0, Point());
    drawContours(src, contours, 0, color, 1, 8, vector<Vec4i>(), 0, Point());

    //drawContours(drawing, contours, 1, color, 1, 8, vector<Vec4i>(), 0, Point());
    //drawContours(src, contours, 1, color, 1, 8, vector<Vec4i>(), 0, Point());

    /// 外接矩形轮廓
    Point2f rect_points0[4];
    minRect[0].points(rect_points0);
    vector<Point2f> rect_points(rect_points0, rect_points0+sizeof(rect_points0)/sizeof(Point2f));

    //Point2f rect_points1[4];
    //minRect[1].points(rect_points1);

    sort(rect_points.begin(),rect_points.end(),PtCompareFun);

    ///Darw Rect
    MatDrawRect(drawing,rect_points,color);
    MatDrawRect(src,rect_points,color);

    for (size_t j = 0; j < rect_points.size(); j++)
    {
        cout <<"x="<< rect_points[j].x <<" y="<< rect_points[j].y << endl;
        x0 += rect_points0[j].x;
        y0 += rect_points0[j].y;
    }

    x0 = x0 / 4.0;
    y0 = y0 / 4.0;

    Point2f p_left;
    p_left.x = (rect_points.at(0).x + rect_points.at(1).x)/2.0f;
    p_left.y = (rect_points.at(0).y + rect_points.at(1).y)/2.0f;

    Point2f p_right;
    p_right.x = (rect_points.at(2).x + rect_points.at(3).x)/2.0f;
    p_right.y = (rect_points.at(2).y + rect_points.at(3).y)/2.0f;

    ptkey[0] = GetDefiniteProportion(p_left,p_right,1.0/3.0);
    ptkey[1] = GetDefiniteProportion(p_left,p_right,1.0);
    ptkey[2] = GetDefiniteProportion(p_left,p_right,3.0);

    cout << "1/4 x=" <<ptkey[0].x <<" y="<<ptkey[0].y <<endl;
    cout << "1/2 x=" <<ptkey[1].x <<" y="<<ptkey[1].y <<endl;
    cout << "3/4 x=" <<ptkey[2].x <<" y="<<ptkey[2].y <<endl;

    circle(drawing, ptkey[0], 2, Scalar(255, 0, 255), -1);   //绘制中心点
    circle(drawing, ptkey[1], 2, Scalar(0, 255, 255), -1);   //绘制中心点
    circle(drawing, ptkey[2], 2, Scalar(0, 0, 255), -1);   //绘制中心点

    //Angle = atan((ptkey[0].y-ptkey[2].y)/(ptkey[2].x-ptkey[0].x))*180/3.1415;
    //cout <<"Angle = "<<Angle<< endl;

    circle(drawing, Point2f(x0, y0), 2, Scalar(0, 0, 255), -1);   //绘制中心点

    printf("x0=%.3lf,y0=%.3lf\n", x0, y0);

    /// 结果在窗体中显示
    showImg("Contours", drawing,1);
    showImg("Src_pos", src,1);
}

void MeasuareMethod::BinImg(Mat &gray, Mat &dst, int type)
{
    switch (type)
    {
    case 1:
    {
        threshold(gray, dst, 0, 255, CV_THRESH_OTSU);
        showImg("threshold_OTSU",dst,0.5);
        break;
    }
    case 2:
    {
        threshold(gray, gray, 10, 255, THRESH_BINARY);

        for (int i = 0;i<gray.cols;i++)
            for (int j = 0;j<gray.rows;j++)
            {
                if (gray.at<uchar>(j,i)<8)
                    gray.at<uchar>(j,i)=0;
            }
        showImg("threshold_BINARY",gray,0.5);
        adaptiveThreshold(gray, dst, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, 41, 0);
        break;
    }
    default:
        break;
    }
}

Point2f MeasuareMethod::GetDefiniteProportion(Point2f &p_start, Point2f &p_end, double ratio)
{
    Point2f p_ratio;
    p_ratio.x = (p_start.x + ratio * p_end.x)/(1+ratio);
    p_ratio.y = (p_start.y + ratio * p_end.y)/(1+ratio);
    return p_ratio;
}

Scalar MeasuareMethod::icvprGetRandomColor()
{
    uchar r = 255 * (rand()/(1.0 + RAND_MAX));
    uchar g = 255 * (rand()/(1.0 + RAND_MAX));
    uchar b = 255 * (rand()/(1.0 + RAND_MAX));
    return Scalar(b,g,r);
}

void MeasuareMethod::icvprLabelColor(Mat &_labelImg, Mat &_colorLabelImg)
{
    int count=0;
    if (_labelImg.empty() ||
            _labelImg.type() != CV_32SC1)
    {
        return ;
    }

    map<int, Scalar> colors ;

    int rows = _labelImg.rows ;
    int cols = _labelImg.cols ;

    _colorLabelImg.release() ;
    _colorLabelImg.create(rows, cols, CV_8UC3) ;
    _colorLabelImg = Scalar::all(0) ;

    for (int i = 0; i < rows; i++)
    {
        const int* data_src = (int*)_labelImg.ptr<int>(i) ;
        uchar* data_dst = _colorLabelImg.ptr<uchar>(i) ;
        for (int j = 0; j < cols; j++)
        {
            int pixelValue = data_src[j] ;
            if (pixelValue == 2)  //pixelValue是标记号
            {
                count++;//数像素点
                if (colors.count(pixelValue) <= 0)
                {
                    colors[pixelValue] = icvprGetRandomColor() ;
                }
                Scalar color = colors[pixelValue] ;
                *data_dst++ = color[0] ;
                *data_dst++ = color[1] ;
                *data_dst++ = color[2] ;
            }
            else
            {
                data_dst++ ;
                data_dst++ ;
                data_dst++ ;
            }
        }
    }
    cout <<"count = " <<count <<endl;
    //printf("%d",count);
}

void MeasuareMethod::icvprCcaBySeedFill(Mat &_binImg, Mat &_lableImg)
{
    // connected component analysis (4-component)
    // use seed filling algorithm
    // 1. begin with a foreground pixel and push its foreground neighbors into a stack;
    // 2. pop the top pixel on the stack and label it with the same label until the stack is empty
    //
    // foreground pixel: _binImg(x,y) = 1
    // background pixel: _binImg(x,y) = 0


    if (_binImg.empty() ||
            _binImg.type() != CV_8UC1)
    {
        return ;
    }

    _lableImg.release() ;
    _binImg.convertTo(_lableImg, CV_32SC1) ;

    int label = 1 ;  // start by 2

    int rows = _binImg.rows - 1 ;
    int cols = _binImg.cols - 1 ;
    for (int i = 1; i < rows-1; i++)
    {
        int* data= _lableImg.ptr<int>(i) ;
        for (int j = 1; j < cols-1; j++)
        {
            if (data[j] == 0)
            {
                stack<pair<int,int>> neighborPixels ;
                neighborPixels.push(pair<int,int>(i,j)) ;     // pixel position: <i,j>
                ++label ;  // begin with a new label
                while (!neighborPixels.empty())
                {
                    // get the top pixel on the stack and label it with the same label
                    pair<int,int> curPixel = neighborPixels.top() ;
                    int curX = curPixel.first ;
                    int curY = curPixel.second ;
                    _lableImg.at<int>(curX, curY) = label ;

                    // pop the top pixel
                    neighborPixels.pop() ;

                    // push the 4-neighbors (foreground pixels)
                    if (_lableImg.at<int>(curX, curY-1) == 0)
                    {// left pixel
                        neighborPixels.push(pair<int,int>(curX, curY-1)) ;
                    }
                    if (_lableImg.at<int>(curX, curY+1) == 0)
                    {// right pixel
                        neighborPixels.push(pair<int,int>(curX, curY+1)) ;
                    }
                    if (_lableImg.at<int>(curX-1, curY) == 0)
                    {// up pixel
                        neighborPixels.push(pair<int,int>(curX-1, curY)) ;
                    }
                    if (_lableImg.at<int>(curX+1, curY) == 0)
                    {// down pixel
                        neighborPixels.push(pair<int,int>(curX+1, curY)) ;
                    }
                }
            }
        }
    }
}

void MeasuareMethod::GetTwoEndPt(Mat& dst, vector<vector<Point> >&contours, int rotor_index, Point2f center, int radius, Point2f& pt_start, Point2f& pt_end)
{
    Mat rotor_area = Mat::zeros(dst.size(),CV_8UC1);
    Mat rotor_cirle = Mat::zeros(dst.size(),CV_8UC1);
    Mat cross_pts = Mat::zeros(dst.size(),CV_8UC1);

    drawContours(rotor_area, contours, rotor_index, 255, -1, 8,vector<Vec4i>(), 0, Point());
    circle(rotor_cirle, center, radius-5, 255, 1, 8);


    for (int i = 0; i < rotor_area.rows;i++)
    {
        for (int j = 0; j < rotor_area.cols;j++)
        {
            if (rotor_area.at<uchar>(i,j) == 255 && rotor_cirle.at<uchar>(i,j) == 255)
                cross_pts.at<uchar>(i,j) =255;
        }
    }

    //drawContours(dst, contours, rotor_index, Scalar(0, 0, 255), -1, 8,vector<Vec4i>(), 0, Point());
    //circle(dst, center, radius, Scalar(0, 255, 0), 1, 8);  //绘制最小外接圆

    cout << "radius = " << radius << endl;

    //    imshow("rotor_area0", rotor_area);
    //    imshow("rotor_cirle0", rotor_cirle);
    //    imshow("cross_pts0",cross_pts);


    vector<vector<Point>> v_crosspts;
    vector<Vec4i> hierarcyedge;

    findContours(cross_pts, v_crosspts, hierarcyedge, CV_RETR_TREE, CV_CHAIN_APPROX_NONE);
    cout << "crosspts:" <<v_crosspts.size()<<endl;

    if (v_crosspts.size() >= 2)
    {
        sort(v_crosspts.begin(), v_crosspts.end(), CompXPosGreater());
        if (v_crosspts.size() > 2)
        {
            cout <<"v_crosspts.size() is more than 2 element!" <<endl;
            pt_start = AverVecPt(v_crosspts[0]);
            pt_end = AverVecPt(v_crosspts[v_crosspts.size()-1]);
        }
        else
        {
            pt_start= AverVecPt(v_crosspts.at(0));
            pt_end= AverVecPt(v_crosspts.at(1));
        }
        cout <<"pt_start:(" <<pt_start.x <<","<<pt_start.y<<")"<<endl;
        cout <<"pt_end:(" <<pt_end.x <<","<<pt_end.y<<")"<<endl;
    }
    else
        cout <<"v_crosspts.size() is less than 2 element!" <<endl;
}

void MeasuareMethod::GetTwoEndPt1(Mat& dst, vector<vector<Point> >&contours, int rotor_index, Point2f center, int radius, Point2f& pt_start, Point2f& pt_end)
{
    Mat rotor_area = Mat::zeros(dst.size(),CV_8UC1);
    Mat rotor_cirle = Mat::zeros(dst.size(),CV_8UC1);
    Mat cross_pts = Mat::zeros(dst.size(),CV_8UC1);

    drawContours(rotor_area, contours, rotor_index, 255, -1, 8,vector<Vec4i>(), 0, Point());
    circle(rotor_cirle, center, radius-5, 255, 1, 8);


    for (int i = 0; i < rotor_area.rows;i++)
    {
        for (int j = 0; j < rotor_area.cols;j++)
        {
            if (rotor_area.at<uchar>(i,j) == 255 && rotor_cirle.at<uchar>(i,j) == 255)
                cross_pts.at<uchar>(i,j) =255;
        }
    }

    //drawContours(dst, contours, rotor_index, Scalar(0, 0, 255), -1, 8,vector<Vec4i>(), 0, Point());
    //circle(dst, center, radius, Scalar(0, 255, 0), 1, 8);  //绘制最小外接圆
    cout << "radius = " << radius << endl;
    //    imshow("rotor_area1", rotor_area);
    //    imshow("rotor_cirle1", rotor_cirle);
    //    imshow("cross_pts1",cross_pts);
    vector<vector<Point>> v_crosspts;
    vector<Vec4i> hierarcyedge;

    findContours(cross_pts, v_crosspts, hierarcyedge, CV_RETR_TREE, CV_CHAIN_APPROX_NONE);
    cout << "crosspts:" <<v_crosspts.size()<<endl;

    if (v_crosspts.size() >= 2)
    {
        sort(v_crosspts.begin(), v_crosspts.end(), CompXPosGreater());
        if (v_crosspts.size() > 2)
        {
            cout <<"v_crosspts.size() is more than 2 element!" <<endl;
            pt_start = AverVecPt(v_crosspts[0]);
            pt_end = AverVecPt(v_crosspts[v_crosspts.size()-1]);
        }
        else
        {
            pt_start= AverVecPt(v_crosspts.at(0));
            pt_end= AverVecPt(v_crosspts.at(1));
        }
        cout <<"pt_start:(" <<pt_start.x <<","<<pt_start.y<<")"<<endl;
        cout <<"pt_end:(" <<pt_end.x <<","<<pt_end.y<<")"<<endl;
    }
    else
        cout <<"v_crosspts.size() is less than 2 element!" <<endl;
}

void MeasuareMethod::GetMaxArea(Mat &binImage)
{
    threshold(binImage, binImage, 0, 255, CV_THRESH_OTSU);
    threshold(binImage, binImage, 100,255, CV_THRESH_BINARY_INV);

    imshow("binImage", binImage);

    Mat labelImg;
    icvprCcaBySeedFill(binImage, labelImg);
    imshow("labelImg", labelImg);

    Mat colorLabelImg;
    icvprLabelColor(labelImg, colorLabelImg);
    imshow("colorLabelImg", colorLabelImg);
}

Point2d MeasuareMethod::AverVecPt(vector<Point> &vecpt)
{
    if (vecpt.size() == 0)
    {
        cout <<"vecpt.size() < 0!!!"<<endl; //没有取到点
        return Point2d(0,0);
    }

    double x = 0;
    double y = 0;
    for (size_t i = 0; i<vecpt.size(); i++)
    {
        x+=vecpt.at(i).x;
        y+=vecpt.at(i).y;
    }
    x = x/vecpt.size();//取出所有去取到的交点的均值
    y = y/vecpt.size();

    return Point2d(x,y);
}

void MeasuareMethod::GetCode(Mat &bin_mat, Point2f ptkey[], bool RotorCode[])
{
    cout << "bin_mat.rows = " << bin_mat.rows  <<endl;
    cout << "bin_mat.cols = " << bin_mat.cols  <<endl;

    cout << "bin_mat.channels()  = " << bin_mat.channels() <<endl;

    for (int i =0 ;i<4; i++)
    {
        int col = round(ptkey[i].x);
        int row = round(ptkey[i].y);

        //uchar px_value= bin_mat.at<uchar>(row,col);

        int count = 0;

        //circle(bin_mat, ptkey[i], 30, 255, 3, 8);  //绘制最小外接圆

        cout << "px_value:"<<endl;
        for(int m = -1; m<=1 ; m++ )
        {
            for(int n = -1; n<=1 ; n++ )
            {
                int px_value = bin_mat.at<uchar>(row+n,col+m);
                cout <<px_value<<" ";
                if (bin_mat.at<uchar>(row+n,col+m) == 0)
                    count++;
            }
            cout << endl;
        }

        cout << "count = " << count <<endl;

        if (count >= 3)
            RotorCode[i] = true;
        else
            RotorCode[i] = false;
    }

    //imshow("bin_mat",bin_mat);

    cout <<"RotorCode:";
    for (int i=0;i<3;i++)
        cout << RotorCode[i];
    cout << endl;
}


void MeasuareMethod::GetCode1(Mat &bin_mat, Point2f ptkey[], bool RotorCode[])
{
    cout << "bin_mat.rows = " << bin_mat.rows << endl;
    cout << "bin_mat.cols = " << bin_mat.cols << endl;

    cout << "bin_mat.channels()  = " << bin_mat.channels() << endl;

    for (int i = 0; i < 2; i++)
    {
        int row = round(ptkey[i].x);
        int col = round(ptkey[i].y);

        //uchar px_value= bin_mat.at<uchar>(row,col);

        int count = 0;

        cout << "px_value:" << endl;
        for (int m = -1; m <= 1; m++)
        {
            for (int n = -1; n <= 1; n++)
            {
                int px_value = bin_mat.at<uchar>(col + n, row + m);
                cout << px_value << " ";
                if (bin_mat.at<uchar>(col + n, row + m) == 0)
                    count++;
            }
            cout << endl;
        }


        cout << "count = " << count << endl;

        if (count >= 1)
            RotorCode[i] = true;
        else
            RotorCode[i] = false;
    }
    showImg("bin_mat",bin_mat,1);

    cout << "RotorCode:";
    for (int i = 0; i < 2; i++)
        cout << RotorCode[i];
    cout << endl;
}




