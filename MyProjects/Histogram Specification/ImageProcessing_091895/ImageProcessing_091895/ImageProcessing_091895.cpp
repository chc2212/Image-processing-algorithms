// 091895 조호철
// visual studio 2008로 제작

#include "stdafx.h"

#include <cv.h>
#include <cxcore.h>
#include <highgui.h>

#define LOW 0
#define HIGH 255

#define HISTOGRAM_SIZE 256

void imageHistogramViewer(IplImage* image, char* srcWindowName, char* histoWindowName,
	IplImage* compareImage = NULL, char* compWindowName = NULL, char* compHistoWindowName = NULL,
	IplImage* compare2Image = NULL, char* comp2WindowName = NULL, char* comp2HistoWindowName = NULL);


//메인 - 메인에 히스토그램명세화를 구현하였습니다. 함수 imageHistogramViewer는 기존에는 한번에 두 이미지와 히스토그램이 보이던 것을 한번에 세 이미지와 히스토그램이 보이게 수정하였습니다.
int _tmain(int argc, _TCHAR* argv[])
{

	//각종 변수들 선언
	int i, j, value, dvalue, top, bottom;

	top = 255;
	bottom = top - 1;


	CvScalar temp;
	CvScalar dtemp;
	CvScalar DADD;

	
	unsigned char m_TABLE[256], dSum_Of_ScHIST[256];
	double HIST[HISTOGRAM_SIZE];
	double DHIST[HISTOGRAM_SIZE];
	double DMIN, DMAX;

	double sum_Of_HIST[HISTOGRAM_SIZE];
	double SUM = 0.0;
	double dsum_Of_HIST[HISTOGRAM_SIZE];
	double DSUM = 0.0;


	//레나 이미지와 원하는 히스토그램 이미지인 baboon.bmp를 불러옴.
	IplImage *inputImage = cvLoadImage("lena.bmp", CV_LOAD_IMAGE_GRAYSCALE);
	IplImage *dinputImage = cvLoadImage("baboon.bmp", CV_LOAD_IMAGE_GRAYSCALE);


	//히스토그램평활화 , 히스토그램명세화 작업할 이미지 생성
	IplImage *histogramEqualized = cvCreateImage(cvGetSize(inputImage), IPL_DEPTH_8U, 1);
	IplImage *m_OutputImage = cvCreateImage(cvGetSize(dinputImage), IPL_DEPTH_8U, 1);


	//초기화
	for(i=0; i<HISTOGRAM_SIZE; i++) {
		HIST[i] = LOW;
	}

	for(i=0; i<HISTOGRAM_SIZE; i++) {
		DHIST[i] = LOW;
	}

	for(i=0; i<HISTOGRAM_SIZE; i++) {
		m_TABLE[i] = LOW;
	}

	//빈도 수 조사
	for(i=0;  i < inputImage->height; i++) {
		for(j=0; j<inputImage->width; j++) {
			temp = cvGet2D(inputImage, i, j);
			value = (int)temp.val[0];
			HIST[value]++;
		}
	}

	for(i=0;  i < dinputImage->height; i++) {
		for(j=0; j<dinputImage->width; j++) {
			dtemp = cvGet2D(dinputImage, i, j);
			dvalue = (int)dtemp.val[0];
			DHIST[dvalue]++;
		}
	}


	//누적 히스토그램 생성
	for(i=0; i<HISTOGRAM_SIZE; i++) {
		SUM += HIST[i];
		sum_Of_HIST[i] = SUM;

		DSUM += DHIST[i];
		dsum_Of_HIST[i] = DSUM;

	}

	//입력 영상을 평활화된 영상으로 출력

	for(i=0; i<inputImage->height; i++){
		for(j=0; j<inputImage->width; j++){
			temp = cvGet2D(inputImage, i, j);
			cvSet2D(histogramEqualized, i, j, cvScalar(sum_Of_HIST[ (int)temp.val[0] ] * HIGH/(inputImage->height*inputImage->width) ));
		}
	}
	
	
	DMIN = dsum_Of_HIST[0]; // baboon 이미지의 누적히스토그램에서 최소,최대 누적값을 지정.
	DMAX = dsum_Of_HIST[255];  // 누적 히스토그램에서 배열의 인덱스가 0이면 최소 누적값이며, 255이면 최대 누적값이다.

	
	//baloon 이미지의 누적 히스토그램을 정규화(0~255 사이의 값으로 정규화)	
	for(i=0; i<256; i++){
		dSum_Of_ScHIST[i] = (unsigned char)((dsum_Of_HIST[i]-DMIN)*HIGH / (DMAX - DMIN));
	}
	

	//룩업 테이블을 이용한 명세화
	for( ; ; ){
		for(i=dSum_Of_ScHIST[bottom] ; i <= dSum_Of_ScHIST[top] ; i++){
			m_TABLE[i] = top; //룩업테이블 생성
		}
		top = bottom;
		bottom=bottom-1;

		if(bottom<-1)
			break;
	}

	//평활화된 lena이미지의 명도값을 룩업테이블의 인덱스로 사용하여 output 이미지 생성
	for(i=0; i<inputImage->height; i++){
		for(j=0; j<inputImage->width; j++){
			DADD = cvGet2D(histogramEqualized, i, j);
			cvSet2D(m_OutputImage, i, j, cvScalar(m_TABLE[(int)DADD.val[0]]));	
		}
	}



	//이미지와 히스토그램 창에 띄워서 보이기
	imageHistogramViewer(inputImage, "Input", "Input Histo" , dinputImage, "D_input", "D_Input Histo", m_OutputImage, "Result Image", "Result Histo");
	

	cvReleaseImage(&dinputImage);
	cvReleaseImage(&m_OutputImage);
	cvReleaseImage(&inputImage);
	cvReleaseImage(&histogramEqualized);

	return 0;
}



//함수 imageHistogramViewer

//기존에 ppt에 있던 imageHistogramViewer함수에서 세 이미지와 히스토그램이 한번에 출력되도록 변경
void imageHistogramViewer(IplImage* image, char* srcWindowName, char* histoWindowName,
	IplImage* compareImage, char* compWindowName, char* compHistoWindowName,
	IplImage* compare2Image, char* comp2WindowName, char* comp2HistoWindowName)
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
	IplImage *comp2HistogramImage = cvCreateImage(cvSize(HISTOGRAM_SIZE, HISTOGRAM_SIZE+20), IPL_DEPTH_8U, 1);

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


	//두번째
	////////////////////////////////////////////////////////////
	if(compareImage != NULL && compWindowName != NULL && compHistoWindowName != NULL){
		//초기화
		for(i=0; i<HISTOGRAM_SIZE; i++){
			HIST[i] = LOW;
		}

	
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

	//세번째
	////////////////////////////////////////////////////////////

	if(compare2Image != NULL && comp2WindowName != NULL && comp2HistoWindowName != NULL){
		//초기화
		for(i=0; i<HISTOGRAM_SIZE; i++){
			HIST[i] = LOW;
		}

	
		//빈도 수 조사
		for(i=0; i<compare2Image->height; i++) {
			for(j=0; j<compare2Image->width; j++){
				temp = cvGet2D(compare2Image, i, j);
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

		cvSet(comp2HistogramImage, cvScalar(255));

		//정규화된 히스토그램의 값은 출력 배열에 검은 점(0)으로 표현
		for(i=0; i<HISTOGRAM_SIZE; i++){
			for(j=0; j<scale_HIST[i]; j++){
				cvSet2D(comp2HistogramImage, HISTOGRAM_SIZE-j-1, i, cvScalar(0));
			}
		}

		//히스토그램을 출력하고 그 아래 부분에 히스토그램의 색을 표시
		for(i=HISTOGRAM_SIZE; i<HISTOGRAM_SIZE+5; i++){
			for(j=0; j<HISTOGRAM_SIZE; j++){
				cvSet2D(comp2HistogramImage, i, j, cvScalar(255));
			}
		}

		for(i=HISTOGRAM_SIZE+5; i<HISTOGRAM_SIZE+20; i++){
			for(j=0; j<HISTOGRAM_SIZE; j++){
				cvSet2D(comp2HistogramImage, i, j, cvScalar(j));
			}
		}

		cvShowImage(comp2WindowName, compare2Image);
		cvShowImage(comp2HistoWindowName, comp2HistogramImage);
	}

	cvWaitKey();

	cvDestroyAllWindows();
	cvReleaseImage(&histogramImage);
	cvReleaseImage(&compHistogramImage);
	cvReleaseImage(&comp2HistogramImage);

	return ;
}


