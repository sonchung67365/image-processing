#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>


using namespace std;
using namespace cv;

Mat chiaMau(Mat src, int k) {
	vector<Mat> rbg_channels;
	//tach mau
	split(src, rbg_channels);
	//
	if (k == 0) {
		rbg_channels[1] = 0;
		rbg_channels[2] = 0;
	}
	if (k == 1)
	{
		rbg_channels[0] = 0;
		rbg_channels[2] = 0;
	}
	if (k == 2)
	{
		rbg_channels[0] = 0;
		rbg_channels[1] = 0;
	}
	Mat dst;
	merge(rbg_channels, dst);
	return dst;
}

void main() {
	string img_path = "Resources/60da7c6a1adfe.jpg";
	//string img_path = "Resources/Untitled.png";
	Mat src = imread(img_path);
	string kt = to_string(src.cols) + "x" + to_string(src.rows);
	putText(src, kt, Point(0, 30), FONT_HERSHEY_COMPLEX, 1, Scalar(255, 255, 255), 1);
	resize(src, src, Size(), 0.7, 0.7);
	imshow("SRC", src);

	cout << src.size()<< endl;
	cout << src.cols << endl;
	//cout << kt << endl;
	
	

	Mat blue = chiaMau(src, 0);
	imshow("Blue", blue);
	Mat green = chiaMau(src, 1);
	imshow("Green", green);
	Mat red = chiaMau(src, 2);
	imshow("Red", red);


	waitKey();
}