#include <iostream>
#include <math.h>
#include <opencv.hpp>
using namespace std;
using namespace cv;

void SingleScaleRetinex(const Mat& src, Mat& dst, int sigma);

int main(int argc, char** argv)
{
	Mat src, src_small, src_gray, dst;
	src = imread("D:\\CodeTest IMG\\原图/Image8.tif");
	//src = imread("D:\\CodeTest IMG\\数字图像处理经典照片/X光胸部透视.png");
	if (src.empty())
	{
		cout << "could not load image..." << endl;
		return -1;
	}

	float scale = 0.2;
	int width = static_cast<float>(src.cols * scale);
	int height = static_cast<float>(src.rows * scale);
	resize(src, src_small, cv::Size(width, height));
	namedWindow("small", CV_WINDOW_AUTOSIZE);
	imshow("small", src_small);

	cvtColor(src_small, src_gray, CV_BGR2GRAY);
	namedWindow("gray", CV_WINDOW_AUTOSIZE);
	imshow("gray", src_gray);

	SingleScaleRetinex(src_gray, dst, 0.5);

	namedWindow("output", CV_WINDOW_AUTOSIZE);
	imshow("output", dst);

	waitKey(0);

	return 0;
}

void SingleScaleRetinex(const Mat& src, Mat& dst, int sigma)
{
	Mat doubleImage, gaussianImage, logIImage, logGImage, logRImage;
	//转换范围，所有图像元素增加1.0保证log操作正常,防止溢出
	src.convertTo(doubleImage, CV_64FC3, 1.0, 1.0);
	//高斯模糊，当size为零时将通过sigma自动进行计算
	GaussianBlur(doubleImage, gaussianImage, Size(3, 3), sigma);
	//OpenCV的log函数可以计算出对数值。logIImage和logGImage就是对数计算的结果。
	log(doubleImage, logIImage);
	log(gaussianImage, logGImage);
	//Retinex公式，Log(R(x,y))=Log(I(x,y))-Log(G(x,y)))
	logRImage = logIImage - logGImage;
	//将结果量化到[0,255]范围内，NORM_MINMAX表示线性量化，CV_8UC3表示将图像转回
	normalize(logRImage, dst, 0, 255, NORM_MINMAX, CV_8UC3);

}