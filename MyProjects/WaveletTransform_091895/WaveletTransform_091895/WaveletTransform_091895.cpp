// WaveletTransformProject_091895.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//
#include "stdafx.h"

#include <stdio.h>



#include <stdlib.h>
#include "math.h"

#include <cv.h>
#include <cxcore.h>
#include <highgui.h>



double* OnConvolution(double* m_Target, double* m_Filter, int size, int  mode);
double* OnDownSampling(double* m_Target, int size);
void OnFilterTapGen();
void OnFilterGen(double* m_H0, double* m_H1, double* m_G0, double* m_G1);
double** OnMem2DAllocDouble(int height, int width);
unsigned char ** OnMem2DAllocUnsigned(int height, int width);
double** OnScale(double** m_Target, int height, int width);
void OnWaveletEncode();
//����ȯ
double* OnUpSampling(double* m_Target, int size);
void OnSNR();
void OnWaveletDecode();

//����

int m_Size, m_Width = 512, m_Height = 512;
unsigned char *m_InputImage, *m_OutputImage, **m_ArrangeImage;
double *m_FilterH0, *m_FilterH1, *m_FilterG0, *m_FilterG1;

double **m_tempInput;
double **m_tempOutput;
int m_Level=3;//chc
int m_FilterTap =2; //chc

//����ȯ����
double *m_Recon;

int _tmain(int argc, _TCHAR* argv[])
{
	OnWaveletEncode();
	OnWaveletDecode();


	return 0;
}




void OnWaveletEncode()
{



	m_Size = m_Height * m_Width;

	m_InputImage = new unsigned char [m_Size]; // �Է� ���� ���� ����
	m_OutputImage = new unsigned char [m_Size]; // ��� ���� ���� ����


	for(int i = 0 ; i<m_Size ; i++){
		m_InputImage[i] = 255; // �ʱ�ȭ
		m_OutputImage[i] = 255; // �ʱ�ȭ
	}

	//chc
	//CvScalar temp;
	CvScalar temp_value;
	IplImage *inputImage = cvLoadImage("lena.bmp", CV_LOAD_IMAGE_GRAYSCALE);
	IplImage *outputImage = cvCreateImage(cvGetSize(inputImage), 8,0);

	// �ִ� ���� ���� 512*512 ��� 6������ ����
	/*
	if(m_Level <= 0 || (pow(2, m_Level+3) > (double)m_Width) || (pow(2, m_Level+3) > (double)m_Height)){
	AfxMessageBox("Not support decomposition level");
	return;
	}
	*/

	int i, j, k, width, height;
	double *m_Conv1, *m_Conv2, *m_Conv3, *m_Conv4; // Convolution�� ���� ����
	double *m_Down1, *m_Down2, *m_Down3, *m_Down4; // �ٿ� ���ø��� ���� ����
	double *m_Hor, *m_Ver1, *m_Ver2;
	double **m_L, **m_H, **m_LL, **m_LH, **m_HL, **m_HH, **m_SLL, **m_SLH, **m_SHL, **m_SHH;


	m_tempInput = OnMem2DAllocDouble(m_Height, m_Width);
	m_tempOutput = OnMem2DAllocDouble(m_Height, m_Width);
	m_ArrangeImage = OnMem2DAllocUnsigned(m_Height, m_Width);


	/* chc
	for(i=0 ; i<m_Height ; i++){
	for(j=0 ; j<m_Width ; j++){
	m_tempInput[i][j] = (double)m_InputImage[i*m_Width + j];
	// 1���� �Է��� 2���� �迭�� ��ȯ
	}
	}
	*/

	//chc  ��ǲ �־��ֱ�
	for(i=0; i<inputImage->height; i++){ 
		for(j=0; j<inputImage->width; j++){
			temp_value = cvGet2D(inputImage, i, j);
			m_tempInput[i][j] = temp_value.val[0];

		}
	}

	OnFilterTapGen();

	m_FilterH0 = new double [m_FilterTap]; // ���� ����� ���� �迭
	m_FilterH1 = new double [m_FilterTap]; // ���� ����� ���� �迭
	m_FilterG0 = new double [m_FilterTap]; // ���� ����� ���� �迭
	m_FilterG1 = new double [m_FilterTap]; // ���� ����� ���� �迭

	OnFilterGen(m_FilterH0, m_FilterH1, m_FilterG0, m_FilterG1); // ���� ��� ����

	width = m_Width;
	height = m_Height;

	for(k=0 ; k<m_Level ; k++){
		m_L = OnMem2DAllocDouble(height, width/2);
		m_H = OnMem2DAllocDouble(height, width/2);
		m_LL = OnMem2DAllocDouble(height/2, width/2); // LL ������ ���� �迭
		m_LH = OnMem2DAllocDouble(height/2, width/2); // LH ������ ���� �迭
		m_HL = OnMem2DAllocDouble(height/2, width/2); // HL ������ ���� �迭
		m_HH = OnMem2DAllocDouble(height/2, width/2); // HH ������ ���� �迭

		m_Hor = new double [width]; // Ⱦ �Է��� ���� �迭

		for(i=0 ; i<height ; i++){
			for(j=0 ; j<width ; j++){
				m_Hor[j] = m_tempInput[i][j]; // �Է� �迭�� 1���� �迭�� �Ҵ�
			}

			m_Conv1 = OnConvolution(m_Hor, m_FilterH0, width, 1); // Convolution ó��
			m_Conv2 = OnConvolution(m_Hor, m_FilterH1, width, 1); // Convolution ó��
			m_Down1 = OnDownSampling(m_Conv1, width); // �ٿ� ���ø�
			m_Down2 = OnDownSampling(m_Conv2, width); // �ٿ� ���ø�

			for(j=0 ; j<width/2 ; j++){ // �ٿ� ���ø� ����� ����
				m_L[i][j] = m_Down1[j];
				m_H[i][j] = m_Down2[j];
			}
		}

		m_Ver1 = new double[height];
		m_Ver2 = new double[height];

		for(i=0 ; i<width/2 ; i++){
			for(j=0 ; j<height ; j++){
				m_Ver1[j] = m_L[j][i]; // �� �������� 1���� �迭�� �Ҵ�
				m_Ver2[j] = m_H[j][i];
			}

			m_Conv1 = OnConvolution(m_Ver1, m_FilterH0, height, 1); // Convolution ó��
			m_Conv2 = OnConvolution(m_Ver1, m_FilterH1, height, 1);
			m_Conv3 = OnConvolution(m_Ver2, m_FilterH0, height, 1);
			m_Conv4 = OnConvolution(m_Ver2, m_FilterH1, height, 1);

			m_Down1 = OnDownSampling(m_Conv1, height); // �ٿ� ���ø�
			m_Down2 = OnDownSampling(m_Conv2, height);
			m_Down3 = OnDownSampling(m_Conv3, height);
			m_Down4 = OnDownSampling(m_Conv4, height);	

			for(j=0 ; j<height/2 ; j++){
				m_LL[j][i] = m_Down1[j]; // ��� ����
				m_LH[j][i] = m_Down2[j];
				m_HL[j][i] = m_Down3[j];
				m_HH[j][i] = m_Down4[j];
			}
		}

		m_SLL = OnScale(m_LL, height/2, width/2); // ó�� ����� ����ȭ
		m_SLH = OnScale(m_LH, height/2, width/2); // ó�� ����� ����ȭ
		m_SHL = OnScale(m_HL, height/2, width/2); // ó�� ����� ����ȭ
		m_SHH = OnScale(m_HH, height/2, width/2); // ó�� ����� ����ȭ

		for(i=0 ; i<height/2 ; i++){
			for(j=0 ; j<width/2 ; j++){
				m_tempOutput[i][j] = m_LL[i][j];
				m_tempOutput[i][j+(width/2)] = m_HL[i][j];
				m_tempOutput[i+(height/2)][j] = m_LH[i][j];
				m_tempOutput[i+(height/2)][j+(width/2)] = m_HH[i][j];
				// ó�� ����� ����

				m_ArrangeImage[i][j] = (unsigned char)m_SLL[i][j];
				m_ArrangeImage[i][j+(width/2)] = (unsigned char)m_SHL[i][j];
				m_ArrangeImage[i+(height/2)][j] = (unsigned char)m_SLH[i][j];
				m_ArrangeImage[i+(height/2)][j+(width/2)] = (unsigned char)m_SHH[i][j];
				// �ð������� ���� ���ϰ� �ϱ� ���� ���� ������ 
				// ����ȭ ������ ��ģ��

			}
		}

		width = width / 2; // ���ظ� ����ϱ� ���� ������ ������ ũ�⸦ ������ ����
		height = height / 2; // ���ظ� ����ϱ� ���� ������ ������ ũ�⸦ ������ ����	 




		m_tempInput = OnMem2DAllocDouble(height, width);

		for(i=0 ; i<height ; i++){
			for(j=0 ; j<width ; j++){
				m_tempInput[i][j] = m_LL[i][j]; // LL ���� ���ο� �Է����� �Ҵ�
			}
		}
	}

	for(i=0; i<inputImage->height; i++){ 
		for(j=0; j<inputImage->width; j++){
			temp_value.val[0] = m_ArrangeImage[i][j];
			cvSet2D(outputImage, i, j,temp_value);
		}
	}




	//chc ��� �� �޸�����

	cvNamedWindow("��ȣö_Input Image", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("��ȣö_WaveletTransform Image", CV_WINDOW_AUTOSIZE);

	cvShowImage("��ȣö_Input Image", inputImage);
	cvShowImage("��ȣö_WaveletTransform Image", outputImage);
	//cvWaitKey();

	//cvDestroyAllWindows();
	cvReleaseImage(&inputImage);
	cvReleaseImage(&outputImage);



	// �޸� ����

	delete [] m_Conv1, m_Conv2, m_Conv3, m_Conv4;
	delete [] m_Down1, m_Down2, m_Down3, m_Down4; 
	delete [] m_Hor, m_Ver1, m_Ver2;

	for(i=0 ; i<height ; i++){
		delete[] m_LL[i];		delete[] m_LH[i];		delete[] m_HL[i];		delete[] m_HH[i];		
		delete[] m_SLL[i];		delete[] m_SLH[i];		delete[] m_SHL[i];		delete[] m_SHH[i];
		delete[] m_L[i];           delete[] m_H[i];
	}
	delete m_L, m_H, m_LL, m_LH, m_HL, m_HH, m_SLL, m_SLH, m_SHL, m_SHH;

	//UpdateAllViews(NULL); chc



}









double* OnDownSampling(double* m_Target, int size)
{
	int i;
	double* m_temp;

	m_temp = new double [size/2];

	for(i=0; i<size/2 ; i++)
		m_temp[i] = m_Target[2*i]; //�ٿ� ���ø� ó��

	return m_temp;
}

double* OnConvolution(double *m_Target, double *m_Filter, int size, int mode)
{
	// Circular convolution�� ���� �Լ�
	int i, j;
	double *m_temp, *m_tempConv;
	double m_sum = 0.0;

	m_temp = new double [size + m_FilterTap - 1];
	m_tempConv = new double [size];// Convolution ��� ��� �迭

	switch(mode){
	case 1 : // Circular Convolution�� ���� �ʱ�ȭ
		for(i=0 ; i<size ; i++)
			m_temp[i] = m_Target[i];

		for(i=0 ; i<m_FilterTap-1 ; i++)	
			m_temp[size + i] = m_Target[i];

		break;

	case 2 :
		for(i=0 ; i<m_FilterTap-1 ; i++)
			m_temp[i] = m_Target[size - m_FilterTap + i + 1];

		for(i=m_FilterTap-1 ; i<size + m_FilterTap-1 ; i++)	
			m_temp[i] = m_Target[i - m_FilterTap + 1];

		break;
	}

	for(i=0 ; i<size ; i++){
		for(j=0 ; j<m_FilterTap ; j++){
			// Convolution ����
			m_sum += (m_temp[j+i] * m_Filter[m_FilterTap-j-1]);
		}

		m_tempConv[i] = m_sum;
		m_sum = 0.0;
	}

	return m_tempConv; // ���� ����� return
}


void OnFilterTapGen()
{
	/*chc
	// Filter Tap ����
	switch(pDlg->m_FilterCheck)
	{
	case 0 : m_FilterTap = 2;
	break;
	case 1 : m_FilterTap = 4;
	break;
	case 2 : m_FilterTap = 6;
	break;
	case 3 : m_FilterTap = 8;
	break;
	default : 
	//AfxMessageBox("Wrong Filter Tap");
	return;
	}
	*/
}



void OnFilterGen(double *m_H0, double *m_H1, double *m_G0, double *m_G1)
{
	// ���� �����
	int i;
	switch(m_FilterTap)
	{
	case 2 : 
		m_H0[0] = 0.70710678118655;
		m_H0[1] = 0.70710678118655;
		break;
	case 4 :
		m_H0[0] = -0.12940952255092;
		m_H0[1] = 0.22414386804186;
		m_H0[2] = 0.83651630373747;
		m_H0[3] = 0.48296291314469;
		break;
	case 6 :
		m_H0[0] = 0.03522629188210;  
		m_H0[1] = -0.08544127388224;
		m_H0[2] = -0.13501102001039;   
		m_H0[3] = 0.45987750211933;   
		m_H0[4] = 0.80689150931334;   
		m_H0[5] = 0.33267055295096;
		break;
	case 8 :
		m_H0[0] = -0.01059740178500;   
		m_H0[1] = 0.03288301166698;   
		m_H0[2] = 0.03084138183599;  
		m_H0[3] = -0.18703481171888;  
		m_H0[4] = -0.02798376941698;   
		m_H0[5] = 0.63088076792959;
		m_H0[6] = 0.71484657055254;   
		m_H0[7] = 0.23037781330886;
		break;
	default :
		//	AfxMessageBox("Wrong Filter");
		return;
	}

	// H0 ���� ����� �̿���, H1, G0, G1 ���� ��� ����
	for(i=0 ; i<m_FilterTap ; i++)
		m_H1[i] = pow(-1.0,i+1) * m_H0[m_FilterTap - i - 1];

	for(i=0 ; i<m_FilterTap ; i++)
		m_G0[i] = m_H0[m_FilterTap - i - 1];

	for(i=0 ; i<m_FilterTap ; i++)
		m_G1[i] = pow(-1.0, i) * m_H0[i];


}


unsigned char** OnMem2DAllocUnsigned(int height, int width)
{
	// unsigned char ������ 2���� �迭 �Ҵ�
	int i, j;
	unsigned char** temp;

	temp = new unsigned char *[height];

	for(i=0 ; i<height ; i++) // 2���� �迭 �Ҵ�
		temp[i] = new unsigned char [width];

	for(i=0 ; i<height ; i++){
		for(j=0 ; j<width ; j++){
			temp[i][j] = 0; // 2���� �迭 �ʱ�ȭ
		}
	}
	return temp;


}


double** OnMem2DAllocDouble(int height, int width)
{
	// double ������ 2���� �迭 �Ҵ�
	int i, j;
	double** temp;

	temp = new double *[height];

	for(i=0 ; i<height ; i++)
		temp[i] = new double [width];

	for(i=0 ; i<height ; i++){
		for(j=0 ; j<width ; j++){
			temp[i][j] = 0;
		}
	}
	return temp;



}



double** OnScale(double **m_Target, int height, int width)
{
	// ����ȭ �Լ� : ���͸� �� ���� 0~ 255 ������ ������ ����ȭ
	int i, j;
	double min, max;
	double **temp;

	temp = OnMem2DAllocDouble(height, width);

	min = max = m_Target[0][0];

	for(i=0 ; i<height ; i++){
		for(j=0 ; j<width ; j++){
			if(m_Target[i][j] <= min){ // �ּҰ�
				min = m_Target[i][j];
			}

			if(m_Target[i][j] >= max){ // �ִ밪
				max = m_Target[i][j];
			}
		}
	}

	max = max - min;

	for(i=0 ; i<height ; i++){
		for(j=0 ; j<width ; j++){
			temp[i][j] = (m_Target[i][j] - min) * (255. / max); // ����ȭ ó��
		}
	}
	return temp;

}



//����ȯ�Լ�

double* OnUpSampling(double *m_Target, int size)
{
	// �� ���ø��� ���� �Լ�
	int i;
	double* m_temp;

	m_temp = new double[size * 2];

	for(i=0 ; i<size*2 ; i++)
		m_temp[i] = 0.0; //�ʱ�ȭ

	for(i=0 ; i<size ; i++)
		m_temp[2*i] = m_Target[i]; // �����ø� ó��

	return m_temp;

}


void OnSNR()
{
	double OrgSum, ErrSum, MeanErr, MeanOrg;
	int i;

	OrgSum = 0.0;
	ErrSum = 0.0;

	// calculate mean squared error
	for(i=0 ; i<m_Size ; i++){
		// ������ ������ ���
		ErrSum += ((double)m_InputImage[i] - m_Recon[i]) * ((double)m_InputImage[i] - m_Recon[i]);
	}
	MeanErr = ErrSum / m_Size; // ���� ������ ���


	for(i=0 ; i<m_Size ; i++){
		// ��ȣ�� ������ ���
		OrgSum += ((double)m_InputImage[i]) * ((double)m_InputImage[i]);
	}
	MeanOrg = OrgSum / m_Size; // ��ȣ ������ ���

	//pDlg->m_Error = (float)MeanErr; // ���� ��� chc
	//pDlg->m_SNR = (float)(10 * (double)log10(MeanOrg / MeanErr)); // ��ȣ�� ������ ��� chc
}



void OnWaveletDecode()
{

	//chc
	CvScalar temp_value;
	IplImage *inputImage = cvLoadImage("lena.bmp", CV_LOAD_IMAGE_GRAYSCALE);
	IplImage *outputImage = cvCreateImage(cvGetSize(inputImage), 8, 0);


	int i, j, k;
	int width, height;
	double *tempLL, *tempLH, *tempHL, *tempHH, *tempL, *tempH;
	double **L, **H;
	double *Up1, *Up2, *Up3, *Up4;
	double *Conv1, *Conv2, *Conv3, *Conv4;
	double **R;

	width = m_Width / (int)(pow(2.0, m_Level));
	height = m_Height / (int)(pow(2.0, m_Level));

	m_Recon = new double [m_Width * m_Height];

	for(k=m_Level ; k>0 ; k--){
		if(width > m_Width || height > m_Height){ // ���� ����
			return;
		}

		tempLL = new double [height];
		tempLH = new double [height];
		tempHL = new double [height];
		tempHH = new double [height];

		L = OnMem2DAllocDouble(height*2, width);
		H = OnMem2DAllocDouble(height*2, width);

		tempL = new double [width];
		tempH = new double [width];

		R = OnMem2DAllocDouble(height*2, width*2);

		for(i=0 ; i<width ; i++){
			for(j=0 ; j<height ; j++){ // ���Ŀ��󿡼� ó���ϰ��� �ϴ� ���� �и�
				tempLL[j] = m_tempOutput[j][i];
				tempLH[j] = m_tempOutput[j + height][i];
				tempHL[j] = m_tempOutput[j][i + width];
				tempHH[j] = m_tempOutput[j + height][i + width];
			}

			Up1 = OnUpSampling(tempLL, height); // �� ���ø�
			Up2 = OnUpSampling(tempLH, height);
			Up3 = OnUpSampling(tempHL, height);
			Up4 = OnUpSampling(tempHH, height);

			Conv1 = OnConvolution(Up1, m_FilterG0, height*2, 2); // Convolution ����
			Conv2 = OnConvolution(Up2, m_FilterG1, height*2, 2);
			Conv3 = OnConvolution(Up3, m_FilterG0, height*2, 2);
			Conv4 = OnConvolution(Up4, m_FilterG1, height*2, 2);

			for(j=0 ; j<height*2 ; j++){
				L[j][i] = Conv1[j] + Conv2[j];
				H[j][i] = Conv3[j] + Conv4[j];
			}
		}

		for(i=0 ; i<height*2 ; i++){
			for(j=0 ; j<width ; j++){
				tempL[j] = L[i][j]; // Ⱦ ������ �и�
				tempH[j] = H[i][j];
			}


			Up1 = OnUpSampling(tempL, width); // �� ���ø�
			Up2 = OnUpSampling(tempH, width);

			Conv1 = OnConvolution(Up1, m_FilterG0, width*2, 2); //Convolution ����
			Conv2 = OnConvolution(Up2, m_FilterG1, width*2, 2);

			for(j=0 ; j<width*2 ; j++){
				R[i][j] = Conv1[j] + Conv2[j];
			}
		}

		for(i=0 ; i<height*2 ; i++){
			for(j=0 ; j<width*2 ; j++){
				m_tempOutput[i][j] = R[i][j]; // ���� �����͸� �ٽ� ����
			}
		}
		height = height * 2; // ������ ũ�⸦ �ι� Ȯ��
		width = width * 2;

	}

	for(i=0 ; i<m_Height ; i++){
		for(j=0 ; j<m_Width ; j++){
			m_Recon[i*m_Width + j] = R[i][j];
			m_OutputImage[i*m_Width + j] = (unsigned char)R[i][j]; // ���� ������ ����� ���
		}
	}



	//chc ��� �� �޸�����
	for(i=0 ; i<m_Height ; i++){
		for(j=0 ; j<m_Width ; j++){
			m_Recon[i*m_Width + j] = R[i][j];
			temp_value.val[0] = (unsigned char)R[i][j];
			cvSet2D(outputImage, i, j,temp_value); // ���� ������ ����� ���
		}
	}

	/*
	for(i=0; i<inputImage->height; i++){ 
	for(j=0; j<inputImage->width; j++){
	temp_value.val[0] = m_ArrangeImage[i][j];
	cvSet2D(outputImage, i, j,temp_value);
	}
	}
	*/


	//cvNamedWindow("Input Image", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("��ȣö_restore Image", CV_WINDOW_AUTOSIZE);

	//cvShowImage("Input Image", inputImage);
	cvShowImage("��ȣö_restore Image", outputImage);
	cvWaitKey();

	cvDestroyAllWindows();
	cvReleaseImage(&inputImage);
	cvReleaseImage(&outputImage);




	//	UpdateAllViews(NULL);chc

	// �޸� ����
	delete [] tempLL, tempLH, tempHL, tempHH, tempL, tempH;
	delete [] Up1, Up2, Up3, Up4;
	delete [] Conv1, Conv2, Conv3, Conv4;


	for(i=0 ; i<m_Height ; i++){
		delete[] L[i];
		delete[] H[i];
		delete[] R[i];
	}

	delete L, H, R;
}