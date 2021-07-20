#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include <cstdlib>
#include <iostream>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include<math.h>
using namespace cv;
using namespace std;

void main()
{
	// 输入图像
	const char* filename = "PN_MR.jpg";
	IplImage* inputimage = cvLoadImage(filename, -1);
	IplImage* extendimage = cvCreateImage(cvSize(inputimage->width + 2, inputimage->height + 2), IPL_DEPTH_8U, inputimage->nChannels);
	IplImage* outputimage = cvCreateImage(cvSize(inputimage->width, inputimage->height), IPL_DEPTH_8U, inputimage->nChannels);

	int i, j, k,N;
	double p[9];
	double w[9];
	double c[9];
	double pbzc=0;
	double cbzc=0;
	double sum,sum1;

	p[0] = p[2]=p[5]=p[7]=sqrt(2.0);
	p[1] = p[3] = p[4] = p[6] = 1;
	p[8] = 0;
	float aver=0;
	for (N = 0; N < 9; N++)
	{
		aver = aver + p[N];
	}
	aver = aver / 9;
	for (N = 0; N < 9; N++)
	{
		pbzc=(p[N]-aver)* (p[N] - aver)+pbzc;
	}
	pbzc = (pbzc / 9);

	// 扩展图像
	for (j = 1; j < extendimage->width - 1; j++)
	{
		for (k = 0; k < extendimage->nChannels; k++)
			extendimage->imageData[j * extendimage->nChannels + k] = inputimage->imageData[(j - 1) * inputimage->nChannels + k];
	}
	for (j = 1; j < extendimage->width - 1; j++)
	{
		for (k = 0; k < extendimage->nChannels; k++)
			extendimage->imageData[(extendimage->height - 1) * extendimage->widthStep + j * extendimage->nChannels + k] = inputimage->imageData[(inputimage->height - 1) * inputimage->widthStep + (j - 1) * inputimage->nChannels + k];
	}
	for (i = 1; i < extendimage->height - 1; i++)
	{
		for (k = 0; k < extendimage->nChannels; k++)
			extendimage->imageData[i * extendimage->widthStep + k] = inputimage->imageData[(i - 1) * inputimage->widthStep + k];

	}
	for (i = 1; i < extendimage->height - 1; i++)
	{
		for (k = 0; k < extendimage->nChannels; k++)
			extendimage->imageData[i * extendimage->widthStep + (extendimage->width - 1) * extendimage->nChannels + k] = inputimage->imageData[(i - 1) * inputimage->widthStep + (inputimage->width - 1) * inputimage->nChannels + k];

	}
	for (k = 0; k < extendimage->nChannels; k++)
	{
		extendimage->imageData[k] = inputimage->imageData[k];
		extendimage->imageData[(extendimage->width - 1) * extendimage->nChannels + k] = inputimage->imageData[(inputimage->width - 1) * inputimage->nChannels + k];
		extendimage->imageData[(extendimage->height - 1) * extendimage->widthStep + k] = inputimage->imageData[(inputimage->height - 1) * inputimage->widthStep + k];
		extendimage->imageData[(extendimage->height - 1) * extendimage->widthStep + (extendimage->width - 1) * extendimage->nChannels + k] = inputimage->imageData[(inputimage->height - 1) * inputimage->widthStep + (inputimage->width - 1) * inputimage->nChannels + k];
	}
	for (i = 1; i < extendimage->height - 1; i++)
	{
		for (j = 1; j < extendimage->width - 1; j++)
		{
			for (k = 0; k < extendimage->nChannels; k++)
				extendimage->imageData[i * extendimage->widthStep + j * extendimage->nChannels + k] = inputimage->imageData[(i - 1) * inputimage->widthStep + (j - 1) * inputimage->nChannels + k];
		}
	}
	//迭代

	int  M ;
	for (M = 0; M < 20; M++)
	{
		//双边滤波
		for (i = 0; i < outputimage->height; i++)
		{
			for (j = 0; j < outputimage->width; j++)
			{
				sum = 0; sum1 = 0; cbzc = 0;
				for (N = 0; N < 9; N++)
				{
					c[N] = 0;
				}

				for (k = 0; k < outputimage->nChannels; k++)
				{
					c[0] = c[0] + ((unsigned char)extendimage->imageData[i * extendimage->widthStep + j * extendimage->nChannels + k] - (unsigned char)extendimage->imageData[(i + 1) * extendimage->widthStep + (j + 1) * extendimage->nChannels + k]) * ((unsigned char)extendimage->imageData[i * extendimage->widthStep + j * extendimage->nChannels + k] - (unsigned char)extendimage->imageData[(i + 1) * extendimage->widthStep + (j + 1) * extendimage->nChannels + k]);
					c[1] = c[1] + ((unsigned char)extendimage->imageData[i * extendimage->widthStep + (j + 1) * extendimage->nChannels + k] - (unsigned char)extendimage->imageData[(i + 1) * extendimage->widthStep + (j + 1) * extendimage->nChannels + k]) * ((unsigned char)extendimage->imageData[i * extendimage->widthStep + (j + 1) * extendimage->nChannels + k] - (unsigned char)extendimage->imageData[(i + 1) * extendimage->widthStep + (j + 1) * extendimage->nChannels + k]);
					c[2] = c[2] + ((unsigned char)extendimage->imageData[i * extendimage->widthStep + (j + 2) * extendimage->nChannels + k] - (unsigned char)extendimage->imageData[(i + 1) * extendimage->widthStep + (j + 1) * extendimage->nChannels + k]) * ((unsigned char)extendimage->imageData[i * extendimage->widthStep + (j + 2) * extendimage->nChannels + k] - (unsigned char)extendimage->imageData[(i + 1) * extendimage->widthStep + (j + 1) * extendimage->nChannels + k]);
					c[3] = c[3] + ((unsigned char)extendimage->imageData[(i + 1) * extendimage->widthStep + (j)*extendimage->nChannels + k] - (unsigned char)extendimage->imageData[(i + 1) * extendimage->widthStep + (j + 1) * extendimage->nChannels + k]) * ((unsigned char)extendimage->imageData[(i + 1) * extendimage->widthStep + (j)*extendimage->nChannels + k] - (unsigned char)extendimage->imageData[(i + 1) * extendimage->widthStep + (j + 1) * extendimage->nChannels + k]);
					c[4] = c[4] + ((unsigned char)extendimage->imageData[(i + 1) * extendimage->widthStep + (j + 2) * extendimage->nChannels + k] - (unsigned char)extendimage->imageData[(i + 1) * extendimage->widthStep + (j + 1) * extendimage->nChannels + k]) * ((unsigned char)extendimage->imageData[(i + 1) * extendimage->widthStep + (j + 2) * extendimage->nChannels + k] - (unsigned char)extendimage->imageData[(i + 1) * extendimage->widthStep + (j + 1) * extendimage->nChannels + k]);
					c[5] = c[5] + ((unsigned char)extendimage->imageData[(i + 2) * extendimage->widthStep + (j)*extendimage->nChannels + k] - (unsigned char)extendimage->imageData[(i + 1) * extendimage->widthStep + (j + 1) * extendimage->nChannels + k]) * ((unsigned char)extendimage->imageData[(i + 2) * extendimage->widthStep + (j)*extendimage->nChannels + k] - (unsigned char)extendimage->imageData[(i + 1) * extendimage->widthStep + (j + 1) * extendimage->nChannels + k]);
					c[6] = c[6] + ((unsigned char)extendimage->imageData[(i + 2) * extendimage->widthStep + (j + 1) * extendimage->nChannels + k] - (unsigned char)extendimage->imageData[(i + 1) * extendimage->widthStep + (j + 1) * extendimage->nChannels + k]) * ((unsigned char)extendimage->imageData[(i + 2) * extendimage->widthStep + (j + 1) * extendimage->nChannels + k] - (unsigned char)extendimage->imageData[(i + 1) * extendimage->widthStep + (j + 1) * extendimage->nChannels + k]);
					c[7] = c[7] + ((unsigned char)extendimage->imageData[(i + 2) * extendimage->widthStep + (j + 2) * extendimage->nChannels + k] - (unsigned char)extendimage->imageData[(i + 1) * extendimage->widthStep + (j + 1) * extendimage->nChannels + k]) * ((unsigned char)extendimage->imageData[(i + 2) * extendimage->widthStep + (j + 2) * extendimage->nChannels + k] - (unsigned char)extendimage->imageData[(i + 1) * extendimage->widthStep + (j + 1) * extendimage->nChannels + k]);
				}
				for (N = 0; N < 9; N++)
				{
					c[N] = sqrt(c[N]);
					
				}
				aver = 0;
				for (N = 0; N < 9; N++)
				{
					aver = aver + c[N];
				}
				aver = aver / 9;
				for (N = 0; N < 9; N++)
				{
					cbzc = (c[N] - aver) * (c[N] - aver) + cbzc;
				}
				cbzc = (cbzc / 9);
				if(cbzc<0.001)
				{
					cbzc = 0.001;
				}
				for (N = 0; N < 9; N++)
				{
					w[N] = exp(-p[N] * p[N] / 2 / pbzc-c[N] * c[N] / 2 / cbzc);//修改
					
				}
				for (N = 0; N < 9; N++)
				{
					sum = sum + w[N];
				}
				for (N = 0; N < 9; N++)
				{
					w[N] = w[N] / sum;
				}

				for (k = 0; k < outputimage->nChannels; k++)
				{
					sum1 = 0;
					c[0] = w[0]*(unsigned char)extendimage->imageData[i * extendimage->widthStep + j * extendimage->nChannels + k] ;
					c[1] = w[1]*(unsigned char)extendimage->imageData[i * extendimage->widthStep + (j + 1) * extendimage->nChannels + k] ;
					c[2] =  w[2]*(unsigned char)extendimage->imageData[i * extendimage->widthStep + (j + 2) * extendimage->nChannels + k] ;
					c[3] = w[3]*(unsigned char)extendimage->imageData[(i + 1) * extendimage->widthStep + (j)*extendimage->nChannels + k] ;
					c[4] = w[4]*(unsigned char)extendimage->imageData[(i + 1) * extendimage->widthStep + (j + 2) * extendimage->nChannels + k] ;
					c[5] = w[5]*(unsigned char)extendimage->imageData[(i + 2) * extendimage->widthStep + (j)*extendimage->nChannels + k] ;
					c[6] =w[6]* (unsigned char)extendimage->imageData[(i + 2) * extendimage->widthStep + (j + 1) * extendimage->nChannels + k]  ;
					c[7] =w[7]* (unsigned char)extendimage->imageData[(i + 2) * extendimage->widthStep + (j + 2) * extendimage->nChannels + k] ;
					c[8] = w[8] * (unsigned char)extendimage->imageData[(i + 1) * extendimage->widthStep + (j + 1) * extendimage->nChannels + k];
					for (N = 0; N < 9; N++)
					{
						sum1 = sum1 + c[N];
					}
					outputimage->imageData[(i)*outputimage->widthStep + (j)*outputimage->nChannels + k] = sum1 ;
				}
			}
		}
		// 第二次扩展图像
		for (j = 1; j < extendimage->width - 1; j++)
		{
			for (k = 0; k < extendimage->nChannels; k++)
				extendimage->imageData[j * extendimage->nChannels + k] = outputimage->imageData[(j - 1) * outputimage->nChannels + k];
		}
		for (j = 1; j < extendimage->width - 1; j++)
		{
			for (k = 0; k < extendimage->nChannels; k++)
				extendimage->imageData[(extendimage->height - 1) * extendimage->widthStep + j * extendimage->nChannels + k] = outputimage->imageData[(outputimage->height - 1) * outputimage->widthStep + (j - 1) * outputimage->nChannels + k];
		}
		for (i = 1; i < extendimage->height - 1; i++)
		{
			for (k = 0; k < extendimage->nChannels; k++)
				extendimage->imageData[i * extendimage->widthStep + k] = outputimage->imageData[(i - 1) * outputimage->widthStep + k];

		}
		for (i = 1; i < extendimage->height - 1; i++)
		{
			for (k = 0; k < extendimage->nChannels; k++)
				extendimage->imageData[i * extendimage->widthStep + (extendimage->width - 1) * extendimage->nChannels + k] = outputimage->imageData[(i - 1) * outputimage->widthStep + (inputimage->width - 1) * outputimage->nChannels + k];

		}
		for (k = 0; k < extendimage->nChannels; k++)
		{
			extendimage->imageData[k] = outputimage->imageData[k];
			extendimage->imageData[(extendimage->width - 1) * extendimage->nChannels + k] = outputimage->imageData[(outputimage->width - 1) * outputimage->nChannels + k];
			extendimage->imageData[(extendimage->height - 1) * extendimage->widthStep + k] = outputimage->imageData[(outputimage->height - 1) * outputimage->widthStep + k];
			extendimage->imageData[(extendimage->height - 1) * extendimage->widthStep + (extendimage->width - 1) * extendimage->nChannels + k] = outputimage->imageData[(outputimage->height - 1) * outputimage->widthStep + (outputimage->width - 1) * outputimage->nChannels + k];
		}
		for (i = 1; i < extendimage->height - 1; i++)
		{
			for (j = 1; j < extendimage->width - 1; j++)
			{
				for (k = 0; k < extendimage->nChannels; k++)
					extendimage->imageData[i * extendimage->widthStep + j * extendimage->nChannels + k] = outputimage->imageData[(i - 1) * outputimage->widthStep + (j - 1) * outputimage->nChannels + k];
			}
		}
	}
	

	cvSaveImage("salt_pepper_图片6做了20次.jpg",outputimage);
	cvNamedWindow("inputimage", 1);
	cvNamedWindow("extendimage", 1);
	cvNamedWindow("outputimage", 1);

	cvShowImage("inputimage", inputimage);
	cvShowImage("extendimage", extendimage);
	cvShowImage("outputimage", outputimage);
	cvWaitKey(0);

	cvSaveImage("shuangbianjywlf3.jpg", outputimage);

	cvDestroyWindow("inputimage");
	cvDestroyWindow("extendimage");
	cvDestroyWindow("outputimage");

	cvReleaseImage(&inputimage);
	cvReleaseImage(&extendimage);
	cvReleaseImage(&outputimage);

}

