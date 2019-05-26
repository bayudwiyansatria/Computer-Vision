#include <iostream>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;
Mat source, destination;

void dilation(Mat source);
void erosion(Mat source);
void distanceTransformation(Mat source);

int main() {
	String  imageData = "Practice/Neighborhood Operation/Input/people.jpg";
	source = imread(imageData, CV_LOAD_IMAGE_COLOR);
	imshow("Input", source);

	//dilation(source);
	erosion(source);
	//distanceTransformation(source);

	waitKey(0);
	return 1;
}

void dilation(Mat source) {
	int dilation_size = 3;
	Mat element = getStructuringElement(MORPH_CROSS, Size(2 * dilation_size + 1, 2 * dilation_size + 1), Point(dilation_size, dilation_size));

	dilate(source, destination, element);

	namedWindow("Output", WINDOW_AUTOSIZE);
	imshow("Output", destination);
}

void erosion(Mat source) {
	int erosion_size = 3;
	Mat element = getStructuringElement(MORPH_CROSS, Size(2 * erosion_size + 1, 2 * erosion_size + 1), Point(erosion_size, erosion_size));

	erode(source, destination, element);

	namedWindow("Output", WINDOW_AUTOSIZE);
	imshow("Output", destination);
}

void distanceTransformation(Mat source) {
	imshow("Source Image", source);

	Mat bw;
	cvtColor(source, bw, CV_BGR2GRAY);

	threshold(bw, bw, 40, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
	imshow("Binary Image", bw);

	Mat destination;
	distanceTransform(bw, destination, CV_DIST_L2, 3);

	normalize(destination, destination, 0, 1, NORM_MINMAX);
	imshow("Distance Transform", destination);
}

