#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

void main() {
	Mat src = imread("Resources/apple.jpeg");
	imshow("src", src);

	Mat hsv, dst;
	cvtColor(src, hsv, COLOR_BGR2HSV);
	vector<Mat> hsv_channels;
	//tach mau thanh 3 kenh
	split(hsv, hsv_channels);
	//can bang histogram kenh v (value)
	equalizeHist(hsv_channels[2], hsv_channels[2]);
	//tron anh
	merge(hsv_channels, hsv);
	//chuyen anh sang RGB --> hien thi
	cvtColor(hsv, dst, COLOR_HSV2BGR);
	imshow("dst", dst);

	waitKey();
}