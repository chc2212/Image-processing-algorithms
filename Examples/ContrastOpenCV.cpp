Typing of korean is impossible.

p = 2;
q = 3;

p && q  -> true

value > 0 -> true

value <0 -> false


(p   &    q) -> 2 

10  && 11 -> 10



#include "stdafx.h"

#include <cv.h>
#include <cxcore.h>
#include <highgui.h>

#define AND_CONSTANT 128

int _tmain(int argc, _TCHAR* argv[])
{
	int i,j;
	IplImage *inputImage = cvLoadImage("lena.bmp",CV_LOAD_IMAGE_GRAYSCALE);
	IplImage *andImage = cvCreateImage(cvGetSize(inputImage), inputImage->depth, inputImage->nChannels);
	
	CvScalar pixelValue, temp;

	for(i=0;i<inputImage->height;i++)
	{
		for(j=0;j<inputImage->width;j++)
		{
			pixelValue = cvGet2D(inputImage, i, j);
			unsigned char charPixel = (unsigned char)pixelValue.val[0]; // waste low number (ex)1.223...~)

			if ( charPixel & (unsigned char)AND_CONSTANT >= 255)
			{
				temp.val[0] = 255;
				cvSet2D(andImage, i, j, temp);
			}

			//It doesn't consider case of pixel<0 because  account of AND is always larger than 0.
			else
			{
				unsigned char t = charPixel & (unsigned char)AND_CONSTANT;
				temp.val[0] = (double)t;
				cvSet2D(andImage, i, j, temp);
			}
		}
	}

	cvNamedWindow("input",CV_WINDOW_AUTOSIZE);
	cvNamedWindow("and Image",CV_WINDOW_AUTOSIZE);

	cvShowImage("input",inputImage);
	cvShowImage("and Image",andImage);

	cvWaitKey();
	
	cvDestroyAllWindows();
	cvReleaseImage(&inputImage);
	cvReleaseImage(&andImage);

	return 0;
}