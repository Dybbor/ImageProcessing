#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <math.h>
#include <vector>
using namespace cv;
using namespace std;

auto calculateLowCoef() 
{
	vector<double> lowCoef;
	lowCoef.push_back((1 + sqrt(3)) / (4 * sqrt(2)));
	lowCoef.push_back((3 + sqrt(3)) / (4 * sqrt(2)));
	lowCoef.push_back((3 - sqrt(3)) / (4 * sqrt(2)));
	lowCoef.push_back((1 - sqrt(3)) / (4 * sqrt(2)));
	return lowCoef;
}
auto calculateHighCoef(vector<double> lowCoef) 
{
	vector<double> highCoef(lowCoef.size());
	for (int i = 0; i < lowCoef.size(); i++) 
	{
		highCoef[i]= pow(-1, i)*lowCoef[lowCoef.size() - i - 1];
	}
	return highCoef;
}
auto pairConvolution(vector<double>data, vector<double> lowCoef, vector <double> highCoef,int delta=0) 
{
	int n = lowCoef.size();
	int m = data.size();
	vector <double> res;
	for (int i = 0; i < m; i += 2) 
	{
		double sL = 0;
		double sH = 0;
		for (int j = 0; j < n; j++) 
		{
			sL += data[(i + j - delta) % m] * lowCoef[j];
			sH += data[(i + j - delta) % m] * highCoef[j];
		}
		res.push_back(sL);
		res.push_back(sH);
	}
	/*for (auto it = res.begin(); it != res.end(); it++)
		cout << *it << endl;*/
	return res;
}
auto backCoeff(vector <double> lowCoef, vector <double> highCoef)
{
	vector <double> res;
	vector <double> tmp;
	for (int i = 0; i < lowCoef.size(); i += 2)
	{
		int pos = i - 2;
		if (i - 2 < 0)
			pos = lowCoef[lowCoef.size() - 2];
		res.push_back(lowCoef[pos]);
		res.push_back(highCoef[pos]);
		tmp.push_back(lowCoef[pos+1]);
		tmp.push_back(highCoef[pos+1]);
	}
	for (int i = 0; i < highCoef.size(); i++)
		res.push_back(tmp[i]);
	return res;
}
auto getRow(Mat image, int n) 
{
	vector <double>res;
		for (int i = 0; i < image.cols; i++)
		{
			res.push_back(image.at<uchar>(n, i));
		}
		
		return res;
}

auto getCol(Mat image, int n)
{
	vector <double> res;
	for (int i = 0; i < image.cols; i++)
	{
		res.push_back(image.at<uchar>(i,n));
	}
	
	return res;
}
auto setRow(Mat image, int n, vector <double> row ) 
{
	for (int i = 0; i < image.cols; i++)
		image.at <uchar>(n, i) = (int)row[i];
	return image;
}
auto setCol(Mat image, int n, vector <double> col)
{
	for (int i = 0; i < image.rows; i++)
		image.at <uchar>(i, n) = (int)col[i];
	return image;
}
Mat Wavelet(Mat image, vector<double> lowCoef) 
{
	vector <double> highCoef = calculateHighCoef(lowCoef);
	Mat copy = image.clone();
	for (int i = 0; i < image.rows; i++)
	{
		setRow(copy, i, pairConvolution(getRow(image, i), lowCoef, highCoef));
	}
	for (int i = 0; i < image.cols; i++) 
	{
		setCol(copy, i, pairConvolution(getCol(image, i), lowCoef, highCoef));
	}
	namedWindow("Copy", WINDOW_NORMAL);
	imshow("Copy", copy);
	return copy;
	
}
Mat Transposition(Mat image) 
{
	Mat copy = image.clone();
	int k = 0;
	int l = 0;
	for (int i = 0; i < image.rows / 2; i++)
	{
		 l = 0;
		for (int j = 0; j < image.cols / 2; j++)
		{
			copy.at<uchar>(i, j) = image.at<uchar>(k, l);
			l += 2;
		}
		k += 2;
	}
	k = 1;
	for (int i = image.rows/2; i < image.rows; i++)
	{
		 l = 0;
		for (int j = 0; j < image.cols / 2; j++)
		{
			copy.at<uchar>(i, j) = image.at<uchar>(k, l);
			l += 2;
		}
		k += 2;
	}
	k = 0;
	for (int i = 02; i < image.rows/2; i++)
	{
		 l = 1;
		for (int j = image.cols/2; j < image.cols ; j++)
		{
			copy.at<uchar>(i, j) = image.at<uchar>(k, l);
			l += 2;
		}
		k += 2;
	}
	k = 1;
	for (int i = image.rows/2; i < image.rows; i++)
	{
		 l = 1;
		for (int j = image.cols/2; j < image.cols; j++)
		{
			copy.at<uchar>(i, j) = image.at<uchar>(k, l);
			l += 2;
		}
		k += 2;
	}
	namedWindow("Transposition", WINDOW_NORMAL);
	imshow("Transposition", copy);
	return copy;
}
// тут ффигня надо переделовать
//Mat backTransposition(Mat image)
//{
//	int k = 0, l = 0;
//	Mat copy = image.clone();
//	for (int i = 0; i < image.rows / 2; i++)
//	{
//		for (int j = 0; j < image.cols / 2; j++)
//		{
//			
//		}
//	}
//	namedWindow("Back Transposition", WINDOW_NORMAL);
//	imshow("Back Transposition", copy);
//	waitKey();
//	for (int i = image.rows / 2; i < image.rows; i++)
//	{
//		int k = 1, l = 0;
//		for (int j = 0; j < image.cols / 2; j++)
//		{
//			copy.at<uchar>(k, l) = image.at<uchar>(i, j);
//			k += 2;
//			l += 2;
//		}
//	}
//	for (int i = 02; i < image.rows / 2; i++)
//	{
//		int k = 0, l = 1;
//		for (int j = image.cols / 2; j < image.cols; j++)
//		{
//			copy.at<uchar>(k, l) = image.at<uchar>(i, j);
//			k += 2;
//			l += 2;
//		}
//	}
//	for (int i = image.rows / 2; i < image.rows; i++)
//	{
//		int k = 1, l = 1;
//		for (int j = image.cols / 2; j < image.cols; j++)
//		{
//			copy.at<uchar>(k, l) = image.at<uchar>(i, j);
//			k += 2;
//			l += 2;
//		}
//	}
//	namedWindow("Back Transposition", WINDOW_NORMAL);
//	imshow("Back Transposition", copy);
//	return copy;
//}
Mat hist_intensity(Mat img) {
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
		if (gist_intensity[i] > Max_value)Max_value = gist_intensity[i];
	while (Max_value > 700) {
		for (int i = 0; i < 256; i++) {
			gist_intensity[i] /= 2;
		}
		Max_value /= 2;
	}
	Point p(0, Max_value);
	Mat result(Max_value, 1290, CV_8UC1);
	cout << Max_value << endl << result.rows << endl;
	for (int i = 0; i < 256; i++) {
		if (gist_intensity[i] > 1)
		{
			line(result, p, Point(p.x, result.rows - gist_intensity[i]), Scalar(255, 255, 255), 2);
			p.x += 5;
		}
	}
	return result;
}

int main() 
{
	Mat image = imread("D:\\GitProject\\OU\\picture\\ship.jpg");
	if (!image.data) 
	{
		cout << "Error immage load" << endl;
		return -1;
	}
	if (image.channels() >1 )

		cvtColor(image,image, COLOR_BGR2GRAY);
	vector<double> lowCoef = calculateLowCoef();
	vector<double> highCoef= calculateHighCoef(lowCoef);
	namedWindow("original", WINDOW_NORMAL);
	imshow("original", image);
	Transposition(Wavelet(image, lowCoef));
	//imwrite("D:/GitProject/OU/picture/image.jpg",Transposition(Wavelet(image, lowCoef)));
	
	waitKey();

	return 0;
}