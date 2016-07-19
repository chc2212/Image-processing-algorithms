
#include "stdafx.h"

#include <cv.h>
#include <cxcore.h>
#include <highgui.h>

#define ZOOM_RATE 2

int _tmain(int argc, _TCHAR* argv[])
{
	int i, j;

	IplImage* inputImage = cvLoadImage("lena.bmp", CV_LOAD_IMAGE_GRAYSCALE);
	IplImage* outputImage = cvCreateImage(cvSize(inputImage->width*ZOOM_RATE, inputImage->height*ZOOM_RATE), 8, 1);

	for(i=0;i<outputImage->height;i++)
	{
		for(j=0;j<outputImage->width;j++)
		{
			cvSet2D(outputImage, i, j, cvGet2D(outputImage, i, j, cvGet2D(inputImage, i/ZOOM_RATE, j/ZOOM_RATE));
		}
	}

	cvShowImage("input image", inputImage);
	cvShowImage("output image", outputImage);
	cvWaitKey();

	cvSaveImage("lena_Nearest.bmp", outputImage);

	cvDestroyAllWindows();
	cvReleaseImage(&inputImage);
	cvReleaseImage(&outputImage);

	return 0;
}






#include "stdafx.h"

#include <cv.h>
#include <cxcore.h>
#include <highgui.h>

#define ZOOM_RATE 2.0

int _tmain(int argc, _TCHAR* argv[])
{
	int i, j, i_H, i_W;
	double r_H, r_W, s_H, s_W;

	CvScalar C1, C2, C3, C4, newValue;

	ImlImage* inputImage = cvLoadImage("lena.bmp", CV_LOAD_IMAGE_GRAYSCALE);
	
	IplImage* outputImage = cvCreateImage(cvSize(inputImage->width*ZOOM_RATE, inputImage->height*ZOOM_RATE), 8, 1);
	
	for(i=0; i<outputImage->height;i++)
	{
		for(j=0; j<outputImage->width; j++)
		{
			r_H = i / ZOOM_RATE; 
			r_W = j / ZOOM_RATE;

			i_H = (int)floor(r_H);
			i_W = (int)floor(r_W);

			if ((i_H < 0 || i_H >= inputImage->height-1) || i_W < 0 || i_H >= (inputImage->width-1))
				cvSet2D(outputImage, i, j, cvScalar(255));

			else
			{
				C1 = cvGet2D(inputImage, i_H, i_W);
				C2 = cvGet2D(inputImage, i_H, i_W+1);
				C3 = cvGet2D(inputImage, i_H+1, i_W+1);
				C4 = cvGet2D(inputImage, i_H+1, i_W);

				newValue.val[0] = 
			}
		}
	}


	return 0;
}


