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
	int m = 0; // m - сумма высот всех бинов, домноженных на положение их середины
	int n = 0; // n - сумма высот всех бинов

	for (int i= 0; i <= max - min; i++)
	{
		m += i * hist[i];
		n += hist[i];
	}
	float maxSigma = -1; // Максимальное значение межклассовой дисперсии
	int threshold = 0; // Порог, соответствующий maxSigma

	int alpha1 = 0; // Сумма высот всех бинов для класса 1
	int beta1 = 0; // Сумма высот всех бинов для класса 1, домноженных на положение их середины
	// Переменная alpha2 не нужна, т.к. она равна m - alpha1
	// Переменная beta2 не нужна, т.к. она равна n - alpha1

	// t пробегается по всем возможным значениям порога
	for (int i = 0; i < max - min; i++)
	{
		alpha1 += i * hist[i];
		beta1 += hist[i];
		// Считаем вероятность класса 1.
		float w1 = (float)beta1 / n;
		// Нетрудно догадаться, что w2 тоже не нужна, т.к. она равна 1 - w1
		// a = a1 - a2, где a1, a2 - средние арифметические для классов 1 и 2
		float a = (float)alpha1 / beta1 - (float)(m - alpha1) / (n - beta1);
		// Наконец, считаем sigma
		float sigma = w1 * (1 - w1) * a * a;
		// Если sigma больше текущей максимальной, то обновляем maxSigma и порог
		if (sigma > maxSigma)
		{
			maxSigma = sigma;
			threshold = i;
		}
	}
	// Не забудем, что порог отсчитывался от min, а не от нуля
	threshold += min;
	// Все, порог посчитан, возвращаем его наверх :)
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