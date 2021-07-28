#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include <iostream>

using namespace cv;
using namespace std;

int fontFace = FONT_HERSHEY_PLAIN;
double fontScale = 1;
int thickness = 1;

void main() {
	Mat src = imread("Resources/detect-simple-shapes-src-img.png");
	//resize(src, src, Size(), 2, 2);
	imshow("SRC", src);

	//chuyen doi anh xam
	Mat gray;
	cvtColor(src, gray, COLOR_BGR2GRAY);
	//imshow("gray", gray);
	Mat srcBlur;
	GaussianBlur(gray, srcBlur, Size(3, 3), 3, 0);
	//lay duong canny
	Mat imgCanny;
	Canny(srcBlur, imgCanny, 25, 75);
	//imshow("canny", imgCanny);

	//ve duong bao doi tuong
	vector<vector<Point>> contours;
	vector<Vec4i> hierachy;
	findContours(imgCanny, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	Mat contour = src.clone();
	//drawContours(contour, contours, -1, Scalar(255, 255, 255), 2);
	//imshow("Contour", contour);

	//ve duong bao Rect doi tuong
	vector<vector<Point>> polygon(contours.size());
	vector<Rect> boundRect(contours.size());
	string objText;
	Point objCenter(0, 0);
	for (int i = 0; i < contours.size(); i++)
	{
		float peri = arcLength(contours[i], true);
		approxPolyDP(contours[i], polygon[i], 0.02 * peri, true);
		//drawContours(src, polygon, i, Scalar(255, 255, 255), 2);
		//lay toa do tam doi tuong
		boundRect[i] = boundingRect(polygon[i]);
		//rectangle(src, boundRect[i].tl(), boundRect[i].br(), Scalar(0, 255, 0), 2);
		objCenter.x = boundRect[i].x + boundRect[i].width / 2;
		objCenter.y = boundRect[i].y + boundRect[i].height / 2;
		//xac dinh hinh
		int objEdge = (int)polygon[i].size();
		if (objEdge == 3) objText = "TRI";
		if (objEdge == 4)
		{
			float ratio = (float)boundRect[i].width / (float)boundRect[i].height;
			if (ratio > 0.95 && ratio < 1.05) objText = "SQU";
			else objText = "RECT";
		}
		if (objEdge == 5) objText = "PENTA";
		if (objEdge == 6) objText = "HEXA";
		if (objEdge > 6) objText = "CIR";
		//viet chu
		int baseline = 0;
		Size objTextSize = getTextSize(objText, fontFace, fontScale, thickness, &baseline); //lay kich thuoc chu
		Point objPText(objCenter.x - objTextSize.width / 2, objCenter.y + objTextSize.height / 2);
		Point recTL = objPText + Point(-1, 1);
		Point recBR = objPText + Point(objTextSize.width, -objTextSize.height) + Point(1, -3);
		rectangle(src, recTL, recBR, Scalar(255,255,255), FILLED);
		putText(src, objText, { objPText.x, objPText.y }, fontFace, fontScale, Scalar(0, 0, 0), thickness); //hien chu
		
		//cout << polygon[i].size() << endl;
	}
	imshow("DST", src);

	waitKey();
}