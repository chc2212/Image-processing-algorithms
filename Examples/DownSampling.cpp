
#include "stdafx.h"

#include <cv.h>
#include <cxcore.h>
#include <highgui.h>

#define DOWNSAMPLE_RATE 4


/***
1. image loading

2. image data I/O  - process

3. Image Save

*/


int _tmain(int argc, _TCHAR* argv[])
{
	int i, j;
	int re_height, re_width;
	
	IplImage *inputImage = cvLoadImage("lena.bmp",CV_LOAD_IMAGE_COLOR);
	
	re_height = inputImage->height / DOWNSAMPLE_RATE;
	//축소 영상의 세로 길이를 계산
	re_width = inputImage->width / DOWNSAMPLE_RATE;
	// 축소 영상의 가로 길이를 계산

	IplImage *outputImage = cvCreateImage(cvSize(re_height,re_width), inputImage->depth, inputImage->nChannels);
	//축소 영상을 위한 메모리 할당


	//output 이미지에 값 넣기.
	for(i=0; i<re_height; i++)
	{
		for(j=0; j<re_width;j++)
		{
			cvSet2D(outputImage,i, j, cvGet2D(inputImage, i*DOWNSAMPLE_RATE, j*DOWNSAMPLE_RATE));
		}
	}

	cvNamedWindow("Input Image",CV_WINDOW_AUTOSIZE);
	cvNamedWindow("Output Image",CV_WINDOW_AUTOSIZE);
	
	cvShowImage("Input Image",inputImage);
	cvShowImage("Output Image",outputImage);
	cvWaitKey();

	cvDestroyAllWindows();
	cvReleaseImage(&inputImage);
	cvReleaseImage(&outputImage);

	return 0;
}

