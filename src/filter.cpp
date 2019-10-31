#pragma once
#include <iostream>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <string>
#include <iomanip>
#include <fstream>

using namespace std;
using namespace cv;

int Find_Max(int a, int b, int c) {
	if (a > b) {
		if (a > c)return a;
		else return c;
	}
	else if (b > c)return b;
	else return c;
}

int Find_Min(int a, int b, int c) {
	if (a < b) {
		if (a < c)return a;
		else return c;
	}
	else if (b < c)return b;
	else return c;
}

void ClearFile(ofstream& out,string path_file)
{
	/*out.open("D:\\GithubProjects\\Computer Graphics\\OU\\info.txt");*/
	out.open(path_file);
	if (!out.is_open())
		cout << "Error open file " << path_file << endl;
	out.close();
}

int MaxDiff(Mat diff)
{
	int max = 0;
	for (int i = 0; i < diff.rows; i++)
		for (int j = 0; j < diff.cols; j++)
			if (max < diff.at<uchar>(i, j))
				max = diff.at<uchar>(i, j);
	return max;
}

void WriteInfo(Mat image, ofstream& out, string path_file,string path_image=0, string name=0,int diff=0) //write matrix in file
{
	out << endl;
	out.open(path_file, ios::app);
	if (out.is_open())
	{
		out << "Image:" << path_image << endl;
		out << "Filter:" << name << endl;
		out << "Max:" << MaxDiff(image) << endl;
		out << "Mistake:" << diff << endl;
		if (image.rows <= 180 && image.cols < 180) {
			for (int i = 0; i < image.rows; i++) {
				for (int j = 0; j < image.cols; j++)
				{
					int num = (int)image.at<uchar>(i, j);
					if (num < 10)
						out << right << setw(6) << num;
					else if (num < 100)
						out << right << setw(5) << num;
					else
						out << right << setw(4) << num;
				}
				out << endl;
			}
		}
	}
	else
		cout << "Error open file in function WriteInfo " << path_file << endl;
	out << endl;
	out.close();
}

Mat Difference(Mat greyCV, Mat fil) // Matrix difference
{

	Mat result(greyCV.rows, greyCV.cols, CV_8UC1); //create monochro
	for (int i = 0; i < greyCV.rows; i++)
		for (int j = 0; j < greyCV.cols; j++)
			result.at<uchar>(i, j) = abs((int)greyCV.at<uchar>(i, j) - (int)fil.at<Vec3b>(i, j)[0]);
	return result;
}

class All_Fill {
protected:
	int cc;
	Mat img1, img2, img3;
public:
	All_Fill() { cc = 0; }
	void Count_CC(Mat orig) {
		int res = 0;
		for (int i = 0; i < orig.rows; i++) {
			for (int j = 0; j < orig.cols; j++) {
				res += abs((int)orig.at<uchar>(i, j) - (int)img1.at<Vec3b>(i, j)[0]);
			}
		}
		cc = abs(res / (orig.rows * orig.cols));
	};
	int Get_CC() { return cc; }

	void Make_Problems(Mat orig) {
		Mat result = img1.clone(); 
		for (int i = 0; i < orig.rows; i++) {
			for (int j = 0; j < orig.cols; j++) {
				if (abs((int)orig.at<uchar>(i, j) - (int)img1.at<Vec3b>(i, j)[0]) > cc) {
					result.at<Vec3b>(i, j)[2] = 255;
					result.at<Vec3b>(i, j)[1] = 0;
					result.at<Vec3b>(i, j)[0] = 0;
				}
			}
		}
		img2 = result.clone();
	};
	void Formula(Mat orig) 
	{
		Mat diff = Difference(orig, img1).clone();
		Mat ni(diff.rows, diff.cols, CV_8UC3);
		for (int i = 0; i < diff.rows; i++) {
			for (int j = 0; j < diff.cols; j++) {
				if ((int)diff.at<uchar>(i, j) < 8)
					ni.at<Vec3b>(i, j) = Vec3b(128, 0, 0);
				if ( (int)diff.at<uchar>(i,j)<20)
					ni.at<Vec3b>(i, j) = Vec3b(0, 255, 0);
				else if ((int)diff.at<uchar>(i, j) < 40)
					ni.at<Vec3b>(i, j) = Vec3b(0, 128, 0);
				else if ((int)diff.at<uchar>(i, j) < 60)
					ni.at<Vec3b>(i, j) = Vec3b(0, 255, 255);
				else if ((int)diff.at<uchar>(i, j) < 80)
					ni.at<Vec3b>(i, j) = Vec3b(0, 165, 255);
				else if ((int)diff.at<uchar>(i, j) < 100)
					ni.at<Vec3b>(i, j) = Vec3b(0, 0, 255);
				else if ((int)diff.at<uchar>(i, j) < 120)
					ni.at<Vec3b>(i, j) = Vec3b(0, 0, 128);
				else
					ni.at<Vec3b>(i, j) = Vec3b(0, 0, 0);

			}
		}

		/*imshow("New", ni);
		waitKey(0);*/
		img3 = ni.clone();
	}
	Mat Get_Mat1() { return img1; }
	Mat Get_Mat2() { return img2; }
	Mat Get_Mat3() { return img3; }
};

class Avg_Fill :public All_Fill {
public:
	Avg_Fill(Mat img) {
		Mat result = img.clone();
		for (int i = 0; i < result.rows; i++) {
			for (int j = 0; j < result.cols; j++) {
				int intense = (result.at<Vec3b>(i, j)[0] + result.at<Vec3b>(i, j)[1] + result.at<Vec3b>(i, j)[2]) / 3;
				result.at<Vec3b>(i, j)[0] = intense;
				result.at<Vec3b>(i, j)[1] = intense;
				result.at<Vec3b>(i, j)[2] = intense;
			}
		}
		img1 = result;
	}
};

class Light_Fill :public All_Fill {
public:
	Light_Fill(Mat img) {
		Mat result = img.clone();
		for (int i = 0; i < result.rows; i++) {
			for (int j = 0; j < result.cols; j++) {
				int intense = (Find_Max(result.at<Vec3b>(i, j)[0], result.at <Vec3b>(i, j)[1], result.at <Vec3b>(i, j)[2]) + Find_Min(result.at<Vec3b>(i, j)[0], result.at <Vec3b>(i, j)[1], result.at <Vec3b>(i, j)[2])) / 2;
				result.at<Vec3b>(i, j)[0] = intense;
				result.at<Vec3b>(i, j)[1] = intense;
				result.at<Vec3b>(i, j)[2] = intense;
			}
		}
		img1 = result.clone();
	}
};

class Lum_Fill :public All_Fill {
public:
	Lum_Fill(Mat img) {
		Mat result = img.clone();
		for (int i = 0; i < result.rows; i++) {
			for (int j = 0; j < result.cols; j++) {
				int intense = (result.at<Vec3b>(i, j)[0] * 0.21 + result.at<Vec3b>(i, j)[1] * 0.72 + result.at<Vec3b>(i, j)[2] * 0.07);
				result.at<Vec3b>(i, j)[0] = intense;
				result.at<Vec3b>(i, j)[1] = intense;
				result.at<Vec3b>(i, j)[2] = intense;
			}
		}
		img1 = result.clone();
	}
};

class Ph_Fill :public All_Fill {
public:
	Ph_Fill(Mat img) {
		Mat result = img.clone();
		for (int i = 0; i < result.rows; i++) {
			for (int j = 0; j < result.cols; j++) {
				int intense = (result.at<Vec3b>(i, j)[0] * 0.3 + result.at<Vec3b>(i, j)[1] * 0.59 + result.at<Vec3b>(i, j)[2] * 0.11);
				result.at<Vec3b>(i, j)[0] = intense;
				result.at<Vec3b>(i, j)[1] = intense;
				result.at<Vec3b>(i, j)[2] = intense;
			}
		}
		img1 = result.clone();
	}
};

class ITU_R_Fill :public All_Fill {
public:
	ITU_R_Fill(Mat img) {
		Mat result = img.clone();
		for (int i = 0; i < result.rows; i++) {
			for (int j = 0; j < result.cols; j++) {
				int intense = (result.at<Vec3b>(i, j)[0] * 0.2126 + result.at<Vec3b>(i, j)[1] * 0.7152 + result.at<Vec3b>(i, j)[2] * 0.0722);
				result.at<Vec3b>(i, j)[0] = intense;
				result.at<Vec3b>(i, j)[1] = intense;
				result.at<Vec3b>(i, j)[2] = intense;
			}
		}
		img1 = result.clone();
	}
};

class Max_Fill :public All_Fill {
public:
	Max_Fill(Mat img) {
		Mat result = img.clone();
		for (int i = 0; i < result.rows; i++) {
			for (int j = 0; j < result.cols; j++) {
				int intense = (Find_Max(result.at<Vec3b>(i, j)[0], result.at<Vec3b>(i, j)[1], result.at<Vec3b>(i, j)[2]));
				result.at<Vec3b>(i, j)[0] = intense;
				result.at<Vec3b>(i, j)[1] = intense;
				result.at<Vec3b>(i, j)[2] = intense;
			}
		}
		img1 = result.clone();
	}
};

class Min_Fill :public All_Fill {
public:
	Min_Fill(Mat img) {
		Mat result = img.clone();
		for (int i = 0; i < result.rows; i++) {
			for (int j = 0; j < result.cols; j++) {
				int intense = (Find_Min(result.at<Vec3b>(i, j)[0], result.at<Vec3b>(i, j)[1], result.at<Vec3b>(i, j)[2]));
				result.at<Vec3b>(i, j)[0] = intense;
				result.at<Vec3b>(i, j)[1] = intense;
				result.at<Vec3b>(i, j)[2] = intense;
			}
		}
		img1 = result.clone();
	}
};

class NN_Fill :public All_Fill {
public:
	NN_Fill(Mat img) {
		Mat result = img.clone();
		for (int i = 0; i < result.rows; i++) {
			for (int j = 0; j < result.cols; j++) {
				int intense = (result.at<Vec3b>(i, j)[0] * 0.2952 + result.at<Vec3b>(i, j)[1] * 0.5547 + result.at<Vec3b>(i, j)[2] * 0.148);
				result.at<Vec3b>(i, j)[0] = intense;
				result.at<Vec3b>(i, j)[1] = intense;
				result.at<Vec3b>(i, j)[2] = intense;
			}
		}
		img1 = result.clone();
	}
};
Mat CreateBlur(char f, Mat src,bool flag=0) 
{
	Mat dst(src.rows, src.cols,CV_8UC3);
	if (flag)
	{
		switch (f) 
		{
		default:
			break;
		}
	}
	else 
	{
		switch (f) 
		{
		case 'G':
			GaussianBlur(src, dst, Size(5, 5),0);
			break;
		case 'M':
			medianBlur(src, dst,5);
		}
	}
	return dst;
}