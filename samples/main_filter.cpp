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
	}
	int key = 0;
	int curr = 0;
	while (key!=27) {//esc
		key = waitKey();
		switch (key)
		{
		case 52: {//numpad4
			if (curr > 0)curr--;
			system("cls");
			cout << labels[curr] << endl;
			cout << filters[curr].Get_CC();
			break;
		}
		case 54: {//numpad6
			if (curr < 7)curr++;
			system("cls");
			cout << labels[curr] << endl;
			cout << filters[curr].Get_CC() << endl;
			break;
		}
		}
		imshow("CV_GRAY", cv_grey);
		imshow("OUR FILTER", filters[curr].Get_Mat1());
		imshow("PROBLEMS", filters[curr].Get_Mat2());
		waitKey(1);
	}
	return 0;
}



