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

	int B, M = 100, N = 200;//直线斜率量化输量
	double ct = -(pi / 2), bt = 0;//声明角度变量
	double a_[200];
	double b_[200];
	int c[100][200];//累加数组
	double bb;
	double d, dmin;//截距的距离

	char* filename = "road.bmp";
	IplImage* inputimage = cvLoadImage(filename, -1);

	//初始化二维数组
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

	//下面先用高斯算子对其进行处理
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

	//下面将运算得到的各个像素值赋给输出图像
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

	//做了高斯变换后继续进行哈夫变换处理
	for (int i = 0; i < outputimage->height; i++)
	{
		for (int j = 0; j < outputimage->width; j++)
		{
			if ((unsigned char)outputimage->imageData[i * outputimage->widthStep + j * outputimage->nChannels] == 255)
			{
				outputimage->imageData[i * outputimage->widthStep + j * outputimage->nChannels] = 100;

				for (int m = 0; m < M; m++)
				{
					bb = -(i * a_[m]) + j;//计算出来的b值
					dmin = 9999;//距离初始化为大数
					for (int n = 0; n < N; n++)
					{
						d = fabs(b_[n] - bb);

						if (d < dmin)
						{
							dmin = d;//遍历找到最小距离
							B = n;//记录此时b的标号为B;
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
