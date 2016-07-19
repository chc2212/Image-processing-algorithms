
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

	//�ʱ�ȭ
	for(i=0; i<HISTOGRAM_SIZE; i++) {
		HIST[i] = LOW;
	}


	//�� �� ����
	for(i=0;  i < inputImage->height; i++) {
		for(j=0; j<inputImage->width; j++) {
			temp = cvGet2D(inputImage, i, j);
			value = (int)temp.val[0];
			HIST[value]++;
		}
	}

	//���� ������׷� ����
	for(i=0; i<HISTOGRAM_SIZE; i++) {
		SUM += HIST[i];
		sum_Of_HIST[i] = SUM;
	}

	//�Է� ������ ��Ȱȭ�� �������� ���

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
	//������׷��� ���� 0~255
	//������׷��� ũ�� ���� MAX=255�� ����ȭ�Ͽ� ���
	//������׷��� ũ�� : 256*256 ����

	int i, j, value;
	CvScalar temp;

	double HIST[HISTOGRAM_SIZE];
	unsigned char scale_HIST[HISTOGRAM_SIZE];
	double MAX, MIN, DIF;

	IplImage *histogramImage = cvCreateImage(cvSize(HISTOGRAM_SIZE, HISTOGRAM_SIZE+20), IPL_DEPTH_8U, 1);
	IplImage *compHistogramImage = cvCreateImage(cvSize(HISTOGRAM_SIZE, HISTOGRAM_SIZE+20), IPL_DEPTH_8U, 1);

	//�ʱ�ȭ
	for(i=0; i<HISTOGRAM_SIZE; i++){
		HIST[i] = LOW;
	}

	//�� �� ����
	for(i=0; i<image->height; i++) {
		for(j=0; j<image->width; j++){
			temp = cvGet2D(image, i, j);
			value = (int)temp.val[0];
			HIST[value]++;
		}
	}

	//����ȭ

	MAX = HIST[0]; //�ִ밪�� ã�� ���� �ʱⰪ
	MIN = HIST[0]; //�ּҰ��� ã�� ���� �ʱⰪ

	for(i=0; i<HISTOGRAM_SIZE; i++) {
		//�Է� ������ �ִ� ������ ã��
		if(HIST[i] > MAX){
			MAX = HIST[i];
		}

		//�Է� ������ �ּ� ������ ã��
		if(HIST[i] < MIN){
			MIN = HIST[i];
		}
	}

	DIF = MAX - MIN;

	//����ȭ�� ������׷�
	for(i=0; i<HISTOGRAM_SIZE; i++){
		scale_HIST[i] = (unsigned char)((HIST[i] - MIN) * HIGH / DIF);
	}

	//����ȭ�� ������׷� ���

	cvSet(histogramImage, cvScalar(255));

	//����ȭ�� ������׷��� ���� ��� �迭�� ���� ��(0)���� ǥ��
	for(i=0; i<HISTOGRAM_SIZE; i++){
		for(j=0; j<scale_HIST[i]; j++){
			cvSet2D(histogramImage, HISTOGRAM_SIZE-j-1, i, cvScalar(0));
		}
	}

	//������׷��� ����ϰ� �� �Ʒ� �κп� ������׷��� ���� ǥ��
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
		//�ʱ�ȭ
		for(i=0; i<HISTOGRAM_SIZE; i++){
			HIST[i] = LOW;
		}


		///////////////////////////////////////////////////////////////////////////////////
		//�� �� ����
		for(i=0; i<compareImage->height; i++) {
			for(j=0; j<compareImage->width; j++){
				temp = cvGet2D(compareImage, i, j);
				value = (int)temp.val[0];
				HIST[value]++;
			}
		}

		//����ȭ

		MAX = HIST[0]; //�ִ밪�� ã�� ���� �ʱⰪ
		MIN = HIST[0]; //�ּҰ��� ã�� ���� �ʱⰪ

		for(i=0; i<HISTOGRAM_SIZE; i++) {
			//�Է� ������ �ִ� ������ ã��
			if(HIST[i] > MAX){
				MAX = HIST[i];
			}

			//�Է� ������ �ּ� ������ ã��
			if(HIST[i] < MIN){
				MIN = HIST[i];
			}
		}

		DIF = MAX - MIN;

		//����ȭ�� ������׷�
		for(i=0; i<HISTOGRAM_SIZE; i++){
			scale_HIST[i] = (unsigned char)((HIST[i] - MIN) * HIGH / DIF);
		}

		//����ȭ�� ������׷� ���

		cvSet(compHistogramImage, cvScalar(255));

		//����ȭ�� ������׷��� ���� ��� �迭�� ���� ��(0)���� ǥ��
		for(i=0; i<HISTOGRAM_SIZE; i++){
			for(j=0; j<scale_HIST[i]; j++){
				cvSet2D(compHistogramImage, HISTOGRAM_SIZE-j-1, i, cvScalar(0));
			}
		}

		//������׷��� ����ϰ� �� �Ʒ� �κп� ������׷��� ���� ǥ��
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


