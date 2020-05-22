#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

bool gussBG(vector<Mat> srcMat, Mat &mMat, Mat &vMat)
{
	int row = srcMat[0].rows;
	int col = srcMat[0].cols;
	int Matsize = srcMat.size();
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			int sum = 0;
			float var = 0;
			for (int m = 0; m < Matsize; m++)
			{
				sum += srcMat[m].at<uchar>(i, j);
			}
			mMat.at<uchar>(i, j) = sum / Matsize;
			for (int n = 0; n < Matsize; n++)
			{
				var += pow((srcMat[n].at<uchar>(i, j) - mMat.at<uchar>(i, j)),2);
			}
			vMat.at<float>(i, j) = var / Matsize;
		}
	}
	return true;
}

bool gussTh(Mat frame, Mat mMat, Mat vMat, float v, Mat &dst)
{
	int row = frame.rows;
	int col = frame.cols;
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			int x = abs(frame.at<uchar>(i, j) - mMat.at<uchar>(i, j));
			int th = v * vMat.at<float>(i, j);
			if (x > th)
				dst.at<uchar>(i, j) = 255;
			else
				dst.at<uchar>(i, j) = 0;
		}
	}
	return true;
}
int main()
{
	VideoCapture vc(0);
	Mat frame, bg, sub, bny_sub;
	vector<Mat> srcMat;
	Mat mMat;
	Mat vMat;
	Mat dst;
	int cnt = 0;
	int nBG = 50;
	float v = 2.5;  //ШЈжи
	while (1)
	{
		vc >> frame;
		cvtColor(frame, frame, COLOR_BGR2GRAY);
		Size fsize = frame.size();

		if(cnt < nBG)
		{
			srcMat.push_back(frame);
		}

		else if (cnt == nBG)
		{
			mMat.create(fsize, CV_8UC1);
			vMat.create(fsize, CV_32FC1);
			gussBG(srcMat, mMat, vMat);
		}
		else
		{
			dst.create(fsize, CV_8UC1);
			gussTh(frame, mMat, vMat, v, dst);
			imshow("res", dst);
			imshow("frame", frame);
			imshow("m", mMat);
			waitKey(30);
		}
		cnt++;
		/*
		if (cnt == 0)
		{
			frame.copyTo(bg);
		}
		else
		{
			absdiff(frame, bg, sub);
			threshold(sub, bny_sub, 50, 255, THRESH_BINARY);
			imshow("bny", bny_sub);
			imshow("sub", sub);
			waitKey(30);
		}
		cnt++;
		*/
	}

	return 0;
}
