#include <opencv2/photo.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <vector>
#include <iostream>
#include <fstream>
using namespace cv;
using namespace std;


void readImagesAndTimes(vector<Mat> &images, vector<float> &times)
{
	static const float timesArray[] = { 0 };
	static const char* filenames[] = { 0 };
	int numImages = 4;
	int selectImage;

	cout << "Please Select Image HDR: ";
	cin >> selectImage;

	if (selectImage == 1) {
		static const float timesArray[] = { 1 / 30.0f,0.25f,2.5f,15.0f };
		static const char* filenames[] = { "Theory/HDR Image/1/1.jpg", "Theory/HDR Image/1/2.jpg", "Theory/HDR Image/1/3.jpg", "Theory/HDR Image/1/4.jpg" };
		times.assign(timesArray, timesArray + numImages);

		for (int i = 0; i < numImages; i++)
		{
			Mat im = imread(filenames[i]);
			images.push_back(im);
		}
	}

	/*
	if (selectImage == 2) {
		static const float timesArray[] = { 1 / 30.0f,1 / 30.0f,1 / 24.0f,1 / 24.0f };
		static const char* filenames[] = { "Theory/HDR Image/2/1.jpg", "Theory/HDR Image/2/2.jpg", "Theory/HDR Image/2/3.jpg", "Theory/HDR Image/2/4.jpg" };
		times.assign(timesArray, timesArray + numImages);

		for (int i = 0; i < numImages; i++)
		{
			Mat im = imread(filenames[i]);
			images.push_back(im);
		}
	}

	if (selectImage == 3) {
		static const float timesArray[] = { 0.5f,1.0f,2.0f,4.0f };
		static const char* filenames[] = { "Theory/HDR Image/3/1.jpg", "Theory/HDR Image/3/2.jpg", "Theory/HDR Image/3/3.jpg", "Theory/HDR Image/3/4.jpg" };
		times.assign(timesArray, timesArray + numImages);

		for (int i = 0; i < numImages; i++)
		{
			Mat im = imread(filenames[i]);
			images.push_back(im);
		}
	}
	*/
}

void methodSelected(Mat hdrDebevec) {
	cout << "Preparing Sample 1 ... ";
	Mat ldrDrago;
	Ptr<TonemapDrago> tonemapDrago = createTonemapDrago(1.0, 0.7);
	tonemapDrago->process(hdrDebevec, ldrDrago);
	ldrDrago = 3 * ldrDrago;
	imwrite("Theory/HDR Image/Result/result1.jpg", ldrDrago * 255);
	cout << "result1.jpg" << endl;

	cout << "Preparing Sample 2 ... ";
	Mat ldrDurand;
	Ptr<TonemapDurand> tonemapDurand = createTonemapDurand(1.5, 4, 1.0, 1, 1);
	tonemapDurand->process(hdrDebevec, ldrDurand);
	ldrDurand = 3 * ldrDurand;
	imwrite("Theory/HDR Image/Result/result2.jpg", ldrDurand * 255);
	cout << "result2.jpg" << endl;

	cout << "Preparing Sample 3 ... ";
	Mat ldrReinhard;
	Ptr<TonemapReinhard> tonemapReinhard = createTonemapReinhard(1.5, 0, 0, 0);
	tonemapReinhard->process(hdrDebevec, ldrReinhard);
	imwrite("Theory/HDR Image/Result/result3.jpg", ldrReinhard * 255);
	cout << "result3.jpg" << endl;

	cout << "Preparing Sample 4 ... ";
	Mat ldrMantiuk;
	Ptr<TonemapMantiuk> tonemapMantiuk = createTonemapMantiuk(2.2, 0.85, 1.2);
	tonemapMantiuk->process(hdrDebevec, ldrMantiuk);
	ldrMantiuk = 3 * ldrMantiuk;
	imwrite("Theory/HDR Image/Result/result4.jpg", ldrMantiuk * 255);
	cout << "result4.jpg" << endl;
}


int main(int, char**argv)
{
	// Read images
	cout << "Reading images ... " << endl;
	vector<Mat> images;
	vector<float> times;
	readImagesAndTimes(images, times);


	// Align
	cout << "Aligning images ... " << endl;
	Ptr<AlignMTB> alignMTB = createAlignMTB();
	alignMTB->process(images, images);

	// Obtain Camera Response Function (CRF)
	cout << "Calculating Camera Response Function (CRF) ... " << endl;
	Mat responseDebevec;
	Ptr<CalibrateDebevec> calibrateDebevec = createCalibrateDebevec();
	calibrateDebevec->process(images, responseDebevec, times);

	// Merge images
	cout << "Merging images into one HDR image ... ";
	Mat hdrDebevec;
	Ptr<MergeDebevec> mergeDebevec = createMergeDebevec();
	mergeDebevec->process(images, hdrDebevec, times, responseDebevec);

	// Save HDR image.
	imwrite("Theory/HDR Image/Result/hdrDebevec.hdr", hdrDebevec);
	cout << "saved hdrDebevec.hdr " << endl;

	methodSelected(hdrDebevec);



	return EXIT_SUCCESS;
}