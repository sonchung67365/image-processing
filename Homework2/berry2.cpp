#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include <iostream>
#include <string>

using namespace cv;
using namespace std;

//138, 50, 38, 179, 255, 255, 24		berry2.jpg
int hMin = 138, sMin = 50, vMin = 38;
int hMax = 179, sMax = 255, vMax = 255;
int kSize = 25;

void main() {
	//tai anh
	Mat src = imread("Resources/berry2.jpg");
	resize(src, src, Size(), 0.5, 0.5);
	//imshow("SRC", src);

	//loc lay mau do tren anh
	//tao anh mo
	Mat src_blur;
	blur(src, src_blur, Size(kSize, kSize));
	//tao hsv
	Mat src_hsv;
	cvtColor(src_blur, src_hsv, COLOR_BGR2HSV);
	//mat na
	Scalar lower(hMin, sMin, vMin);
	Scalar upper(hMax, sMax, vMax);
	Mat mask;
	inRange(src_hsv, lower, upper, mask);
	//tao anh filter
	Mat filter(Size(), CV_8SC3);
	src.copyTo(filter, mask);
	//lam ro duong bien anh
	medianBlur(filter, filter, 3);
	//hien thi
	//imshow("Filter", filter);
	

	//ve duong vien
	//chuyen ve anh xam
	Mat gray;
	cvtColor(filter, gray, COLOR_BGR2GRAY);
	//chuyen ve anh den trang
	Mat bw;
	threshold(gray, bw, 1, 255, 0);
	//imshow("bw", bw);
	//tao anh canny
	Mat imgCanny;
	Canny(bw, imgCanny, 100, 300);
	//imshow("canny", imgCanny);

	vector< vector<Point> > contours;
	vector<Vec4i> hierarchy;

	findContours(imgCanny, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	Mat dst = src.clone();
	//drawContours(dst, contours, -1, Scalar(255, 0, 255));
	
	//ve hinh vuong
	vector<vector<Point>> polygon(contours.size());
	vector<Rect> boundRect(contours.size());

	
	for (int i = 0; i < contours.size(); i++)
	{
		int area = contourArea(contours[i]);
		if (area > 1000) {
			//int num = 1;
			float peri = arcLength(contours[i], true);
			approxPolyDP(contours[i], polygon[i], 0.001 * peri, true);
			//drawContours(dst, polygon, i, Scalar(255, 0, 255), 2);
			boundRect[i] = boundingRect(polygon[i]);
			//hien hinh vuong obj
			rectangle(dst, boundRect[i].tl(), boundRect[i].br(), Scalar(0, 255, 0), 2);
			//hien chu
			string text = "Qua chin ";
			putText(dst, text, { boundRect[i].x,boundRect[i].y - 5 }, FONT_HERSHEY_PLAIN,
				1.5, Scalar(255, 255, 255), 2);
			
			//num++;
		}

	}
	
	//resize(dst, dst, Size(), 0.5, 0.5);
	imshow("DST", dst);
	//cho nhan phim bat ki
	waitKey();

}