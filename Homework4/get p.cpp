#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>

#include <iostream>

using namespace cv;
using namespace std;

Mat getImageStandard();
void getData(Mat src, Mat imgCanny);

void main() {
	//Mat test = getImageStandard();
	//imshow("test", test);
	
	Mat src = imread("src.png");
	//imshow("src", src);
	Mat gray;
	cvtColor(src, gray, COLOR_BGR2GRAY);
	//imshow("Gray", gray);
	
	//chuyen sang anh nhi phan
	Mat binary;

	//hieu chinh gia tri thresh
	/*int thresh = 0;
	namedWindow("Fix Thresh");
	createTrackbar("Thresh", "Fix Thresh", &thresh, 255);
	while (true)
	{
		threshold(gray, binary, thresh, 255, 0);
		imshow("binary", binary);
		waitKey(1);
	}*/

	threshold(gray, binary, 127, 255, 0); //gia tri 127 lay duoc sau khi hieu chinh
	//imshow("binary", binary);

	//tim duong bien
	Mat imgCanny;
	Canny(binary, imgCanny, 100, 300);
	//imshow("Canny", imgCanny);

	//nhan hinh thai hoc
	getData(src, imgCanny);


	waitKey();
}

Mat getImageStandard() {
	VideoCapture cap(0);
	Mat frame;
	Mat img(frame.size(), CV_8SC3);
	while (cap.isOpened())
	{
		cap.read(frame);			//doc anh tu cam
		flip(frame, frame, 1);		//lat anh

		imshow("Webcam", frame);

		int k = waitKey(25);
		if (k == 's') {
			img = frame.clone();
		}
		if (k == 27) break;
	}
	cap.release();
	
	return img;
}

void getData(Mat src, Mat imgCanny) {
	vector < vector<Point>> contours;
	vector<Vec4i> hierachy;
	findContours(imgCanny, contours, hierachy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	//drawContours(src, contours, -1, Scalar(255, 0, 255));
	//imshow("contour", src);
	for (int i = 0; i < contours.size(); i++)
	{
		int area = contourArea(contours[i]);
		//cout << area << endl;
		vector< vector<Point> > conPoly(contours.size());
		vector<Rect> boundRect(contours.size());
		if (area > 10000)
		{
			float peri = arcLength(contours[i], true);
			approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);
			//drawContours(src, conPoly, i, Scalar(255, 0, 255), 2);
			boundRect[i] = boundingRect(conPoly[i]);
			rectangle(src, boundRect[i].tl(), boundRect[i].br(), Scalar(0, 255, 0), 2);
			cout << boundRect[i].width << ", " << boundRect[i].height << endl;
			//hien thi pixel
			string textW = to_string(boundRect[i].width);
			string textH = to_string(boundRect[i].height);
			string textWxH = "WxH=" + textW + "x" + textH;
			putText(src, textWxH, { boundRect[i].x, boundRect[i].y - 5 }, FONT_HERSHEY_DUPLEX, 1,
				Scalar(0, 255, 0), 1);
		}
	}
	putText(src, "D = 55cm", Point(0, 20), FONT_HERSHEY_DUPLEX, 0.7, Scalar(0, 0, 255), 1);
	imshow("Contour", src);
}

//WxH=249x360, d=55cm