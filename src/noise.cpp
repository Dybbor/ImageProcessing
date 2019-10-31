#pragma once
#include <iostream>
#include <math.h>
#include <random>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include "histogram.cpp"

using namespace std;
using namespace cv;

const float PI = 3.14159265358979323846;


int random(int * mas)
{
	int value;
	while (value = rand() % 11) {
		if (mas[0] == 0 && mas[1] == 0 && mas[2] == 0 && mas[3] == 0 && mas[4 == 0])
			return 0;
		if (value == 5 && mas[value - 1] > 0)
		{
			mas[value]--;
			return value;
		}
		else if ((value == 0 || value == 10) && mas[0] > 0)
		{
			mas[0]--;
			return value;
		}
		else if ((value == 1 || value == 9) && mas[1] >0)
		{
			mas[1]--;
			return value;
		}
		else if ((value == 2 || value == 8) && mas[2] > 0)
		{
			mas[2]--;
			return value;
		}
		else if ((value == 3 || value == 7) && mas[1] >0)
		{
			mas[3]--;
			return value;
		}
		else if ((value == 4 || value == 6) && mas[1] > 0)
		{
			mas[4]--;
			return value;
		}
	}
}

double formula_gauss(double x,double nu, double sigma) 
{
	return (1 / (sigma*sqrt(2 * PI))*exp(-(pow((x - nu), 2)) / (2 * pow(sigma, 2))));
}


Mat normal_generator(int size_x,int size_y,double pr=0.5) 
{
	Mat result(size_x, size_y, CV_8UC3);
	int val = (size_x * size_y*pr)/10;
	int mas[10] = { 2* val,2* val,2* val,2* val,val };
	for (int i = 0; i < result.rows; i++) {
		for (int j = 0; j < result.cols; j++) {
			result.at<Vec3b>(i, j) = 0;
		}
	}
	int f = 0, a = 0, b = 0, c = 0, d = 0, e = 0;
	for (int i = 0; i < size_x * size_y * pr; i++) {
		int x = rand() % size_x;
		int y = rand() % size_y;
		int ind = random(mas);
		cout << ind << endl;
		int add = 0;
		double value = formula_gauss((double)ind,5,1.5)*1000;
		if (value < 1.1)
		{
			f++;
			add = 5;
		}
		if (value <= 7.6) { //1,10
			a++;
			add = 10;
		}
		else if (value < 36.0) {//2,9
			add = 20;
			b++;
		}
		else if (value < 110) //3,8
		{
			add = 30;
			e++;
		}
		else if (value < 213)//4,7
		{
			add = 40;
			c++;
		}
		else {
			add = 50;
			d++;
		}
		result.at<Vec3b>(x, y)[0] = result.at < Vec3b>(x, y)[1] = result.at < Vec3b>(x, y)[2] = add;
	}
	cout << f << " " << a << " " << b << " " << e << " " << c << " " << d << endl;
	cout << val << endl;
	for (int i = 0; i < 5; i++)
		cout << mas[i] << endl;
	return result;
}
int Clamp(int x) {
	int n = x > 0 ? x : 0;
	n = x < 256 ? x : 255;
	return n;
}

int Clamp(int x, int y, int z) {
	int n = x < y ? y : x;
	n = n > z ? z : n;
	return n;
}
Mat createNoise(Mat image, Mat noise) 
{
	return image + noise;
}



Mat Generate_Mask_Gauss(int size_x, int size_y,double pr=0.5) {
	random_device rd;
	mt19937 gen(rd());
	normal_distribution<> d(20, 1);
	Mat result(size_x, size_y, CV_8UC3);
	srand(time(NULL));
	for (int i = 0; i < result.rows; i++) {
		for (int j = 0; j < result.cols; j++) {
			result.at<Vec3b>(i, j) = 0;
		}
	}
	for (int i = 0; i < size_x * size_y * pr; i++) {
		int x = rand() % size_x;
		int y = rand() % size_y;
		result.at<Vec3b>(x, y)[0] = result.at < Vec3b>(x, y)[1] = result.at < Vec3b>(x, y)[2] = abs(d(gen));
	}
	return result;
}

Mat Generate_Mask_Gamma(int size_x, int size_y,double pr=0.5) {
	random_device rd;
	mt19937 gen(rd());
	gamma_distribution<> d(10,1);
	Mat result(size_x, size_y, CV_8UC3);
	srand(time(NULL));
	for (int i = 0; i < result.rows; i++) {
		for (int j = 0; j < result.cols; j++) {
			result.at<Vec3b>(i, j) = 0;
		}
	}
	for (int i = 0; i < size_x * size_y * pr; i++) {
		int x = rand() % size_x;
		int y = rand() % size_y;
		result.at<Vec3b>(x, y)[0] = result.at < Vec3b>(x, y)[1] = result.at < Vec3b>(x, y)[2] = abs(d(gen));
	}
	return result;
}

float** Gauss_Kernel(int radius, float sigma) {
	int size = 2 * radius + 1;
	float** kernel = new float* [size];
	for (int i = 0; i < size; i++) {
		kernel[i] = new float[size];
	}
	float norm = 0;
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
		{
			kernel[i][j] = (float)(exp(-(i * i + j * j) / (sigma * sigma)));
			norm += kernel[i][j];
		}
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			kernel[i][j] /= norm;
	return kernel;
}

Vec3b Get_Color(float** kernel, Mat img, int size, int x, int y) {
	Vec3b color = 0;
	int x1 = 0, y1 = 0;
	for (int i = x; i < size + x; i++) {
		y1 = 0;
		for (int j = y; j < size + y; j++) {
			color[0] += img.at<Vec3b>(i, j)[0] * kernel[x1][y1];
			color[1] += img.at<Vec3b>(i, j)[1] * kernel[x1][y1];
			color[2] += img.at<Vec3b>(i, j)[2] * kernel[x1][y1];
			y1++;
		}
		x1++;
	}
	return color;
}

Mat Gauss_Filter(Mat img) {
	int n = 3;
	Mat result = img.clone();
	float** kernel = Gauss_Kernel(n, 2);
	for (int i = 2 * n + 1; i < img.rows - (2 * n + 1); i++) {
		for (int j = 2 * n + 1; j < img.cols - (2 * n + 1); j++) {
			result.at<Vec3b>(i, j) = Get_Color(kernel, img, 2 * n + 1, i, j);
		}
	}
	return result;
}

int median(vector <int> v) 
{
	
	sort(v.begin(), v.end(), greater<int>());
	return v[4];

}
Mat median_filter(Mat img)
{
	const int n = 3;
	Mat copy(img.rows + 2, img.cols + 2, CV_8UC3);
	Mat result(img.rows, img.cols, CV_8UC3);
	cout << img.cols << endl;
	for (int i = 0; i < copy.rows; i++)
		for (int j = 0; j < copy.cols; j++)
		{
			copy.at<Vec3b>(i, j)[0] = 0;
			copy.at<Vec3b>(i, j)[1] = 0;
			copy.at<Vec3b>(i, j)[2] = 0;
		}
	for (int i = 0; i < 3; i++) {
		copy.at<Vec3b>(0, 0)[i] = img.at <Vec3b>(0, 0)[i];
		copy.at<Vec3b>(copy.rows - 1, 0)[i] = img.at <Vec3b>(img.rows - 1, 0)[i];
		copy.at<Vec3b>(0, copy.cols - 1)[i] = img.at <Vec3b>(0, img.cols - 1)[i];
		copy.at<Vec3b>(copy.rows - 1, copy.cols - 1)[i] = img.at <Vec3b>(img.rows - 1, img.cols - 1)[i];
	}
	for (int i = 0; i < copy.rows; i++) {
		for (int j = 0; j < copy.cols; j++)
		{
			if (j > 0 && i > 0 && j < copy.cols - 1 && i < copy.rows - 1)
			{
				copy.at<Vec3b>(i, j)[0] = img.at <Vec3b>(i - 1, j - 1)[0];
				copy.at<Vec3b>(i, j)[1] = img.at <Vec3b>(i - 1, j - 1)[1];
				copy.at<Vec3b>(i, j)[2] = img.at <Vec3b>(i - 1, j - 1)[2];
			}
			else if (i == 0 && j > 0 && j < copy.cols - 1) {
				copy.at<Vec3b>(i, j)[0] = (int)img.at <Vec3b>(i, j - 1)[0];
				copy.at<Vec3b>(i, j)[1] =(int) img.at <Vec3b>(i, j - 1)[1];
				copy.at<Vec3b>(i, j)[2] = (int)img.at <Vec3b>(i, j - 1)[2];
			}
			else if (i == copy.rows - 1 && j > 0 && j < copy.cols)
			{
				copy.at<Vec3b>(i, j)[0] = (int)copy.at <Vec3b>(i - 1, j)[0];
				copy.at<Vec3b>(i, j)[1] = (int)copy.at <Vec3b>(i - 1, j)[1];
				copy.at<Vec3b>(i, j)[2] = (int)copy.at <Vec3b>(i - 1, j)[2];
			}
			else if (j == copy.cols - 1 && i > 0 && i < copy.rows - 1)
			{
				copy.at<Vec3b>(i, j)[0] = copy.at<Vec3b>(i, j - 1)[0];
				copy.at<Vec3b>(i, j)[1] = copy.at<Vec3b>(i, j - 1)[1];
				copy.at<Vec3b>(i, j)[2] = copy.at<Vec3b>(i, j - 1)[2];
			}
			else if (j == 0 && i > 0 && i < copy.rows - 1)
			{
				copy.at<Vec3b>(i, j)[0] = img.at<Vec3b>(i - 1, j)[0];
				copy.at<Vec3b>(i, j)[1] = img.at<Vec3b>(i - 1, j)[1];
				copy.at<Vec3b>(i, j)[2] = img.at<Vec3b>(i - 1, j)[2];
			}
			/*namedWindow("cpy1", WINDOW_NORMAL);
			imshow("cpy1", copy);
			waitKey();*/
		}
	}
	destroyWindow("cpy");
	for (int i = 1; i < copy.rows - 1; i++){
		for (int j = 1; j < copy.cols - 1; j++)
		{
			vector <int > b, g, r;
			for (int x = -1; x < 2; x++)
				for (int y = -1; y < 2; y++) {
					b.push_back((int)copy.at <Vec3b>(i + x, j + y)[0]);
					g.push_back((int)copy.at <Vec3b>(i + x, j + y)[1]);
					r.push_back((int)copy.at <Vec3b>(i + x, j + y)[2]);
				}
			result.at<Vec3b>(i - 1, j - 1)[0] = median(b);
			result.at<Vec3b>(i - 1, j - 1)[1] = median(g);
			result.at<Vec3b>(i - 1, j - 1)[2] = median(r);
			b.clear();
			g.clear();
			r.clear(); 
			r.clear(); 
		}
}
	namedWindow("cpy1", WINDOW_NORMAL);
	imshow("cpy1", result);
	waitKey();
	
	return result;
}



