#include <iostream>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

Mat averageMat(Mat*srcMat, int cnt)
{
	Mat result = Mat::zeros(srcMat[0].rows, srcMat[0].cols, CV_8UC1);
	for (int i = 0; i < cnt; i++)
	{
		for (int m = 0; m < srcMat[0].rows; m++)
		{
			uchar* data = result.ptr<uchar>(m);
			for (int n = 0; n < srcMat[0].cols; n++)
			{   
				data[n] += (uchar)(srcMat[i].at<uchar>(m, n) / cnt);
			}
		}
	}
	return result;
}

Mat variMat(Mat *srcMat, Mat bgMat_r, int cnt)
{
	Mat result = Mat::zeros(srcMat[0].rows, srcMat[0].cols, CV_8UC1);

	for (int i = 0; i < cnt; i++)
	{
		for (int m = 0; m < srcMat[i].rows; m++)
		{
			uchar* data = result.ptr<uchar>(m);
			for (int n = 0; n < srcMat[i].cols; n++)
			{ 
				data[n] += (uchar)(pow((srcMat[i].at<uchar>(m, n) - bgMat_r.at<uchar>(m, n)), 2) / cnt);
			}
		}
	}
	return result;
}


int main()
{
	VideoCapture cap(0);
	Mat bgMat[20];
	Mat bgMat_r;
	Mat bgMat_v;
	Mat frame; 
	Mat subMat;
	int cnt = 0;
	int frame_cnt = 20;
	int vari = 0;

	
	if (!cap.isOpened())
	{
		std::cout << "不能打开视频文件" << std::endl;
		return -1;
	}

	while (1)
	{
		cap >> frame;
		cvtColor(frame, frame, COLOR_BGR2GRAY);
		medianBlur(frame, frame, 11);
		GaussianBlur(frame, frame, Size(3, 3), 3, 3);
		if (cnt < frame_cnt)
		{
			frame.copyTo(bgMat[cnt]);
		}
		if (cnt == frame_cnt)
		{
			bgMat_r = averageMat(bgMat, frame_cnt);
			bgMat_v = variMat(bgMat, bgMat_r, frame_cnt);
		}
		if (cnt > frame_cnt)
		{
			absdiff(frame, bgMat_r, subMat);
			for (int i = 0; i < frame.rows; i++)
			{
				for (int j = 0; j < frame.cols; j++)
				{
					if (subMat.at<uchar>(i, j) < bgMat_v.at<uchar>(i, j))
					{
						subMat.at<uchar>(i, j) = 0;
					}
					else
					{
						subMat.at<uchar>(i, j) = 255;
					}
				}
			}
			imshow("subMat", subMat);
			waitKey(30);
		}
		cnt++;
	}
}