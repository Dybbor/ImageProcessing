#include "filter.cpp"

const char* cmdOptions = {
"{ i image      | <none> | image to process        }"
"{ f  file  | 0 | file to save info	   }"
"{ q ? |help usage   | <none> | print help message      }" 
};


int main(int argc, char** argv) {

	CommandLineParser parser(argc, argv, cmdOptions);
	ofstream out;
	string path_to_image = "C:\\Users\\aaron\\OneDrive\\Рабочий стол\\OU\\picture\\lob.jpg";
	string path_to_file = "File.txt";
	Mat image = imread(path_to_image);
	if (image.empty()) {
		cout << "Error load image" << endl;
		return -1;
	}
	Mat cv_grey = image.clone();
	cvtColor(image, cv_grey, COLOR_BGR2GRAY);
	string labels[8] = { "Average","Lightness","Luminosity","Photoshop","ITU_R","MAX","MIN","NONAME" };
	All_Fill filters[8];
	Avg_Fill avg(image);
	Light_Fill lig(image);
	Lum_Fill lum(image);
	Ph_Fill ph(image);
	ITU_R_Fill it(image);
	Max_Fill max(image);
	Min_Fill min(image);
	NN_Fill nn(image);
	filters[0] = avg;
	filters[1] = lig;
	filters[2] = lum;
	filters[3] = ph;
	filters[4] = it;
	filters[5] = max;
	filters[6] = min;
	filters[7] = nn;

	for (int i = 0; i < 8; i++) {
		filters[i].Count_CC(cv_grey);
		filters[i].Make_Problems(cv_grey);
		filters[i].Formula(cv_grey);
	}
	int key = 0;
	int curr = 0;
	while (key != 27) {//esc
		key = waitKey();
		cout << key << endl;
		switch (key)
		{
		case 52: {//numpad4
			if (curr > 0)curr--;
			system("cls");
			cout << labels[curr] << endl;
			cout << filters[curr].Get_CC() << endl;
			break;
		}
		case 54: {//numpad6
			if (curr < 7)curr++;
			system("cls");
			cout << labels[curr] << endl;
			cout << filters[curr].Get_CC() << endl;
			break;
		}
		case 56: 
		{	
			destroyAllWindows();
			for (int i = 0; i < 8; i++) {
				namedWindow(labels[i], WINDOW_NORMAL);
				imshow(labels[i], filters[i].Get_Mat3());
			}
			waitKey(0);
			destroyAllWindows();
			break;
		}
		case 48: {//numpad0
			if (path_to_file != "0")
			{
				ClearFile(out, path_to_file);
				for (int i = 0; i < 8; i++)
					WriteInfo(Difference(cv_grey, filters[i].Get_Mat1()), out, path_to_file, path_to_image, labels[i], filters[i].Get_CC());
				if (image.rows < 180 && image.cols < 180) cout << "info write in the file" << endl;
				else cout << "Image too large,can't write matrix" << endl;
			}
			else cout << "Error path file" << endl;
			break;
		}
		}
		namedWindow("CV_GRAY", WINDOW_NORMAL);
		namedWindow("OUR FILTER", WINDOW_NORMAL);
		namedWindow("PROBLEMS", WINDOW_NORMAL);
		imshow("CV_GRAY",cv_grey);
		imshow("OUR FILTER", filters[curr].Get_Mat1());
		imshow("PROBLEMS", filters[curr].Get_Mat2());
		waitKey(1);
	}
		return 0;
}




//Mat Difference(Mat greyCV, Mat fil)
//{
//
//	Mat result(greyCV.rows, greyCV.cols, CV_8UC1);
//	for (int i = 0; i < greyCV.rows; i++)
//		for (int j = 0; j < greyCV.cols; j++)
//			result.at<uchar>(i, j) = abs((int)greyCV.at<uchar>(i, j) - (int)fil.at<Vec3b>(i, j)[0]);
//	return result;
//}
//
//void fillFile(Mat dif, ofstream& out, string name) 
//{
//	
//	out.open("D:\\GithubProjects\\Computer Graphics\\OU\\info1.txt",ios::app);
//	if (out.is_open())
//	{
//		out << "Difference "<<name   << endl;
//		for (int i = 0; i < dif.rows; i++) {
//			for (int j = 0; j < dif.cols; j++)
//			{
//				int num =(int) dif.at<uchar>(i, j);
//				if (num < 10)
//					out << right << setw(6)<< num;
//				else if (num < 100)
//					out << right << setw(5)  << num;
//				else
//					out << right << setw(4)  << num;
//			}
//			out << endl;
//		}
//	}
//	else
//		cout << "Error open file" << endl;
//	out.close();
//}
//
//
//int main(int argc,char** argv) {
//	ofstream out;
//	out.open("D:\\GithubProjects\\Computer Graphics\\OU\\info1.txt");
//	out.close();
//	out.open("D:\\GithubProjects\\Computer Graphics\\OU\\info.txt");
//	out.close();
//	CommandLineParser parser(argc, argv, cmdOptions);
//	string path_to_image(parser.get<String>( "image"));
//	Mat image = imread(path_to_image);
//	if (image.empty()) {
//		cout << "Failed load image" << endl;
//		return -1;
//	}
//	Filter fil(image);
//	Mat new_image = image.clone();
// 	cvtColor(image, new_image, COLOR_BGR2GRAY);
//	Mat avg=fil.AverageFilter();
//	Mat lightness = fil.LightnessFilter();
////	fillFile(avg, out, "avg");
//	Mat dif = Difference(new_image, avg);
//	fillFile(dif, out," avg");
//	//fillFile(Difference(new_image, lightness), out, " lightness");
//	fillFile1(new_image, avg, out, "first avg");
//	//fillFile(new_image, lightness, out, "first light");*/
//	
//	return 0;
//}
//




//лагающий кусок дерьма
//int curr = 1;
//string nf[9] = { "Opencv grey", "Average", "Lightness", "Luminosity", "Photoshop","ITU_R", "Max", "Min", "Noname" };//namef filter
//Mat picture[9];
//cvtColor(image, picture[0], COLOR_BGR2GRAY);
//picture[1] = fil.AverageFilter();
//picture[2] = fil.LightnessFilter();
//picture[3] = fil.LuminosityFilter();
//picture[4] = fil.PhotoshopFilter();
//picture[5] = fil.ITU_R();
//picture[6] = fil.Max();
//picture[7] = fil.Min();
//picture[8] = fil.NoName();
//int key = 10;
//namedWindow(nf[0], WINDOW_AUTOSIZE);
//imshow(nf[0], picture[0]);
//waitKey(1000000000000);
//bool first = true;
//while (key != 27)
//{
//	if (first) {
//		namedWindow(nf[1], WINDOW_AUTOSIZE);
//		imshow(nf[1], picture[1]);
//		first = false;
//		waitKey(100000);
//	}
//	key = _getch();
//	key = _getch();
//	if (key == 75 && curr > 1)
//	{
//		destroyWindow(nf[curr]);
//		curr--;
//		cout << curr << endl;
//	}
//	else if (key == 77 && curr < 8)
//	{
//		destroyWindow(nf[curr]);
//		curr++;
//		cout << curr << endl;
//	}
//	namedWindow(nf[curr], WINDOW_AUTOSIZE);
//	imshow(nf[curr], picture[curr]);
//	waitKey(10000000);
//}
