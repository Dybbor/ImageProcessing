#pragma once
#include <iostream>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

int otsuTreshold(Mat image) 
{
	int min = image.at<uchar>(0, 0);
	int max = image.at<uchar>(0, 0);

	for (int i = 0; i < image.rows; i++)
		for (int j=0;j<image.cols;j++)
	{
		int tmp=image.at<uchar>(i, j);
		if (tmp < min)
			min = tmp;
		if (tmp > max)
			max = tmp;
	}
	int histSize = max - min + 1;
	int* hist = new int[histSize];
	for (int i = 0; i < histSize; i++)
		hist[i] = 0;
	for (int i = 0; i < image.rows; i++)
		for (int j = 0; j < image.cols; j++) 
		{
			hist[image.at<uchar>(i, j)-min]++;
		}
	int m = 0; // m - ����� ����� ���� �����, ����������� �� ��������� �� ��������
	int n = 0; // n - ����� ����� ���� �����

	for (int i= 0; i <= max - min; i++)
	{
		m += i * hist[i];
		n += hist[i];
	}
	float maxSigma = -1; // ������������ �������� ������������ ���������
	int threshold = 0; // �����, ��������������� maxSigma

	int alpha1 = 0; // ����� ����� ���� ����� ��� ������ 1
	int beta1 = 0; // ����� ����� ���� ����� ��� ������ 1, ����������� �� ��������� �� ��������
	// ���������� alpha2 �� �����, �.�. ��� ����� m - alpha1
	// ���������� beta2 �� �����, �.�. ��� ����� n - alpha1

	// t ����������� �� ���� ��������� ��������� ������
	for (int i = 0; i < max - min; i++)
	{
		alpha1 += i * hist[i];
		beta1 += hist[i];
		// ������� ����������� ������ 1.
		float w1 = (float)beta1 / n;
		// �������� ����������, ��� w2 ���� �� �����, �.�. ��� ����� 1 - w1
		// a = a1 - a2, ��� a1, a2 - ������� �������������� ��� ������� 1 � 2
		float a = (float)alpha1 / beta1 - (float)(m - alpha1) / (n - beta1);
		// �������, ������� sigma
		float sigma = w1 * (1 - w1) * a * a;
		// ���� sigma ������ ������� ������������, �� ��������� maxSigma � �����
		if (sigma > maxSigma)
		{
			maxSigma = sigma;
			threshold = i;
		}
	}
	// �� �������, ��� ����� ������������ �� min, � �� �� ����
	threshold += min;
	// ���, ����� ��������, ���������� ��� ������ :)
	return threshold;
}

Mat makeintThresholdOtsu(Mat image, int threshold)
{
	for (int i=0;i<image.rows;i++)
		for (int j = 0; j < image.cols; j++) 
		{
			if (image.at<uchar>(i, j) < threshold)
				image.at<uchar>(i, j) = 255;
			else 
				image.at<uchar>(i, j) = 0;
		}
	return image;
}