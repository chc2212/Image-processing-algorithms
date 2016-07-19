#include "stdafx.h"

#include <cv.h>
#include <cxcore.h>
#include <highgui.h>

struct Complex {
	double Re;	// 실수를 위한 변수
	double Im;	// 허수를 위한 변수
};

Complex **FFT;	// FFT 결과 저장을 위한 포인터

IplImage* Fft2d(IplImage* inputImage);
void Fft1d(Complex *X, int N, int Log2N);
void Shuffle(Complex *X, int N, int Log2N);
void Butterfly(Complex *X, int N, int Log2N, int mode);
int ReverseBitOrder(int index, int Log2N);

IplImage* Ifft2d(IplImage* fftSpectrum);
void Ifft1d(Complex *X, int N, int Log2N);

IplImage* LpfFrequency(IplImage* inputImage);
IplImage* HpfFrequency(IplImage* inputImage);

int _tmain(int argc, _TCHAR* argv[])
{	
	IplImage* inputImage = cvLoadImage("lena.bmp", CV_LOAD_IMAGE_GRAYSCALE);
	
// 	IplImage* lpfFrequency = LpfFrequency(inputImage);
// 	IplImage* hpfFrequency = HpfFrequency(inputImage);	
// 	IplImage* lpfFrequency = LpfFrequency(inputImage);
// 	IplImage* hpfFrequency = HpfFrequency(inputImage);	
// 
// 	cvShowImage("Input Image",inputImage);
// 	cvShowImage("LPF Frequency",lpfFrequency);
// 	cvShowImage("HPF Frequency",hpfFrequency);
// 
// 	cvWaitKey();
// 	cvReleaseImage(&inputImage);	
// 	cvReleaseImage(&lpfFrequency);
// 	cvReleaseImage(&hpfFrequency);
 	
	IplImage* fftSpectrum = Fft2d(inputImage);
	IplImage* ifftImage = Ifft2d(fftSpectrum);

	cvShowImage("input", inputImage);
	cvShowImage("FFT", fftSpectrum);
	cvShowImage("IFFT", ifftImage);

	cvWaitKey();

	cvReleaseImage(&inputImage);
	cvReleaseImage(&fftSpectrum);
	cvReleaseImage(&ifftImage);
	
	return 0;
}

IplImage* Fft2d(IplImage* inputImage) 
{
	int i, j, row, col, Log2N, Num;
	Complex * Data;

	unsigned char **temp;
	double Value, Absol;

	CvScalar tempScalar;

	IplImage* fftSpectrum = cvCreateImage(cvGetSize(inputImage), 8, 1);

	Num = inputImage->width;
	Log2N = 0;

	while(Num >= 2) // 영상의 너비에 대한 계산
	{
		Num >>= 1;
		Log2N++;
	}

	IplImage* tempImage = cvCreateImage(cvGetSize(inputImage), 8, 1);	// 기억장소 할당

	Data = new Complex [inputImage->width];

	FFT = new Complex * [inputImage->height];// 주파수 영역 변환 영상을 저장하기 위한 배열
	temp = new unsigned char * [inputImage->height];

	for(i=0 ; i<inputImage->height; i++){
		FFT[i] = new Complex [inputImage->width]; 
		temp[i] = new unsigned char [inputImage->width];
	}

	for(i=0 ; i<inputImage->height ; i++){
		for(j=0 ; j<inputImage->width ; j++){			
			Data[j].Re = (double)inputImage->imageData[i*inputImage->widthStep+j]; // 입력의 한 행을 복사, 실수 성분 값은 영상의 값	
			Data[j].Im = 0.0; // 복소 성분 값은 0
		}
		Fft1d(Data, inputImage->width, Log2N); // 1차원 FFT

		for(j=0 ; j<inputImage->width ; j++){ // 결과 저장
			FFT[i][j].Re = Data[j].Re;
			FFT[i][j].Im = Data[j].Im;
		}
	}

	Num = inputImage->height; // 영상 높이에 대한 계산
	Log2N = 0;

	while(Num >= 2)
	{
		Num >>= 1;
		Log2N++;
	}

	Data = new Complex [inputImage->height];

	for(i=0 ; i<inputImage->width ; i++){
		for(j=0 ; j<inputImage->height ; j++){
			Data[j].Re = FFT[j][i].Re; // 영상의 한 열을 복사
			Data[j].Im = FFT[j][i].Im;
		}

		Fft1d(Data, inputImage->height, Log2N); //1차원 FFT        

		for(j=0 ; j<inputImage->height ; j++){
			FFT[j][i].Re = Data[j].Re;
			FFT[j][i].Im = Data[j].Im;
		}
	}

	// FFT 실행 결과를 영상으로 출력하기 위한 연산
	for(i=0 ; i<inputImage->height ; i++){
		for(j=0 ; j<inputImage->width ; j++){
			Value = sqrt((FFT[i][j].Re * FFT[i][j].Re)+(FFT[i][j].Im * FFT[i][j].Im));
			Absol = 20 * log(Value);

			if(Absol > 255.0)
				Absol = 255.0;
			if(Absol < 0.0)
				Absol = 0.0;

			cvSet2D(tempImage, i, j, cvScalar(Absol));
		}
	}

	// 셔플링 과정: 영상을 4등분하고 분할된 영상을 상하대칭 및 좌우대칭
	for(i=0 ; i<inputImage->height; i += inputImage->height / 2){
		for(int j=0 ; j<inputImage->width; j += inputImage->width / 2){
			for(row=0 ; row<inputImage->height / 2 ; row++){
				for(col=0 ; col<inputImage->width / 2 ; col++){
					tempScalar = cvGet2D(tempImage, i+row, j+col);
					temp[(inputImage->height/2-1) - row + i][(inputImage->width/2-1) - col + j] = (unsigned char)tempScalar.val[0];
				}
			}
		}
	}

	for (i = 0; i < inputImage->height; i++){
		for (j = 0; j < inputImage->width; j++){			
			cvSet2D(fftSpectrum, i, j, cvScalar(temp[i][j]));
		}
	}

	delete [] Data, **temp;
	cvReleaseImage(&tempImage);

	return fftSpectrum;
}

void Fft1d(Complex *X, int N, int Log2N)
{
	Shuffle(X, N, Log2N); // 함수 호출
	Butterfly(X, N, Log2N, 1); // 함수 호출

}

void Shuffle(Complex *X, int N, int Log2N)
{
	int i;
	Complex *temp;

	temp = new Complex [N];

	for (i=0 ; i<N ; i++) {
		temp[i].Re = X[ReverseBitOrder(i, Log2N)].Re;
		temp[i].Im = X[ReverseBitOrder(i, Log2N)].Im;
	}

	for (i=0 ; i<N ; i++) {
		X[i].Re = temp[i].Re;
		X[i].Im = temp[i].Im;
	}

	delete [] temp;

}

void Butterfly(Complex *X, int N, int Log2N, int mode)
{
	int i, j, k, m;
	int start;
	double Value;
	double PI = 3.14159265358979;

	Complex *Y,	temp;

	Y = new Complex [N/2];

	for(i=0 ; i<Log2N ; i++){
		Value = pow(2., i+1);

		if(mode == 1){
			for(j=0 ; j<(int)(Value/2) ; j++){
				Y[j].Re = cos(j*2.0*PI / Value);
				Y[j].Im = -sin(j*2.0*PI / Value);
			}
		}

		if(mode == 2){
			for(j=0 ; j<(int)(Value/2) ; j++){
				Y[j].Re = cos(j*2.0*PI / Value);
				Y[j].Im = sin(j*2.0*PI / Value);
			}
		}

		start = 0;

		for(k=0 ; k<N/(int)Value ; k++){
			for(j=start ; j<start+(int)(Value/2) ; j++){
				m = j + (int)(Value/2);
				temp.Re = Y[j-start].Re * X[m].Re - Y[j-start].Im * X[m].Im;
				temp.Im = Y[j-start].Im * X[m].Re + Y[j-start].Re * X[m].Im;

				X[m].Re = X[j].Re - temp.Re;
				X[m].Im = X[j].Im - temp.Im;


				X[j].Re = X[j].Re + temp.Re;
				X[j].Im = X[j].Im + temp.Im;
			}
			start = start + (int)Value;
		}
	}

	if(mode == 2){
		for(i=0 ; i<N ; i++){
			X[i].Re = X[i].Re / N;
			X[i].Im = X[i].Im / N;
		}
	}

	delete [] Y;
}

int ReverseBitOrder(int index, int Log2N)
{
	int i, X, Y;

	Y = 0;

	for(i=0 ; i<Log2N ; i++){
		X = (index & (1<< i)) >> i;
		Y = (Y << 1) | X;
	}

	return Y;
}

IplImage* Ifft2d(IplImage* fftSpectrum)
{
	Complex **IFFT;

	int i, j, Num, Log2N;
	Complex *Data;

	IplImage* outputImage = cvCreateImage(cvGetSize(fftSpectrum), 8, 1);

	Num = fftSpectrum->width;
	Log2N = 0;
	while(Num >= 2) // 주파수 변환된 영상의 너비 계산
	{
		Num >>= 1;
		Log2N++;
	}

	Data = new Complex [fftSpectrum->height];
	IFFT = new Complex *[fftSpectrum->height]; // 역변환된 영상을 위한 배열

	for(i=0 ; i<fftSpectrum->height ; i++){
		IFFT[i] = new Complex [fftSpectrum->width];
	}

	for(i=0 ; i< fftSpectrum->height ; i++){
		for(j=0 ; j<fftSpectrum->width ; j++){ // 한 행을 복사
			Data[j].Re = FFT[i][j].Re;
			Data[j].Im = FFT[i][j].Im;
		}

		Ifft1d(Data, fftSpectrum->width, Log2N); // 1차원 IFFT

		for(j=0 ; j<fftSpectrum->width ; j++){
			IFFT[i][j].Re = Data[j].Re; // 결과 저장
			IFFT[i][j].Im = Data[j].Im;
		}
	}

	Num = fftSpectrum->height;
	Log2N = 0;
	while(Num >= 2) // 주파수 변환된 영상의 높이 계산
	{
		Num >>= 1;
		Log2N++;
	}

	Data = new Complex [fftSpectrum->height];

	for(i=0 ; i<fftSpectrum->width ; i++){
		for(j=0 ; j<fftSpectrum->height ; j++){
			Data[j].Re = IFFT[j][i].Re; // 한 열을 복사
			Data[j].Im = IFFT[j][i].Im;
		}

		Ifft1d(Data, fftSpectrum->height, Log2N); // 1차원 IFFT       

		for(j=0 ; j<fftSpectrum->height ; j++){
			IFFT[j][i].Re = Data[j].Re; // 결과 저장
			IFFT[j][i].Im = Data[j].Im;
		}
	}

	for(i=0 ; i<fftSpectrum->width ; i++){
		for(j=0 ; j<fftSpectrum->height ; j++){			
			cvSet2D(outputImage, i, j, cvScalar((unsigned char)IFFT[i][j].Re));// 결과 출력
		}
	}

	delete [] Data;

	return outputImage;
}

void Ifft1d(Complex *X, int N, int Log2N)
{
	Shuffle(X, N, Log2N); // 함수 호출
	Butterfly(X, N, Log2N, 2); // 함수 호출
}

IplImage* LpfFrequency(IplImage* inputImage)
{
	int i, j, x, y;
	double temp, D, N;

	D = 32.0;
	N = 2.0;

	IplImage* fftSpectrum;	
	IplImage* lpfFrequency = cvCreateImage(cvGetSize(inputImage), 8, 1);	

	fftSpectrum = Fft2d(inputImage); // 주파수 변환

	// 주파수 변환된 값에서 고주파 성분 제거
	for(i=0 ; i<fftSpectrum->height ; i++) {
		for(j=0 ; j<fftSpectrum->width ; j++){
			x = i;
			y = j;

			if(x > fftSpectrum->height / 2)
				x = x - fftSpectrum->height;
			if(y > fftSpectrum->width / 2) 
				y = y - fftSpectrum->width;


			temp = 1.0 / (1.0 + pow(sqrt((double)(x * x + y * y)) / D, 2*N));   

			FFT[i][j].Re = FFT[i][j].Re * temp;
			FFT[i][j].Im = FFT[i][j].Im * temp;
		}
	}

	lpfFrequency = Ifft2d(fftSpectrum);
	
	return lpfFrequency;
}

IplImage* HpfFrequency(IplImage* inputImage)
{
	int i, j, x, y;
	double temp, D, N;

	D = 128.0;
	N = 4.0;

	IplImage* fftSpectrum;	
	IplImage* hpfFrequency = cvCreateImage(cvGetSize(inputImage), 8, 1);

	fftSpectrum = Fft2d(inputImage); // 주파수 변환

	// 주파수 변환된 값에서 저주파 성분 제거
	for(i=0 ; i<fftSpectrum->height ; i++) {
		for(j=0 ; j<fftSpectrum->width ; j++){
			x = i;
			y = j;

			if(x > fftSpectrum->height / 2)
				x = x - fftSpectrum->height;
			if(y > fftSpectrum->width / 2) 
				y = y - fftSpectrum->width;

			temp = 1.0 / (1.0 + pow(D / sqrt((double)(x * x + y * y)) , 2*N));  

			FFT[i][j].Re = FFT[i][j].Re * temp;
			FFT[i][j].Im = FFT[i][j].Im * temp;
		}
	}

	hpfFrequency =Ifft2d(fftSpectrum);

	return hpfFrequency;
}
