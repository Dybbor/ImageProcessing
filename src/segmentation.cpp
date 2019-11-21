#pragma once
#include <iostream>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

Mat hist_intensity(Mat img,int porog) {
	int gist_intensity[256];
	for (int i = 0; i < 256; i++)
		gist_intensity[i] = 0;
	int Max_value = 0;;
	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {
			gist_intensity[img.at<uchar>(i, j)]++;
		}
	}
	for (int i = 0; i < 256; i++)
		if (gist_intensity[i] > Max_value)
			Max_value = gist_intensity[i];
	while (Max_value > 700) {
		for (int i = 0; i < 256; i++) {
			gist_intensity[i] /= 2;
		}
		Max_value /= 2;
	}
	Point p(0, Max_value);
	Mat result(Max_value, 1290, CV_8UC3);
	cout << Max_value << endl << result.rows << endl;
	for (int i = 0; i < 256; i++) {
		if (gist_intensity[i] > 1)
		{
			line(result, p, Point(p.x, result.rows - gist_intensity[i]), Scalar(255, 255, 255), 2);
			p.x += 5;
		}
	}
	line(result, Point(porog * 5, 5), Point(porog * 5 + 2, Max_value), Scalar(0, 0, 255), 5);
	return result;
}

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
	int m = 0; 
	int n = 0; 

	for (int i= 0; i <= max - min; i++)
	{
		m += i * hist[i];
		n += hist[i];
	}
	float maxSigma = -1; // Максимальное значение межклассовой дисперсии
	int threshold = 0; // Порог, соответствующий maxSigma

	int alpha1 = 0; // Сумма высот всех бинов для класса 1
	int beta1 = 0; // Сумма высот всех бинов для класса 1, домноженных на положение их середины

	for (int i = 0; i < max - min; i++)
	{
		alpha1 += i * hist[i];
		beta1 += hist[i];
		// Считаем вероятность класса 1.
		float w1 = (float)beta1 / n;
		float a = (float)alpha1 / beta1 - (float)(m - alpha1) / (n - beta1);
		float sigma = w1 * (1 - w1) * a * a;
		if (sigma > maxSigma)
		{
			maxSigma = sigma;
			threshold = i;
		}
	}
	threshold += min;
	return threshold;
}

Mat makeintThresholdOtsu(Mat image, int threshold)
{
	for (int i=0;i<image.rows;i++)
		for (int j = 0; j < image.cols; j++) 
		{
			if (image.at<uchar>(i, j) > threshold)
				image.at<uchar>(i, j) = 255;
			else 
				image.at<uchar>(i, j) = 0;
		}
	return image;
}