// 091895 ��ȣö
// visual studio 2008�� ����

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


//���� - ���ο� ������׷���ȭ�� �����Ͽ����ϴ�. �Լ� imageHistogramViewer�� �������� �ѹ��� �� �̹����� ������׷��� ���̴� ���� �ѹ��� �� �̹����� ������׷��� ���̰� �����Ͽ����ϴ�.
int _tmain(int argc, _TCHAR* argv[])
{

	//���� ������ ����
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


	//���� �̹����� ���ϴ� ������׷� �̹����� baboon.bmp�� �ҷ���.
	IplImage *inputImage = cvLoadImage("lena.bmp", CV_LOAD_IMAGE_GRAYSCALE);
	IplImage *dinputImage = cvLoadImage("baboon.bmp", CV_LOAD_IMAGE_GRAYSCALE);


	//������׷���Ȱȭ , ������׷���ȭ �۾��� �̹��� ����
	IplImage *histogramEqualized = cvCreateImage(cvGetSize(inputImage), IPL_DEPTH_8U, 1);
	IplImage *m_OutputImage = cvCreateImage(cvGetSize(dinputImage), IPL_DEPTH_8U, 1);


	//�ʱ�ȭ
	for(i=0; i<HISTOGRAM_SIZE; i++) {
		HIST[i] = LOW;
	}

	for(i=0; i<HISTOGRAM_SIZE; i++) {
		DHIST[i] = LOW;
	}

	for(i=0; i<HISTOGRAM_SIZE; i++) {
		m_TABLE[i] = LOW;
	}

	//�� �� ����
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


	//���� ������׷� ����
	for(i=0; i<HISTOGRAM_SIZE; i++) {
		SUM += HIST[i];
		sum_Of_HIST[i] = SUM;

		DSUM += DHIST[i];
		dsum_Of_HIST[i] = DSUM;

	}

	//�Է� ������ ��Ȱȭ�� �������� ���

	for(i=0; i<inputImage->height; i++){
		for(j=0; j<inputImage->width; j++){
			temp = cvGet2D(inputImage, i, j);
			cvSet2D(histogramEqualized, i, j, cvScalar(sum_Of_HIST[ (int)temp.val[0] ] * HIGH/(inputImage->height*inputImage->width) ));
		}
	}
	
	
	DMIN = dsum_Of_HIST[0]; // baboon �̹����� ����������׷����� �ּ�,�ִ� �������� ����.
	DMAX = dsum_Of_HIST[255];  // ���� ������׷����� �迭�� �ε����� 0�̸� �ּ� �������̸�, 255�̸� �ִ� �������̴�.

	
	//baloon �̹����� ���� ������׷��� ����ȭ(0~255 ������ ������ ����ȭ)	
	for(i=0; i<256; i++){
		dSum_Of_ScHIST[i] = (unsigned char)((dsum_Of_HIST[i]-DMIN)*HIGH / (DMAX - DMIN));
	}
	

	//��� ���̺��� �̿��� ��ȭ
	for( ; ; ){
		for(i=dSum_Of_ScHIST[bottom] ; i <= dSum_Of_ScHIST[top] ; i++){
			m_TABLE[i] = top; //������̺� ����
		}
		top = bottom;
		bottom=bottom-1;

		if(bottom<-1)
			break;
	}

	//��Ȱȭ�� lena�̹����� ������ ������̺��� �ε����� ����Ͽ� output �̹��� ����
	for(i=0; i<inputImage->height; i++){
		for(j=0; j<inputImage->width; j++){
			DADD = cvGet2D(histogramEqualized, i, j);
			cvSet2D(m_OutputImage, i, j, cvScalar(m_TABLE[(int)DADD.val[0]]));	
		}
	}



	//�̹����� ������׷� â�� ����� ���̱�
	imageHistogramViewer(inputImage, "Input", "Input Histo" , dinputImage, "D_input", "D_Input Histo", m_OutputImage, "Result Image", "Result Histo");
	

	cvReleaseImage(&dinputImage);
	cvReleaseImage(&m_OutputImage);
	cvReleaseImage(&inputImage);
	cvReleaseImage(&histogramEqualized);

	return 0;
}



//�Լ� imageHistogramViewer

//������ ppt�� �ִ� imageHistogramViewer�Լ����� �� �̹����� ������׷��� �ѹ��� ��µǵ��� ����
void imageHistogramViewer(IplImage* image, char* srcWindowName, char* histoWindowName,
	IplImage* compareImage, char* compWindowName, char* compHistoWindowName,
	IplImage* compare2Image, char* comp2WindowName, char* comp2HistoWindowName)
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
	IplImage *comp2HistogramImage = cvCreateImage(cvSize(HISTOGRAM_SIZE, HISTOGRAM_SIZE+20), IPL_DEPTH_8U, 1);

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


	//�ι�°
	////////////////////////////////////////////////////////////
	if(compareImage != NULL && compWindowName != NULL && compHistoWindowName != NULL){
		//�ʱ�ȭ
		for(i=0; i<HISTOGRAM_SIZE; i++){
			HIST[i] = LOW;
		}

	
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

	//����°
	////////////////////////////////////////////////////////////

	if(compare2Image != NULL && comp2WindowName != NULL && comp2HistoWindowName != NULL){
		//�ʱ�ȭ
		for(i=0; i<HISTOGRAM_SIZE; i++){
			HIST[i] = LOW;
		}

	
		//�� �� ����
		for(i=0; i<compare2Image->height; i++) {
			for(j=0; j<compare2Image->width; j++){
				temp = cvGet2D(compare2Image, i, j);
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

		cvSet(comp2HistogramImage, cvScalar(255));

		//����ȭ�� ������׷��� ���� ��� �迭�� ���� ��(0)���� ǥ��
		for(i=0; i<HISTOGRAM_SIZE; i++){
			for(j=0; j<scale_HIST[i]; j++){
				cvSet2D(comp2HistogramImage, HISTOGRAM_SIZE-j-1, i, cvScalar(0));
			}
		}

		//������׷��� ����ϰ� �� �Ʒ� �κп� ������׷��� ���� ǥ��
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


