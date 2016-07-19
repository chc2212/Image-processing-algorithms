
#include "stdafx.h"
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>

#define LOW 0
#define HIGH 255
#define HISTOGRAM_SIZE 256

void ImageHistogramViewer(IplImage *image, char* srcWindowName, char* histoWindowName, IplImage* compareImage = NULL, char* compWindowName = NULL, char* comHistoWindowName = NULL);

int _tmain(int argc, _TCHAR* argv[])
{
	IplImage *inputImage = cvLoadImage("lena.bmp",CV_LOAD_IMAGE_GRAYSCALE);
	
	imageHistogramViewer(inputImage,"Input Image", "Histogram");

	

	




	return 0;
}


void ImageHistogramViewer(IplImage *image, char* srcWindowName, char* histoWindowName, IplImage* compareImage = NULL, char* compWindowName = NULL, char* comHistoWindowName = NULL)
{
	int i, j, value;
	CvScalar temp;

	double HIST[HISTOGRAM_SIZE];
	unsigned char scale_HIST[HISTOGRAM_SIZE];
	double MIN, MAX, DIF;

	IplImage *histogramImage = cvCreateImage(cvSize(HISTOGRAM_SIZE,HISTOGRAM_SIZE+20), IPL_DEPTH_8U, 1);
	IplImage *compHistogramImage = cvCreateImage(cvSize(HISTOGRAM_SIZE,HISTOGRAM_SIZE+20), IPL_DEPTH_8U, 1);

	for(i=0; i<HISTOGRAM_SIZE; i++)
		HIST[i] = LOW;

	for(i=0;i<image->height;i++)
	{
		for(j=0;j<image->width;j++)
		{
			temp = cvGet2D(image,i,j);
			value = (int)temp.val[0];
			HIST[value++];
		}
	}

	MAX = HIST[0];
	MIN = HIST[0];

	for(i=0; i<HISTOGRAM_SIZE ; i++)
	{
		if(HIST[i] > MAX)
			MAX = HIST[i];
		if(HIST[i] < MIN)
			MIN = HIST[i];
	}
	
	DIF = MAX - MIN;

	for(i=0; i<HISTOGRAM_SIZE;i++)
		scale_HIST[i] = (unsigned char) ((HIST[i]-MIN) * HIGH/DIF);

	cvSet(histogramImage, cvScalar(255));


}
