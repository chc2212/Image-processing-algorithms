
#include "stdafx.h"


#include <cv.h>
#include <cxcore.h>
#include <highgui.h>

double doubleABS(double x);

int _tmain(int argc, _TCHAR* argv[])
{
	int i, j , n, m;
	double max;
	CvScalar inputPixel, tempPixel, tempPixel2;

	IplImage *inputImage = cvLoadImage("baboon.bmp", CV_LOAD_IMAGE_GRAYSCALE);
	IplImage *tempImage = cvCreateImage(CvSize(inputImage->width+2, inputImage->height+2), IPL_DEPTH_8U, 1);

	IplImage *outputImage = cvCreateImage(cvSize(inputImage->width, inputImage->height), IPL_DEPTH_8U, 1);
	cvSetZero(tempImage);

	for(i=0;i<inputImage->height;i++)
	{
		for(j=0;j<inputImage->width; j++)
		{
			inputPixel = cvGet2D(inputImage, i, j);

			cvSet2D(tempImage, i+1, j+1, cvScalar(inputPixel.val[0]));
		}
		
	}

	for(i=0;i<outputImage->height;i++)
	{
		for(j=0;j<outputImage->width;j++)
		{
			max = 0.0;
			for(n=0;n<3;n++)
			{
				for(m=0;m<3;m++)
				{
					tempPixel = cvGet2D(tempImage, i+1, j+1);
					tempPixel2 = cvGet2D(tempImage, i+n, j+m);

					if(doubleABS(tempPixel.val[0]-tempPixel2.val[0]) >=max)
						max = doubleABS(tempPixel.val[0]-tempPixel2.val[0];
					
				}
			}
			cvSet2D(outputImage, i, j, cvScalar(max));

		}
	}

	cvShowImage("input Image", inputImage);
	cvShowImage("output Image", outputImage);

	cvWaitKey();

	cvDestroyAllWindows();
	cvReleaseImage(&inputImage);
	cvReleaseImage(&tempImage);
	cvReleaseImage(&outputImage);


	


	return 0;
}

double doubleABS(double x)
{
	if(x>=0)
		return X;
	else
		return -X;
}