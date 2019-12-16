#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <math.h>
#include <vector>
#include <conio.h>
using namespace cv;
using namespace std;

int HowMuchPorog(double** data, int rows, int cols,double porog) 
{
	int count = 0;
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			if (abs(data[i][j]) < porog)
				count++;
	return count;
}
void DeleteLessPorog(double**data, int rows, int cols, double porog) 
{
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			if (abs(data[i][j]) < porog)
				data[i][j] = 0;
}

int Diff(Mat image1, Mat image2) 
{
	int count = 0;
	for (int i=0;i<image1.rows;i++)
		for (int j = 0; j < image1.cols;j++) 
			if (image1.at<uchar>(i, j) != image2.at<uchar>(i, j))
				count++;
	return count;
}


void showDiff(Mat orig, Mat diff)
{
	namedWindow("Diff", WINDOW_NORMAL);
	for (int i = 0; i < orig.rows; i++)
		for (int j = 0; j < orig.cols; j++)
		{
			if (abs(orig.at<uchar>(i, j) - diff.at<uchar>(i, j)) > 10)
				diff.at<uchar>(i, j) = 0;
		}
	imshow("Diff", orig);
	cout << " awdlajwdk " << Diff(orig, diff) << endl;;
}

void PrintData(double** data, int rows, int cols) 
{
	cout << endl;
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			cout <<setw(5)<< data[i][j] << " ";
		}
		cout << endl;
	}
}
void WaveletCols(double** data, int rows, int cols)
{
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j+=2) 
		{
			double tmp1, tmp2;
			tmp1 = (data[i][j] + data[i][j + 1])/2;
			tmp2 = (data[i][j] - data[i][j + 1])/2;
			data[i][j] = tmp1;
			data[i][j + 1]=tmp2;
		}
	}
}
void BackWaveletCols(double**data,int rows, int cols) 
{
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j+=2) 
		{
			double tmp1, tmp2;
			tmp1 = data[i][j] + data[i][j + 1];
			tmp2= data[i][j] - data[i][j + 1];
			data[i][j] = tmp1;
			data[i][j + 1] = tmp2;
		}
	}
}
void WaveletRows(double** data, int rows, int cols)
{
	for (int j = 0; j < cols; j++)
	{
		for (int i = 0; i < rows; i += 2)
		{
			double tmp1, tmp2;
			tmp1 = (data[i][j] + data[i+1][j]) / 2;
			tmp2 = (data[i][j] - data[i+1][j]) / 2;
			data[i][j] = tmp1;
			data[i+1][j] = tmp2;
		}
	}
}
void BackWaveletRows(double**data, int rows, int cols)
{
	for (int j = 0; j < cols; j++)
	{
		for (int i = 0; i < rows; i += 2)
		{
			double tmp1, tmp2;
			tmp1 = data[i][j] + data[i+1][j];
			tmp2 = data[i][j] - data[i+1][j];
			data[i][j] = tmp1;
			data[i+1][j] = tmp2;
		}
	}
}
void SeparateCols(double** data, int rows, int cols, int coef)
{
	double** tmp_data = new double*[rows];
	for (int i = 0; i < rows; i++)
		tmp_data[i] = new double[cols];
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			tmp_data[i][j] = data[i][j];
	int move;
	for (int i = 0; i < rows/coef; i++) {
		move = 0;
		for (int j = 0; j < cols/coef; j += 2) {
			data[i][move] = tmp_data[i][j];
			data[i][cols / 2/ coef + move] = tmp_data[i][j + 1];
			move++;
		}
	}
	for (int i = 0; i < rows; i++)
		delete[] tmp_data[i];
	delete[] tmp_data;
}
void BackSeparateCols(double** data, int rows, int cols, int coef)
{
	double** tmp_data = new double*[rows];
	for (int i = 0; i < rows; i++)
		tmp_data[i] = new double[cols];
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			tmp_data[i][j] = data[i][j];
	int move;
	for (int i = 0; i < rows / coef; i++) {
		move = 0;
		for (int j = 0; j < cols / coef; j += 2) {
			data[i][j] = tmp_data[i][move];
			data[i][j+1] = tmp_data[i][cols / 2 / coef + move];
			move++;
		}
	}
	for (int i = 0; i < rows; i++)
		delete[] tmp_data[i];
	delete[] tmp_data;
}

void SeparateRows(double** data, int rows, int cols,int coef)
{
	double** tmp_data = new double*[rows];
	for (int i = 0; i < rows; i++)
		tmp_data[i] = new double[cols];
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			tmp_data[i][j] = data[i][j];
	int move;
	for (int j = 0; j < cols/coef; j++) {
		move = 0;
		for (int i = 0; i < rows/coef;  i+= 2) {
			data[move][j] = tmp_data[i][j];
			data[rows/2/coef+move][j] = tmp_data[i+1][j];
			move++;
		}
	}
	for (int i = 0; i < rows; i++)
		delete[] tmp_data[i];
	delete[] tmp_data;
}

void BackSeparateRows(double** data, int rows, int cols, int coef)
{
	double** tmp_data = new double*[rows];
	for (int i = 0; i < rows; i++)
		tmp_data[i] = new double[cols];
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			tmp_data[i][j] = data[i][j];
	int move;
	for (int j = 0; j < cols / coef; j++) {
		move = 0;
		for (int i = 0; i < rows / coef; i += 2) {
			data[i][j] = tmp_data[move][j];
			data[i+1][j] = tmp_data[rows / 2 / coef + move][j];
			move++;
		}
	}
	for (int i = 0; i < rows; i++)
		delete[] tmp_data[i];
	delete[] tmp_data;
}

Mat DataToMat(double** data, int rows, int cols) 
{
	Mat image(rows, cols, CV_8UC1);
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			image.at<uchar>(i, j) = (uchar)abs(data[i][j]);
	return image;
}
int main() 
{
	Mat image = imread("D:\\GitProject\\ImageProcessing\\picture\\ship.jpg");
	Mat wl1, wl2, wl3;
	Mat dl1, dl2, dl3;
	double porog = 5;
	cout << "porog " << porog << endl;
	if (!image.data) 
	{
		cout << "Error immage load" << endl;
		return -1;
	}
	//namedWindow("image", WINDOW_NORMAL);
	//namedWindow("wl1",WINDOW_NORMAL); //wavelet level 1
	//namedWindow("wl2",WINDOW_NORMAL); //wavelet level 2
	//namedWindow("wl3",WINDOW_NORMAL); //wavelet level 3
	//namedWindow("dl1", WINDOW_NORMAL); //decoder level 1
	//namedWindow("dl2", WINDOW_NORMAL); //decoder level 2
	//namedWindow("dl3", WINDOW_NORMAL); //decoder level 3
	if (image.channels() >1 )
		cvtColor(image,image, COLOR_BGR2GRAY);
	double**data = new double*[image.rows];
	for (int i = 0; i < image.rows; i++)
	{
		data[i] = new double[image.cols];
	}
	for (int i = 0; i < image.rows; i++)
		for (int j = 0; j < image.cols; j++)
			data[i][j] = (int)image.at<uchar>(i, j);
	//level 1
	WaveletRows(data, image.rows, image.cols);
	SeparateRows(data, image.rows, image.cols,1);
	WaveletCols(data, image.rows, image.cols);
	SeparateCols(data, image.rows, image.cols,1);
	wl1 = DataToMat(data, image.rows, image.cols);
	//level 2
	WaveletRows(data, image.rows/2, image.cols/2);
	SeparateRows(data, image.rows, image.cols,2);
	WaveletCols(data, image.rows/2, image.cols/2);
	SeparateCols(data, image.rows, image.cols,2);
	wl2 = DataToMat(data, image.rows, image.cols);
	//level 3
	WaveletRows(data, image.rows / 4, image.cols / 4);
	SeparateRows(data, image.rows, image.cols, 4);
	WaveletCols(data, image.rows / 4, image.cols / 4);
	SeparateCols(data, image.rows, image.cols, 4);
	wl3 = DataToMat(data, image.rows, image.cols);
	imwrite("D:\\GitProject\\ImageProcessing\\picture\\save_without_porog.jpg", wl3);
	cout << "count porog " << HowMuchPorog(data, image.rows, image.cols, porog) << endl;
	DeleteLessPorog(data, image.rows, image.cols, porog);
	//decoder level 3
	BackSeparateCols(data, image.rows, image.cols, 4);
	BackWaveletCols(data, image.rows / 4, image.cols / 4);
	BackSeparateRows(data, image.rows, image.cols, 4);
	BackWaveletRows(data, image.rows / 4, image.cols / 4);
	dl3 = DataToMat(data, image.rows, image.cols);
	//decoder level 2
	BackSeparateCols(data, image.rows, image.cols,2);
	BackWaveletCols(data, image.rows/2, image.cols/2);
	BackSeparateRows(data, image.rows, image.cols,2);
	BackWaveletRows(data, image.rows/2, image.cols/2);
	dl2= DataToMat(data, image.rows, image.cols);
	//decoder level 1
	BackSeparateCols(data, image.rows, image.cols, 1);
	BackWaveletCols(data, image.rows , image.cols);
	BackSeparateRows(data, image.rows, image.cols,1);
	BackWaveletRows(data, image.rows , image.cols);
	dl1 = DataToMat(data, image.rows, image.cols);
	//showDiff(image, dl1);
	//threshold(wl3, wl3, 15, 255, THRESH_BINARY);
	imwrite("D:\\GitProject\\ImageProcessing\\picture\\save_with_porog.jpg", wl3);
	imwrite("D:\\GitProject\\ImageProcessing\\picture\\grey.jpg", image);
	imshow("image", image);
	imshow("wl1", wl1);
	imshow("wl2", wl2);
	imshow("wl3", wl3);
	imshow("dl3", dl3);
	imshow("dl2", dl2);
	imshow("dl1", dl1);
	cout << "loss: " <<(double) ( (double)(Diff(image, dl1)*100)) / (image.rows*image.cols) << endl;
	waitKey();
	for (int i = 0; i < image.rows; i++)
		delete[] data[i];
	delete[] data;
	return 0;
}

