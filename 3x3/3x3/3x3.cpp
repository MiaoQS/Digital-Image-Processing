#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include <stdio.h>

void main()
{
	//读取图像
	char* filename = "dinner.bmp";
	IplImage* inputimage = cvLoadImage(filename, -1);

	//处理图像边缘
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
	/*for (int i = 0; i<expandimage->height; i++)
	{
		for (int j = 0; j<expandimage->width; j++)
		{
			for (int k = 0; k<expandimage->nChannels; k++)
			{
				if (i==0||j==0||i==(expandimage->height-1)||j==(expandimage->width-1))
				{
					//for (int k = 0; k<expandimage->nChannels; k++)
					//{
						expandimage->imageData[i*expandimage->widthStep+j*expandimage->nChannels+k] = 0;
					//}
				}
			}
		}
	}*/




	//下面用3*3box模板对扩充图像进行运算
	IplImage* outputimage = cvCreateImage(cvSize(inputimage->width, inputimage->height), IPL_DEPTH_8U, inputimage->nChannels);
	unsigned char* Image = (unsigned char*)expandimage->imageData;
	double a[3][3] = { 1.0 / 9,1.0 / 9,1.0 / 9,1.0 / 9,1.0 / 9,1.0 / 9,1.0 / 9,1.0 / 9,1.0 / 9 };
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
