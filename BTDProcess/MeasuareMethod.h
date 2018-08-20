//MeasuareMethod

#pragma once
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <vector>

using namespace cv;
using namespace std;

class MeasuareMethod
{
public:
	float x0, y0;
    float x1,y1;
    float x01, y01;
    float x02, y02;
    float x03, y03;
    float x00, y00;

    float x11, y11;
    float x12, y12;
    float x13, y13;
    float x10, y10;

    float y2;//用于储存上下旋翼桨尖间距
    //double Angle;
    bool IsShowImg;
    //float x1, y1;
    Point2f ptkey[5];
    Point2f ptkeyUpRotor;
    Point2f ptkeyDownRotor;
    bool RotorCode[5];
public:
	MeasuareMethod();
	virtual ~MeasuareMethod();

    bool GetKeyPt(const String& img_path, double rotate_angle, int bin_type,int pos_type,int rotor_type);
    void showImg(const String& window_name,const Mat& mat,double scale = 1);
    void MatDrawRect(Mat &mat,const std::vector<Point2f> &rect_points,const Scalar &color);
    void GetCode(Mat& bin_mat,Point2f ptkey[],bool RotorCode[]);
    void GetCode1(Mat &bin_mat, Point2f ptkey[], bool RotorCode[]);
    void GetMaxArea(Mat& binImage);/////
    Point2d AverVecPt(vector<Point> &vecpt);

private:
	void GetRectPos(Mat &src, Mat& src_gray, int thresh);

    //void GetCirclePos(Mat &src);
	void BinImg(Mat &gray,Mat &dst,int type);
    Point2f GetDefiniteProportion(Point2f &p_start,Point2f &p_end,double ratio);


    void ImgFeatureMatch(const String& img_path1,const String& img_path2);//////
    Scalar icvprGetRandomColor();/////
    void icvprLabelColor(Mat& _labelImg, Mat& _colorLabelImg);//////
    void icvprCcaBySeedFill(Mat& _binImg,Mat& _lableImg);/////
    void GetTwoEndPt(Mat& dst, vector<vector<Point> > &contours,int rotor_index,Point2f center,int radius,Point2f& pt_start,Point2f& pt_end);////
    void GetTwoEndPt1(Mat& dst, vector<vector<Point> > &contours,int rotor_index,Point2f center,int radius,Point2f& pt_start,Point2f& pt_end);
};
