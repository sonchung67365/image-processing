#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>

#include <iostream>

using namespace cv;
using namespace std;

/*Vật tham chiếu có kích thước thật là 175mm, kích thước trên ảnh là 249pixel.
--> Kích thước thật của 1 pixel là: P = kích thước thật / kích thước trên ảnh */
int pixel_width = 249; //pixel
float real_width = 175; //milimet
int d = 550;
float p = real_width / pixel_width;
float f = (pixel_width * d) / real_width;

void getContours(Mat src, Mat imgCanny);

void main() {
	VideoCapture cap(0);
	Mat frame;
	cout << p << endl;
	int thresh = 160;
	namedWindow("Fix Thresh");
	createTrackbar("Thresh", "Fix Thresh", &thresh, 255);

	vector < vector<Point>> contours;
	vector<Vec4i> hierachy;

	while (true)
	{
		cap.read(frame);			//doc anh tu cam
		flip(frame, frame, 1);		//lat anh
		//imshow("Webcam", frame);
		Mat gray;
		cvtColor(frame, gray, COLOR_BGR2GRAY);
		//imshow("Webcam", gray);
		Mat binary;
		threshold(gray, binary, thresh, 255, 0);
		//imshow("binary", binary);
		Mat imgBlur;
		medianBlur(binary, imgBlur, 7);
		//imshow("blur", imgBlur);
		Mat imgCanny;
		Canny(imgBlur, imgCanny, 100, 300);
		//imshow("Canny", imgCanny);
		
		findContours(imgCanny, contours, hierachy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
		drawContours(frame, contours, -1, Scalar(255, 0, 255));
		//imshow("contour", src);

		//find rotated rect
		vector<RotatedRect> minRect(contours.size());

		for (int i = 0; i < contours.size(); i++)
		{
			int area = contourArea(contours[i]);
			//cout << area << endl;
			vector< vector<Point> > conPoly(contours.size());
			vector<Rect> boundRect(contours.size());
			
			if (area > 10000)
			{
				float peri = arcLength(contours[i], true);
				approxPolyDP(contours[i], conPoly[i], 0.005 * peri, true);
				drawContours(frame, conPoly, i, Scalar(255, 0, 255), 2);
				boundRect[i] = boundingRect(conPoly[i]);
				rectangle(frame, boundRect[i].tl(), boundRect[i].br(), Scalar(0, 255, 0), 2);
				minRect[i] = minAreaRect(conPoly[i]);
				Point2f rect_points[4];
				minRect[i].points(rect_points);
				for (int j = 0; j < 4; j++)
					line(frame, rect_points[j], rect_points[(j + 1) % 4], Scalar(0, 0, 255), 2);
				//hien thi pixel
				string textW = to_string(p * boundRect[i].width);
				string textH = to_string(p * boundRect[i].height);
				string textWxH = "WxH=" + textW + "x" + textH;
				string textD = to_string(real_width * f / boundRect[i].width);
				putText(frame, textWxH, { boundRect[i].x, boundRect[i].y - 5 }, FONT_HERSHEY_DUPLEX, 1,
					Scalar(0, 255, 0), 1);
				putText(frame, textD, { 0, 20 }, FONT_HERSHEY_DUPLEX, 0.7,
					Scalar(0, 255, 0), 1);
			}
		}
		//resize(frame, frame, Size(), 1.5, 1.5);
		imshow("DST", frame);

		int k = waitKey(25);
		if (k == 27) break;
	}
}

void getContours(Mat src, Mat imgCanny) {
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
		if (area > 1000)
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
	imshow("Contour", src);
}