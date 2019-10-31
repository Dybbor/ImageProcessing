#include "filter.cpp"
//#include <conio.h>
const char* cmdOptions =
"{ i  image         | <none> | image to process        }"
"{ q ? help usage   | <none> | print help message      }";

int main(int argc,char** argv) {
	
	CommandLineParser parser(argc, argv, cmdOptions);
	//string path_to_image(parser.get<String>("C:\\Users\\aaron\\OneDrive\\Рабочий стол\\OU-build\\samples\\lobachevsky.jpg"));
	string path_to_image(parser.get<String>( "image")); 


	Mat image = imread(path_to_image);
	Mat cv_grey = image.clone();
	cvtColor(image, cv_grey, COLOR_BGR2GRAY);
	
	return 0;
}



