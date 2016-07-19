// PointProcessing_Opencv.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>

#define GAMMA_CONSTANT 0.85


int _tmain(int argc, _TCHAR* argv[])
{
	int i, j;
	IplImage *inputImage = cvLoadImage("lena.bmp",CV_LOAD_IMAGE_GRAYSCALE);
	IplImage *gammaCorrection = cvCreateImage(cvGetSize(inputImage), inputImage->depth, inputImage->nChannels);
	
	CvScalar pixelValue, temp;

	for(i=0; i<inputImage->height;i++)
	{
		for(j=0; j<inputImage->width;j++)
		{
			pixelValue = cvGet2D(inputImage, i, j);
			temp.val[0] = pow(pixelValue.val[0], 1/GAMMA_CONSTANT);
			if(temp.val[0] < 0)
			{
				temp.val[0] = 0;
				cvSet2D(gammaCorrection,i,j,temp);				
			}
			else if(temp.val[0] > 255)
			{
				temp.val[0]=255;
				cvSet2D(gammaCorrection,i,j,temp);
			}
			else 
			{
				cvSet2D(gammaCorrection,i,j,temp);
			}

		}
	}

	cvNamedWindow("input",CV_WINDOW_AUTOSIZE);
	cvNamedWindow("Gamma",CV_WINDOW_AUTOSIZE);

	cvShowImage("input",inputImage);
	cvShowImage("Gamma",gammaCorrection);

	cvWaitKey();

	cvDestroyAllWindows();
	cvReleaseImage(&inputImage);
	cvReleaseImage(&gammaCorrection);


	return 0;
}