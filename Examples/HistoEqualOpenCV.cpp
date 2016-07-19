
#include "stdafx.h"
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>

#define LOW 0
#define HIGH 255

#define HISTOGRAM_SIZE 256

void imageHistogramViewer(IplImage* image, char* srcWindowName, char* histoWindowName,
									IplImage* compareImage = NULL, char* compWindowName = NULL, char* compHistoWindowName = NULL);


int _tmain(int argc, _TCHAR* argv[])
{
	int i, j, value;
	CvScalar temp;

	double HIST[HISTOGRAM_SIZE];
	double sum_Of_HIST[HISTOGRAM_SIZE];
	double SUM = 0.0;

	IplImage *inputImage = cvLoadImage("lena.bmp", CV_LOAD_IMAGE_GRAYSCALE);
	IplImage *histogramEqualized = cvCreateImage(cvGetSize(inputImage), IPL_DEPTH_8U, 1);

	//초기화
	for(i=0; i<HISTOGRAM_SIZE; i++) {
		HIST[i] = LOW;
	}


	//빈도 수 조사
	for(i=0;  i < inputImage->height; i++) {
		for(j=0; j<inputImage->width; j++) {
			temp = cvGet2D(inputImage, i, j);
			value = (int)temp.val[0];
			HIST[value]++;
		}
	}

	//누적 히스토그램 생성
	for(i=0; i<HISTOGRAM_SIZE; i++) {
		SUM += HIST[i];
		sum_Of_HIST[i] = SUM;
	}

	//입력 영상을 평활화된 영상으로 출력

	for(i=0; i<inputImage->height; i++){
		for(j=0; j<inputImage->width; j++){
			temp = cvGet2D(inputImage, i, j);
			cvSet2D(histogramEqualized, i, j, cvScalar(sum_Of_HIST[ (int)temp.val[0] ] * HIGH/(inputImage->height*inputImage->width) ));
		}
	}

	imageHistogramViewer(inputImage, "Input Image", "Input Histogram", histogramEqualized, "HistoEqualized", "HistoEqualized Histogram");

	cvReleaseImage(&inputImage);
	cvReleaseImage(&histogramEqualized);

	return 0;
}



void imageHistogramViewer(IplImage* image, char* srcWindowName, char* histoWindowName,
						  IplImage* compareImage, char* compWindowName, char* compHistoWindowName)
{
	//히스토그램의 값은 0~255
	//히스토그램의 크기 값을 MAX=255로 정규화하여 출력
	//히스토그램의 크기 : 256*256 지정

	int i, j, value;
	CvScalar temp;

	double HIST[HISTOGRAM_SIZE];
	unsigned char scale_HIST[HISTOGRAM_SIZE];
	double MAX, MIN, DIF;

	IplImage *histogramImage = cvCreateImage(cvSize(HISTOGRAM_SIZE, HISTOGRAM_SIZE+20), IPL_DEPTH_8U, 1);
	IplImage *compHistogramImage = cvCreateImage(cvSize(HISTOGRAM_SIZE, HISTOGRAM_SIZE+20), IPL_DEPTH_8U, 1);

	//초기화
	for(i=0; i<HISTOGRAM_SIZE; i++){
		HIST[i] = LOW;
	}

	//빈도 수 조사
	for(i=0; i<image->height; i++) {
		for(j=0; j<image->width; j++){
			temp = cvGet2D(image, i, j);
			value = (int)temp.val[0];
			HIST[value]++;
		}
	}

	//정규화

	MAX = HIST[0]; //최대값을 찾기 위한 초기값
	MIN = HIST[0]; //최소값을 찾기 위한 초기값

	for(i=0; i<HISTOGRAM_SIZE; i++) {
		//입력 영상의 최대 누적값 찾기
		if(HIST[i] > MAX){
			MAX = HIST[i];
		}

		//입력 영상의 최소 누적값 찾기
		if(HIST[i] < MIN){
			MIN = HIST[i];
		}
	}

	DIF = MAX - MIN;

	//정규화된 히스토그램
	for(i=0; i<HISTOGRAM_SIZE; i++){
		scale_HIST[i] = (unsigned char)((HIST[i] - MIN) * HIGH / DIF);
	}

	//정규화된 히스토그램 출력

	cvSet(histogramImage, cvScalar(255));

	//정규화된 히스토그램의 값은 출력 배열에 검은 점(0)으로 표현
	for(i=0; i<HISTOGRAM_SIZE; i++){
		for(j=0; j<scale_HIST[i]; j++){
			cvSet2D(histogramImage, HISTOGRAM_SIZE-j-1, i, cvScalar(0));
		}
	}

	//히스토그램을 출력하고 그 아래 부분에 히스토그램의 색을 표시
	for(i=HISTOGRAM_SIZE; i<HISTOGRAM_SIZE+5; i++){
		for(j=0; j<HISTOGRAM_SIZE; j++){
			cvSet2D(histogramImage, i, j, cvScalar(255));
		}
	}

	for(i=HISTOGRAM_SIZE+5; i<HISTOGRAM_SIZE+20; i++){
		for(j=0; j<HISTOGRAM_SIZE; j++){
			cvSet2D(histogramImage, i, j, cvScalar(j));
		}
	}

	cvShowImage(srcWindowName, image);
	cvShowImage(histoWindowName, histogramImage);


	if(compareImage != NULL && compWindowName != NULL && compHistoWindowName != NULL){
		//초기화
		for(i=0; i<HISTOGRAM_SIZE; i++){
			HIST[i] = LOW;
		}


		///////////////////////////////////////////////////////////////////////////////////
		//빈도 수 조사
		for(i=0; i<compareImage->height; i++) {
			for(j=0; j<compareImage->width; j++){
				temp = cvGet2D(compareImage, i, j);
				value = (int)temp.val[0];
				HIST[value]++;
			}
		}

		//정규화

		MAX = HIST[0]; //최대값을 찾기 위한 초기값
		MIN = HIST[0]; //최소값을 찾기 위한 초기값

		for(i=0; i<HISTOGRAM_SIZE; i++) {
			//입력 영상의 최대 누적값 찾기
			if(HIST[i] > MAX){
				MAX = HIST[i];
			}

			//입력 영상의 최소 누적값 찾기
			if(HIST[i] < MIN){
				MIN = HIST[i];
			}
		}

		DIF = MAX - MIN;

		//정규화된 히스토그램
		for(i=0; i<HISTOGRAM_SIZE; i++){
			scale_HIST[i] = (unsigned char)((HIST[i] - MIN) * HIGH / DIF);
		}

		//정규화된 히스토그램 출력

		cvSet(compHistogramImage, cvScalar(255));

		//정규화된 히스토그램의 값은 출력 배열에 검은 점(0)으로 표현
		for(i=0; i<HISTOGRAM_SIZE; i++){
			for(j=0; j<scale_HIST[i]; j++){
				cvSet2D(compHistogramImage, HISTOGRAM_SIZE-j-1, i, cvScalar(0));
			}
		}

		//히스토그램을 출력하고 그 아래 부분에 히스토그램의 색을 표시
		for(i=HISTOGRAM_SIZE; i<HISTOGRAM_SIZE+5; i++){
			for(j=0; j<HISTOGRAM_SIZE; j++){
				cvSet2D(compHistogramImage, i, j, cvScalar(255));
			}
		}

		for(i=HISTOGRAM_SIZE+5; i<HISTOGRAM_SIZE+20; i++){
			for(j=0; j<HISTOGRAM_SIZE; j++){
				cvSet2D(compHistogramImage, i, j, cvScalar(j));
			}
		}

		cvShowImage(compWindowName, compareImage);
		cvShowImage(compHistoWindowName, compHistogramImage);


	}

	cvWaitKey();

	cvDestroyAllWindows();
	cvReleaseImage(&histogramImage);
	cvReleaseImage(&compHistogramImage);

	return ;
}


