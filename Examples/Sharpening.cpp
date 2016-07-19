High / low pass filtering 


EmbossingOpenCV 설명


Blurring은 Embossing과 다 똑같은데 마스크만 다름.


책 227페이지 보기.. 1/5  , 1/25 - 영역이 넓어질수록 숫자가 작아짐.


GaussianFilteringOpenCV 설명

- 마스크 다름  책 230페이지 시그마 값에 따라서 종 모양이 달라짐.


SharpeningOpenCV 설명- 이것을 실습함.


-1 -1 -1

-1   9 -1

-1  -1  -1



#include <cv.h>
#include <cxcore.h>
#include <highgui.h>

#include "stdafx.h"

IplImage* maskProcess(IplImage* targetImage, double mask[3][3]);

int _tmain(int argc, _TCHAR* argv[])
{
	double SharpeningMask[3][3] = {{-1.,-1.,-1.},{-1.,9.,-1.}, {-1.,-1.,-1.}};


	IplImage* inputImage = cvLoadImage("lena.bmp",CV_LOAD_IMAGE_GRAYSCALE);

	IplImage *sharpeningImage = NULL;

	sharpeningImage = maskProcess(inputImage, SharpeningMask);

	cvShowImage("Input Image",inputImage);
	cvShowImage("Sharpening", sharpeningImage);
	cvWaitKey();

	cvDestroyAllWindows();
	cvReleaseImage(&inputImage);
	cvReleaseImage(&sharpeningImage);

	return 0;
}

IplImage* maskProcess(IplImage* targetImage, double mask[3][3])
{
	int i, j, m, n;
	double s = 0.0;
	CvScalar tempScalar;

	IplImage* tempImputImage = cvCreateImage(cvSize(targetImage->width+2, targetImage->height+2), IPL_DEPTH_8U, 1); 
	IplImage* outImage = cvCreateImage(cvGetSize(targetImage), IPL_DEPTH_8U, 1);

	cvSetZero(tempInputImage);

	for( i=0; i<targetImage->height; i++)
		for(j=0; j<targetImage->width; j++)
			cvSet2D(tempInputImage,i+1,j+1, cvGet2D(targetImage, i, j));

		for(i=0;i<outputImage->height;i++)
		{
			for(j=0;j<outImage->width;j++)
			{
				for(n=0;n<3;n++)
				{
					for(m=0;m<3;m++)
					{
						tempScalar = cvGet2D(tempInputImage, i+n,j+m);
						s +=Mask[n][m] * tempScalar.val[0];
					}
				}
				cvSet2D(outImage, i, j, cvScalar(s));
				s = 0.0;
			}
		}
	
		cvReleaseImage(&tempInputImage);

		return outImage;
}