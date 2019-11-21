#include "segmentation.cpp"
//#include <conio.h>
const char* cmdOptions =
"{ i  image         | <none> | image to process        }"
"{ q ? help usage   | <none> | print help message      }";

int main(int argc, char** argv) {

	CommandLineParser parser(argc, argv, cmdOptions);
	//string path_to_image(parser.get<String>("C:\\Users\\aaron\\OneDrive\\Рабочий стол\\OU-build\\samples\\lobachevsky.jpg"));
	Mat image = imread("../../ImageProcessing/picture/audi.jpg");
	Mat orig;
	int porog;
	if (image.empty())
	{
		cout << "Error image load" << endl;
		return -1;
	}
	if (image.channels() != 1)
	{
		cvtColor(image, image, COLOR_BGR2GRAY);
		orig = image.clone();
		cout << "to grey " << image.channels()<<endl;
	}
	cout << "Porog " << otsuTreshold(image) << endl;
	porog = otsuTreshold(image);
	image = makeintThresholdOtsu(image, porog);
	namedWindow("Image", WINDOW_NORMAL);
	namedWindow("ThrashHold", WINDOW_NORMAL);
	namedWindow("hist", WINDOW_NORMAL);
	imshow("Image", orig);
	//GaussianBlur(image, image, Size(3, 3), 0);
	imshow("ThrashHold",image);
	imshow("hist", hist_intensity(orig,porog));
	waitKey(0);
	return 0;
}



