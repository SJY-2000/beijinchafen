#include <iostream>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

int main()
{
	VideoCapture cap(0);
	Mat bgMat;
	Mat frame;
	Mat subMat;
	Mat bny_subMat;
	int cnt = 0;

	if (!cap.isOpened())
	{
		std::cout << "不能打开视频文件" << std::endl;
		return -1;
	}

	while (1)
	{
		cap >> frame;
		cvtColor(frame, frame, COLOR_BGR2GRAY);
		if (cnt == 0)
		{
			frame.copyTo(bgMat);
		}
		else {
			absdiff(frame, bgMat, subMat);
			threshold(subMat, bny_subMat, 50, 255, THRESH_BINARY);
			imshow("bny_subMat", bny_subMat);
			imshow("subMat", subMat);
			waitKey(30);
		}
		cnt++;
	}
}