#include<stdio.h>
#include<cv.h>
#include<highgui.h>
#include<iostream>
using namespace std;
#define pi 31415926535898

void main()
{
	int yz;
	cin >> yz;

	int B, M = 100, N = 200;//ֱ��б����������
	double ct = -(pi / 2), bt = 0;//�����Ƕȱ���
	double a_[200];
	double b_[200];
	int c[100][200];//�ۼ�����
	double bb;
	double d, dmin;//�ؾ�ľ���

	char* filename = "road.bmp";
	IplImage* inputimage = cvLoadImage(filename, -1);

	//��ʼ����ά����
	for (int i = 0; i < M; i++)
	{
		for (int j = 0; j < N; j++)
			c[i][j] = 0;
	}

	for (int k = 0; k < M; k++)
	{
		ct += (1.0 / M) * pi;
		a_[k] = tan(ct);
	}

	for (int k = 0; k < N; k++)
	{
		bt += (1.0 / N) * inputimage->width;
		b_[k] = bt;
	}

	//�������ø�˹���Ӷ�����д���
	IplImage* expandimage = cvCreateImage(cvSize(inputimage->width + 2, inputimage->height + 2), IPL_DEPTH_8U, inputimage->nChannels);

	for (int i = 0; i < inputimage->height; i++)
	{
		for (int j = 0; j < inputimage->width; j++)
		{
			for (int k = 0; k < inputimage->nChannels; k++)
			{
				expandimage->imageData[(i + 1) * expandimage->widthStep + (j + 1) * expandimage->nChannels + k] = inputimage->imageData[i * inputimage->widthStep + j * inputimage->nChannels + k];
			}
		}
	}

	for (int j = 0; j < inputimage->width; j++)
	{
		for (int k = 0; k < inputimage->nChannels; k++)
		{
			expandimage->imageData[0 * expandimage->widthStep + (j + 1) * expandimage->nChannels + k] = inputimage->imageData[0 * expandimage->widthStep + j * inputimage->nChannels + k];
			expandimage->imageData[(expandimage->height - 1) * expandimage->widthStep + (j + 1) * expandimage->nChannels + k] = inputimage->imageData[(inputimage->height - 1) * inputimage->widthStep + j * inputimage->nChannels + k];
		}
	}

	for (int i = 0; i < inputimage->height; i++)
	{
		for (int k = 0; k < inputimage->nChannels; k++)
		{
			expandimage->imageData[(i + 1) * expandimage->widthStep + 0 * expandimage->nChannels + k] = inputimage->imageData[i * inputimage->widthStep + 0 * expandimage->nChannels + k];
			expandimage->imageData[(i + 1) * expandimage->widthStep + (expandimage->width - 1) * expandimage->nChannels + k] = inputimage->imageData[i * expandimage->widthStep + (inputimage->width - 1) * inputimage->nChannels + k];

		}
	}

	IplImage* outputimage = cvCreateImage(cvSize(inputimage->width, inputimage->height), IPL_DEPTH_8U, inputimage->nChannels);
	unsigned char* Image = (unsigned char*)expandimage->imageData;
	double a[3][3] = { 1.0 / 16,1.0 / 8,1.0 / 16,1.0 / 8,1.0 / 4,1.0 / 8,1.0 / 16,1.0 / 8,1.0 / 16 };
	double b[3][3];
	double S;

	for (int i = 1; i < expandimage->height - 1; i++)
	{
		for (int j = 1; j < expandimage->width - 1; j++)
		{
			for (int k = 0; k < expandimage->nChannels; k++)
			{
				for (int m = 0; m < 3; m++)
				{
					for (int n = 0; n < 3; n++)
					{
						b[m][n] = Image[(i + m - 1) * expandimage->widthStep + (j + n - 1) * expandimage->nChannels + k];
					}
				}
				S = 0;
				for (int m = 0; m < 3; m++)
				{
					for (int n = 0; n < 3; n++)
					{
						S = S + b[m][n] * a[m][n];
					}
				}
				expandimage->imageData[i * expandimage->widthStep + j * expandimage->nChannels + k] = S;
			}
		}
	}

	//���潫����õ��ĸ�������ֵ�������ͼ��
	for (int i = 0; i < outputimage->height; i++)
	{
		for (int j = 0; j < outputimage->width; j++)
		{
			for (int k = 0; k < outputimage->nChannels; k++)
			{
				outputimage->imageData[i * outputimage->widthStep + j * outputimage->nChannels + k] = expandimage->imageData[(i + 1) * expandimage->widthStep + (j + 1) * expandimage->nChannels + k];
			}
		}
	}

	//���˸�˹�任��������й���任����
	for (int i = 0; i < outputimage->height; i++)
	{
		for (int j = 0; j < outputimage->width; j++)
		{
			if ((unsigned char)outputimage->imageData[i * outputimage->widthStep + j * outputimage->nChannels] == 255)
			{
				outputimage->imageData[i * outputimage->widthStep + j * outputimage->nChannels] = 100;

				for (int m = 0; m < M; m++)
				{
					bb = -(i * a_[m]) + j;//���������bֵ
					dmin = 9999;//�����ʼ��Ϊ����
					for (int n = 0; n < N; n++)
					{
						d = fabs(b_[n] - bb);

						if (d < dmin)
						{
							dmin = d;//�����ҵ���С����
							B = n;//��¼��ʱb�ı��ΪB;
						}

					}
					if ((B != 0) && (B != N - 1))
						c[m][B] += 1;
				}
			}
		}
	}

	for (int i = 0; i < M; i++)
	{
		for (int j = 0; j < N; j++)
		{
			if (c[i][j] >= yz)
			{
				for (int nh = 0; nh < outputimage->height; nh++)
				{
					B = (int)(a_[i] * nh + b_[j]);
					if (B > 0 && B < outputimage->width)
					{
						outputimage->imageData[nh * outputimage->widthStep + B * outputimage->nChannels] = 255;
						outputimage->imageData[(nh + 1) * outputimage->widthStep + B * outputimage->nChannels] = 255;
						outputimage->imageData[nh * outputimage->widthStep + (B + 1) * outputimage->nChannels] = 255;
						outputimage->imageData[(nh + 1) * outputimage->widthStep + (B + 1) * outputimage->nChannels] = 255;
					}
				}
				for (int nh = 0; nh < outputimage->width; nh++)
				{

					B = (int)((nh - b_[j]) / a_[i]);

					if (B > 0 && B < outputimage->height)
					{
						outputimage->imageData[B * outputimage->widthStep + nh * outputimage->nChannels] = 255;
						outputimage->imageData[B * outputimage->widthStep + (nh + 1) * outputimage->nChannels] = 255;
						outputimage->imageData[(B + 1) * outputimage->widthStep + nh * outputimage->nChannels] = 255;
						outputimage->imageData[(B + 1) * outputimage->widthStep + (nh + 1) * outputimage->nChannels] = 255;
					}
				}
			}

		}
	}

	cvNamedWindow("inputimage", 1);
	cvNamedWindow("outputimage", 1);

	cvShowImage("inputimage", inputimage);
	cvShowImage("outputimage", outputimage);
	cvWaitKey(0);

	cvDestroyWindow("inputimage");
	cvDestroyWindow("outputimage");

	cvReleaseImage(&inputimage);
	cvReleaseImage(&outputimage);
}
