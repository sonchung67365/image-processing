#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

void main() {
	Mat src = imread("Resources/apple.jpeg");
	imshow("src", src);

	int hMin = 0, sMin = 0, vMin = 0;
	int hMax = 179, sMax = 255, vMax = 255;
	//tao cua so chinh HSV
	namedWindow("Fix HSV", WINDOW_NORMAL); //tao cua so
	createTrackbar("Hue Min", "Fix HSV", &hMin, 179); //tao thanh chinh Hue Min
	createTrackbar("Saturation Min", "Fix HSV", &sMin, 255); //tao thanh chinh Saturation Min
	createTrackbar("Value Min", "Fix HSV", &vMin, 255); //tao thanh chinh Value Min
	createTrackbar("Hue Max", "Fix HSV", &hMin, 179); //tao thanh chinh Hue Max
	createTrackbar("Saturation Max", "Fix HSV", &sMin, 255); //tao thanh chinh Saturation Max
	createTrackbar("Value Max", "Fix HSV", &vMin, 255); //tao thanh chinh Value Max
	//createTrackbar("Blur", "Fix HSV", &kSize, 255); //tao thanh chinh Blur
	//createTrackbar("Contrast", "Fix HSV", &alpha, 25); //tao thanh chinh Blur

	
	//vector<Mat> bgr_channels;
	////tach mau thanh 3 kenh
	//split(src, bgr_channels);
	////can bang histogram kenh v (value)
	//bgr_channels[0] += 255;
	//bgr_channels[1] += 255;
	////tron anh
	////Mat src_cv = src.clone();
	//merge(bgr_channels, src);
	////hien thi anh
	//imshow("src_cv", src);

	Mat srcHSV; //tao bien anh HSV
	cvtColor(src, srcHSV, COLOR_BGR2HSV, 0);
	imshow("HSV", srcHSV);

	while (true)
	{
		Mat imgMask; //tao bien anh mask
		inRange(srcHSV, Scalar(hMin, sMin, vMin), Scalar(hMax, sMax, vMax), imgMask);
		imshow("Image Mask", imgMask); //hien mask
		waitKey();
	}

	
}