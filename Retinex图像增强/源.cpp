#include <iostream>
#include <math.h>
#include <opencv.hpp>
using namespace std;
using namespace cv;

void SingleScaleRetinex(int, void*);

Mat src, src_small, src_gray, dst;
int Gsize = 2;
int Gsigma = 5;

int main(int argc, char** argv)
{
	src = imread("D:\\CodeTest IMG\\ԭͼ/Image4.tif");
	//src = imread("D:\\CodeTest IMG\\����ͼ��������Ƭ/LENA.bmp");
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

	//namedWindow("��˹ģ��", CV_WINDOW_AUTOSIZE);
	namedWindow("output", CV_WINDOW_AUTOSIZE);
	createTrackbar("Gsize", "output", &Gsize, 15, SingleScaleRetinex);
	createTrackbar("Gsigma", "output", &Gsigma, 15, SingleScaleRetinex);
	SingleScaleRetinex(0, 0);

	
	//imshow("output", dst);

	waitKey(0);

	return 0;
}

void SingleScaleRetinex(int, void*)
{
	Mat doubleImage, gaussianImage, logIImage, logGImage, logRImage;
	//ת����Χ������ͼ��Ԫ������1.0��֤log��������,��ֹ���
	src_gray.convertTo(doubleImage, CV_64FC3, 1.0, 1.0);

	//��˹ģ������sizeΪ��ʱ��ͨ��sigma�Զ����м���
	GaussianBlur(doubleImage, gaussianImage, Size(Gsize * 2 + 1, Gsize * 2 + 1), Gsigma * 2 + 1);
	//OpenCV��log�������Լ��������ֵ��logIImage��logGImage���Ƕ�������Ľ����
	log(doubleImage, logIImage);
	log(gaussianImage, logGImage);
	//Retinex��ʽ��Log(R(x,y))=Log(I(x,y))-Log(G(x,y)))
	logRImage = logIImage - logGImage;
	//�����������[0,255]��Χ�ڣ�NORM_MINMAX��ʾ����������CV_8UC3��ʾ��ͼ��ת��
	//exp(logRImage, logRImage);
	//normalize(logIImage, logIImage, 0, 255, NORM_MINMAX, CV_8UC1);
	//normalize(logGImage, logGImage, 0, 255, NORM_MINMAX, CV_8UC1);
	//imshow("ԭͼ����", logIImage);
	//imshow("��˹ģ��", logGImage);
	//imshow("����ͼ��", logRImage);
	normalize(logRImage, dst, 0, 255, NORM_MINMAX, CV_8UC1);
	imshow("output", dst);
}