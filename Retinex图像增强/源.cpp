#include <iostream>
#include <math.h>
#include <opencv.hpp>
using namespace std;
using namespace cv;

void SingleScaleRetinex(const Mat& src, Mat& dst, int sigma);

int main(int argc, char** argv)
{
	Mat src, src_small, src_gray, dst;
	src = imread("D:\\CodeTest IMG\\ԭͼ/Image8.tif");
	//src = imread("D:\\CodeTest IMG\\����ͼ��������Ƭ/X���ز�͸��.png");
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
	//ת����Χ������ͼ��Ԫ������1.0��֤log��������,��ֹ���
	src.convertTo(doubleImage, CV_64FC3, 1.0, 1.0);
	//��˹ģ������sizeΪ��ʱ��ͨ��sigma�Զ����м���
	GaussianBlur(doubleImage, gaussianImage, Size(3, 3), sigma);
	//OpenCV��log�������Լ��������ֵ��logIImage��logGImage���Ƕ�������Ľ����
	log(doubleImage, logIImage);
	log(gaussianImage, logGImage);
	//Retinex��ʽ��Log(R(x,y))=Log(I(x,y))-Log(G(x,y)))
	logRImage = logIImage - logGImage;
	//�����������[0,255]��Χ�ڣ�NORM_MINMAX��ʾ����������CV_8UC3��ʾ��ͼ��ת��
	normalize(logRImage, dst, 0, 255, NORM_MINMAX, CV_8UC3);

}