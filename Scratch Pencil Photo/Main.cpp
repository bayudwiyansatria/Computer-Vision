#include <opencv/cv.h>
#include <opencv/highgui.h>


int main(int argc, char** argv)
{
	int col_1, row_1;
	uchar b_1, g_1, r_1, b_2, g_2, r_2, b_d, g_d, r_d;

	IplImage* img = cvLoadImage("Theory/Scratch Pencil Photo/Input/lena.jpg");
	IplImage* img1 = cvCreateImage(cvSize(img->width, img->height), img->depth, img->nChannels);
	IplImage* img2 = cvCreateImage(cvSize(img->width, img->height), img->depth, img->nChannels);
	IplImage* dst = cvCreateImage(cvSize(img->width, img->height), img->depth, img->nChannels);
	IplImage* gray = cvCreateImage(cvGetSize(img), img->depth, 1);

	cvNamedWindow("Input", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("Output", CV_WINDOW_AUTOSIZE);

	cvShowImage("Input", img);
	cvNot(img, img1);
	//   cvSmooth(img1, img2, CV_BLUR, 25,25,0,0);
	cvSmooth(img, img2, CV_GAUSSIAN, 7, 7, 0, 0);

	for (row_1 = 0; row_1 < img1->height; row_1++)
	{
		for (col_1 = 0; col_1 < img1->width; col_1++)
		{
			b_1 = CV_IMAGE_ELEM(img1, uchar, row_1, col_1 * 3);
			g_1 = CV_IMAGE_ELEM(img1, uchar, row_1, col_1 * 3 + 1);
			r_1 = CV_IMAGE_ELEM(img1, uchar, row_1, col_1 * 3 + 2);

			b_2 = CV_IMAGE_ELEM(img2, uchar, row_1, col_1 * 3);
			g_2 = CV_IMAGE_ELEM(img2, uchar, row_1, col_1 * 3 + 1);
			r_2 = CV_IMAGE_ELEM(img2, uchar, row_1, col_1 * 3 + 2);

			//            b_d = b_1 + b_2;
			//            g_d = g_1 + g_2;
			//            r_d = r_1 + r_2;

			b_d = std::min(255, b_1 + b_2);
			g_d = std::min(255, g_1 + g_2);
			r_d = std::min(255, r_1 + r_2);

			dst->imageData[img1->widthStep * row_1 + col_1 * 3] = b_d;
			dst->imageData[img1->widthStep * row_1 + col_1 * 3 + 1] = g_d;
			dst->imageData[img1->widthStep * row_1 + col_1 * 3 + 2] = r_d;
		}
	}
	cvCvtColor(dst, gray, CV_BGR2GRAY);
	cvShowImage("Output", gray);

	cvWaitKey(0);
	cvReleaseImage(&img);
	cvReleaseImage(&img1);
	cvReleaseImage(&img2);
	cvReleaseImage(&dst);
	cvReleaseImage(&gray);
	cvDestroyWindow("Input");
	cvDestroyWindow("Output");
}