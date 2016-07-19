

#include "stdafx.h"

#include <cv.h>
#include <cxcore.h>
#include <highgui.h>

#define SUB_SAMPLING_RATE 2 // 서브 샘플링 비율

void Swap(double *a, double *b);
void BubleSort(double *A, int MAX);

int _tmain(int argc, _TCHAR* argv[])
{
	int i, j, n, m, index =0;
	double *Mask, *OutPutValue;
	CvScalar tempScalar;

	Mask = new double[SUB_SAMPLING_RATE*SUB_SAMPLING_RATE]; //마스크의 크기 결정

	IplImage *inputImage = cvLoadImage("lena.bmp", CV_LOAD_IMAGE_GRAYSCALE);
	IplImage *outputImage = cvCreateImage(cvSize((inputImage->width+1)/SUB_SAMPLING_RATE, (inputImage->height+1)/SUB_SAMPLING_RATE), 8, 1);
	IplImage *tempImage = cvCreateImage(cvSize(inputImage->width+1, inputImage->height+1), 8, 1);

	OutPutValue = new double[(inputImage->width+1)/SUB_SAMPLING_RATE * (inputImage->height+1)/SUB_SAMPLING_RATE];
	
	cvSetZero(tempImage);
	
	for(i=0; i<inputImage->height; i++){
		for(j=0; j<inputImage->width; j++){
			cvSet2D(tempImage, i, j, cvGet2D(inputImage, i, j));
		}
	}

	for(i=0; i<inputImage->height-1; i=i+SUB_SAMPLING_RATE) {
		for(j=0; j<inputImage->width-1; j=j+SUB_SAMPLING_RATE){
			for(n=0; n<SUB_SAMPLING_RATE; n++){
				for(m=0; m<SUB_SAMPLING_RATE; m++){
					tempScalar = cvGet2D(tempImage, i+n, j+m);
					Mask[n*SUB_SAMPLING_RATE + m] = tempScalar.val[0];
					//입력 영상을 블록으로 잘라 마스크 배열에 저장
				}
			}
			BubleSort(Mask, SUB_SAMPLING_RATE*SUB_SAMPLING_RATE); //마스크에 저장된 값을 정렬
			OutPutValue[index++] = Mask[(int)(SUB_SAMPLING_RATE*SUB_SAMPLING_RATE/2)]; //정렬된 값 중 가운데 값을 선택
		}
	}

	for(i=0; i<outputImage->height; i++){
		for(j=0; j<outputImage->width; j++){
			cvSet2D(outputImage, i, j, cvScalar(OutPutValue[i*outputImage->width + j]));  //가운데 값을 출력
		}
	}

	cvShowImage("Input Image", inputImage);
	cvShowImage("Median Sub Sampling Image", outputImage);
	cvWaitKey();

	cvDestroyAllWindows();
	cvReleaseImage(&inputImage);
	cvReleaseImage(&tempImage);
	cvReleaseImage(&outputImage);

	return 0;
}

void Swap(double *a, double *b)
{ //데이터 교환 함수
	double temp;
	temp = *a;
	*a = *b;
	*b = temp;
}

void BubleSort(double *A, int MAX)
{	//데이터의 정렬을 처리하는 함수
	int i, j;

	for(i=0; i<MAX; i++){
		for(j=0; j<MAX-1; j++){
			if(A[j] > A[j+1]){
				Swap(&A[j], &A[j+1]);
			}
		}
	}
}



