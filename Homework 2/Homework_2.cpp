#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

	

void main() {
//tai anh
	Mat src = imread("Resources/tomato.jpg");
	//hien thi anh goc
	resize(src, src, Size(), 0.5, 0.5);
	//imshow("Image", src);
	
	//can bang mau
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
	//imshow("dst", dst);

	

//nhan dien mau qua chin
// H:0 --> 179, S: 0 --> 255, V: 0 --> 255
	int kSize = 0;
	int alpha = 10;
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
	createTrackbar("Blur", "Fix HSV", &kSize, 50); //tao thanh chinh Blur
	//createTrackbar("Contrast", "Fix HSV", &alpha, 25); //tao thanh chinh Blur
	
	
	
	while (true)
	{
		/*Mat srcContrast;
		src.convertTo(srcContrast, -1, (double)alpha/10, 0);
		imshow("Contrast", srcContrast);*/
		

		Mat srcBlur = dst.clone(); //tao bien anh blur
		if (kSize > 0) {
			blur(dst, srcBlur, Size(kSize, kSize));
		}
		imshow("Blur", srcBlur); //hien anh blur
		
		Mat srcHSV; //tao bien anh HSV
		cvtColor(srcBlur, srcHSV, COLOR_BGR2HSV, 0);
		//imshow("image hsv", srcHSV); //hien anh hsv

		//tao mat na loc mau
		Mat imgMask; //tao bien anh mask
		inRange(srcHSV, Scalar(hMin, sMin, vMin), Scalar(hMax, sMax, vMax), imgMask);
		imshow("Image Mask", imgMask); //hien mask
		Mat imgFilter(Size(), CV_8SC3);
		src.copyTo(imgFilter, imgMask);
		imshow("Image Filter", imgFilter);

		cout << hMin << ", " << sMin << ", " << vMin << ", " << hMax << ", " << sMax 
			<< ", " << vMax << ", " << kSize << endl;
		
		Mat imgCanny;
		Canny(imgMask, imgCanny, 100, 200);
		imshow("canny", imgCanny);

		//doi nhan phim de tat
		waitKey(1);
	}
	
}


//73, 0, 0, 179, 255, 255, 29		apple.jpeg 
//0, 121, 173, 179, 255, 255, 47	berry.jpg
//138, 50, 38, 179, 255, 255, 24	berry2.jpg
//0, 184, 28, 179, 255, 255, 28		papaya-fruit
//10, 0, 0, 179, 255, 255, 20		tomato