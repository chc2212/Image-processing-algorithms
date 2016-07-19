

#include "stdafx.h"
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>

#define AND_CONSTANT 128
#define OR_CONSTANT 128
#define XOR_CONSTANT 128


int _tmain(int argc, _TCHAR* argv[])
{
	int i, j;

	IplImage *inputImage = cvLoadImage("lena.bmp", CV_LOAD_IMAGE_GRAYSCALE);
	IplImage *andImage = cvCreateImage(cvGetSize(inputImage), inputImage->depth, inputImage->nChannels);
	IplImage *orImage = cvCreateImage(cvGetSize(inputImage), inputImage->depth, inputImage->nChannels);
	IplImage *xorImage = cvCreateImage(cvGetSize(inputImage), inputImage->depth, inputImage->nChannels);

	CvScalar pixelValue, temp;

	for(i=0; i<inputImage->height; i++){
		for(j=0; j<inputImage->width; j++){
			pixelValue = cvGet2D(inputImage, i, j);
			unsigned char charPixel = (unsigned char)pixelValue.val[0];
			
			//AND 연산
			if((charPixel & (unsigned char)AND_CONSTANT) >= 255){
				temp.val[0] = 255;
				cvSet2D(andImage, i, j, temp);
			}

			// OR 연산
			if((charPixel | (unsigned char)OR_CONSTANT) >=255){
				temp.val[0] = 255;
				cvSet2D(orImage, i, j, temp);
			} else if((charPixel | (unsigned char)OR_CONSTANT) <0 ){
				temp.val[0] = 0;
				cvSet2D(orImage, i, j, temp);
			} else {
				unsigned char t = charPixel | (unsigned char)OR_CONSTANT;
				temp.val[0] = (double)t;
				cvSet2D(orImage, i, j, temp);
			}

			// XOR연산
			if((charPixel ^ (unsigned char)XOR_CONSTANT) >= 255) {
				temp.val[0] = 255;
				cvSet2D(xorImage, i, j, temp);
			}else if((charPixel ^ (unsigned char)XOR_CONSTANT) < 0) {
				temp.val[0] = 0;
				cvSet2D(xorImage, i, j, temp);
			} else {
				unsigned char t = charPixel ^ (unsigned char)XOR_CONSTANT;
				temp.val[0] = (double)t;
				cvSet2D(xorImage, i, j, temp);
			}
		}
	}

	cvNamedWindow("Input Image", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("AND Image", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("OR Image", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("XOR Image", CV_WINDOW_AUTOSIZE);
	cvShowImage("Input Image", inputImage);
	cvShowImage("And Image", andImage);
	cvShowImage("OR Image", orImage);
	cvShowImage("XOR Image", xorImage);

	cvWaitKey();

	cvDestroyAllWindows();
	cvReleaseImage(&inputImage);
	cvReleaseImage(&andImage);
	cvReleaseImage(&orImage);
	cvReleaseImage(&xorImage);

	return 0;
}

