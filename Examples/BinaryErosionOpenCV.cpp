
#include "stdafx.h"

#include <cv.h>
#include <cxcore.h>
#include <highgui.h>

#define THRESHOLD 100

int _tmain(int argc, _TCHAR* argv[])
{
	int i, j, n, m;
	CvScalar tempValue;
	double S= 0.0;
	double Mask[3][3] = {{255., 255., 255.}, {255., 255., 255.}, {255., 255., 255.}};
	//ħ�� ������ ���� ����ũ

	IplImage *inputImage = cvLoadImage("lena.bmp", CV_LOAD_IMAGE_GRAYSCALE);
	IplImage *binaryImage = cvCreateImage(cvGetSize(inputImage), 8, 1);
	IplImage *tempImage = cvCreateImage(cvSize(inputImage->width+2, inputImage->height+2), 8, 1);
	IplImage *outputImage = cvCreateImage(cvGetSize(inputImage), 8, 1);

	for(i=0; i<inputImage->height; i++){ //���������� ����ȭ
		for(j=0; j<inputImage->width; j++){
			tempValue = cvGet2D(inputImage, i, j);
			if(tempValue.val[0] > THRESHOLD) {
				cvSet2D(binaryImage, i, j, cvScalar(255));
			}	else	{
				cvSet2D(binaryImage, i, j, cvScalar(0));
			}
		}
	}

	for(i=0; i<binaryImage->height; i++) {
		for(j=0; j<binaryImage->width; j++){
			cvSet2D(tempImage, i+1, j+1, cvGet2D(binaryImage, i, j));
		}
	}

	for(i=0; i<binaryImage->height; i++){
		for(j=0; j<binaryImage->width; j++){
			for(n=0; n<3; n++){
				for(m=0; m<3; m++){
					tempValue = cvGet2D(tempImage, i+n, j+m);
					if(Mask[n][m] == tempValue.val[0]){
						//����ũ�� ���� ���� �ִ��� ����
						S += 1.0;
					}
				}
			}
			if(S==9.0){
				cvSet2D(outputImage, i, j, cvScalar(255));
				//���� ��� ��ġ�ϸ� ��� ���� 255
			}	else {
				cvSet2D(outputImage, i, j, cvScalar(0));
				//��� ��ġ���� ������ ��� ���� 0
			}
			S =0.0; // reset
		}
	}

	cvShowImage("Input Image", inputImage);
	cvShowImage("Binary Image", binaryImage);
	cvShowImage("Output Image", outputImage);

	cvWaitKey();

	cvReleaseImage(&inputImage);
	cvReleaseImage(&binaryImage);
	cvReleaseImage(&tempImage);
	cvReleaseImage(&outputImage);

	return 0;
}





