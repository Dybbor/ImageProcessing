#include "segmentation.cpp"

//#include <conio.h>
const char* cmdOptions =
"{ i  image         | <none> | image to process        }"
"{ q ? help usage   | <none> | print help message      }";

int main(int argc, char** argv) {

	CommandLineParser parser(argc, argv, cmdOptions);
	//string path_to_image(parser.get<String>("C:\\Users\\aaron\\OneDrive\\Рабочий стол\\OU-build\\samples\\lobachevsky.jpg"));
	Mat image = imread("../../ImageProcessing/picture/cat.jpg");
	if (image.empty())
	{
		cout << "Error image load" << endl;
		return -1;
	}
	namedWindow("Image", WINDOW_AUTOSIZE);
	imshow("Image", image);
	if (image.channels() != 1)
	{
		cvtColor(image, image, COLOR_BGR2GRAY);
		cout << "to grey " << image.channels()<<endl;
	}
	cout << "Porog " << otsuTreshold(image) << endl;
	namedWindow("Image", WINDOW_NORMAL);
	imshow("Image", image);
	namedWindow("ThrashHold", WINDOW_NORMAL);
	imshow("ThrashHold", makeintThresholdOtsu(image,otsuTreshold(image)));
	waitKey(0);
	return 0;
}



