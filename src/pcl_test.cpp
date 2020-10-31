
#include<opencv2/core.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>
using namespace cv;
#include<iostream>
#include<string>
using namespace std;
#include <opencv2/imgproc/types_c.h>
#include<opencv2/opencv.hpp>
Mat src1,gray_src,src2,dst;
int main()
{
	src1 = imread("/home/steven/Desktop/HSV_inRange-2.jpg");
	Mat rawImg =imread("/home/steven/Desktop/2_Color.png");
	if (src1.empty())
	{
		cout << "could not find src1" << endl;
		return -1;
	}
	namedWindow("input_HoughLine", WINDOW_AUTOSIZE);
	imshow("input_HoughLine", src1);

    /* Mat kernel = getStructuringElement(MORPH_RECT,(3,3));
    Mat getStructuringElement(int shape, Size esize, Point anchor=Point(-1,-1)); */

    int i = 2;
    Mat element = getStructuringElement( MORPH_ELLIPSE,Size( 2*i + 1, 2*i+1 ), Point(i, i ) );
    /// 腐蚀操作
    erode( src1, src1, element );
    dilate( src1, src1, element );
    namedWindow("Er&Di");
    namedWindow("RAW");

	//霍夫直线   canny边缘提取 --> cvtcolor转灰度 --> HoughLinesP霍夫边缘检测
	cvtColor(src1,src1,CV_BGR2GRAY);
	Canny(src1,gray_src,100,200,3,false);

    //gray_src = src1.clone();
	cvtColor(gray_src,dst,CV_GRAY2BGR);
	vector<Vec4f> lines;
	/* 
	生成极坐标时候的像素（length）扫描步长
	'CV_PI/180'生成极坐标时候的角度步长
	累加器阈值
	最小直线长度
	最大间隔（能构成一条直线）  
	*/
	HoughLinesP(gray_src,lines,0.5,CV_PI/180,80,50.0,800.0);
	Scalar color = Scalar(0,0,255);
	Scalar color1 = Scalar(0,255,0);
    int cntLine = 0;
    cvtColor(src1,src1,CV_GRAY2BGR);
	for (size_t i = 0; i < lines.size(); i++)
	{
        cntLine ++;
		Vec4f plines=lines[i];  //一个plines里边是四个点一条直线    
		if(i == 2)
		{
			line(src1,Point(plines[0],plines[1]),Point(plines[2],plines[3]),color1,1,LINE_AA);	
			line(rawImg,Point(plines[0],plines[1]),Point(plines[2],plines[3]),color1,1,LINE_AA);
			cout << "第" << cntLine << "条线为：\t" << plines[0] << "," << plines[1] 
       			 << "\t" << plines[2] << "," << plines[3] << endl;
			continue;		
		}
		//rawImg                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       
	    line(dst,Point(plines[0],plines[1]),Point(plines[2],plines[3]),color,1,LINE_AA);
        line(src1,Point(plines[0],plines[1]),Point(plines[2],plines[3]),color,1,LINE_AA);
		line(rawImg,Point(plines[0],plines[1]),Point(plines[2],plines[3]),color,1,LINE_AA);

        cout << "第" << cntLine << "条线为：\t" << plines[0] << "," << plines[1] 
        << "\t" << plines[2] << "," << plines[3] << endl;
	}

	//测试DDA方法

	imshow("RAW",    rawImg);
	imshow("output", dst);
    imshow("Er&Di",	 src1);

	waitKey(0);
	return 0;
}