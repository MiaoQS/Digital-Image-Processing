// GaosiSmooth.cpp

#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include <stdio.h>
#include <string.h>

void main()
{
	char *filename = "PN_MR.jpg";
	IplImage *inputimage = cvLoadImage(filename, -1);
	IplImage *kuozhan = cvCreateImage(cvSize(inputimage->width+2, inputimage->height+2), IPL_DEPTH_8U, inputimage->nChannels);
	IplImage *gaosismooth = cvCreateImage(cvSize(inputimage->width, inputimage->height), IPL_DEPTH_8U, inputimage->nChannels);
	//cvSaveImage（“GS_messi.bmp”,gaosismooth）;

	// 保留初始图像
	IplImage *outputimage;
	outputimage = inputimage;

	// 迭代高斯平滑
	for (int d = 1; d< 10; d++)
	{
		// 扩展第一行和最后一行（不包括第一列和最后一列）
		for (int j = 1; j < kuozhan->width-1; j++)
		{
			for (int k = 0; k < kuozhan->nChannels; k++)
			{
				kuozhan->imageData[j*kuozhan->nChannels+k] =  inputimage->imageData[(j-1)*inputimage->nChannels+k];
				kuozhan->imageData[(kuozhan->height-1)*kuozhan->widthStep+j*kuozhan->nChannels+k] =  inputimage->imageData[(inputimage->height-1)*inputimage->widthStep+(j-1)*inputimage->nChannels+k];
			}
		}

		// 复制中间的图像
		for (int i = 1; i < kuozhan->height-1; i++)
		{
			for (int j = 1; j < kuozhan->width-1; j++)
			{
				for (int k = 0; k < kuozhan->nChannels; k++)
				{
					kuozhan->imageData[i*kuozhan->widthStep+j*kuozhan->nChannels+k] = inputimage->imageData[(i-1)*inputimage->widthStep+(j-1)*inputimage->nChannels+k];
				}
			}
		}


		// 扩展第一列和最后一列
		for (int i = 0; i < kuozhan->height; i++)
		{
			for (int k = 0; k < kuozhan->nChannels; k++)
			{
				kuozhan->imageData[i*kuozhan->widthStep+k] = kuozhan->imageData[i*kuozhan->widthStep+kuozhan->nChannels+k];
				kuozhan->imageData[(i+1)*kuozhan->widthStep-kuozhan->nChannels+k] = kuozhan->imageData[(i+1)*kuozhan->widthStep-kuozhan->nChannels+k];
			}
		}

		// 高斯平滑
		for (int i = 0; i < gaosismooth->height; i++)
		{
			for (int j = 0; j < gaosismooth->width; j++)
			{
				for (int k = 0; k < gaosismooth->nChannels; k++)
				{
					gaosismooth->imageData[i*gaosismooth->widthStep+j*gaosismooth->nChannels+k] = 1.0/16*((unsigned char)kuozhan->imageData[i*kuozhan->widthStep+j*kuozhan->nChannels+k]+2*(unsigned char)kuozhan->imageData[(i+1)*kuozhan->widthStep+j*kuozhan->nChannels+k]+(unsigned char)kuozhan->imageData[(i+2)*kuozhan->widthStep+j*kuozhan->nChannels+k]+2*(unsigned char)kuozhan->imageData[i*kuozhan->widthStep+(j+1)*kuozhan->nChannels+k]+4*(unsigned char)kuozhan->imageData[(i+1)*kuozhan->widthStep+(j+1)*kuozhan->nChannels+k]+2*(unsigned char)kuozhan->imageData[(i+2)*kuozhan->widthStep+(j+1)*kuozhan->nChannels+k]+(unsigned char)kuozhan->imageData[i*kuozhan->widthStep+(j+2)*kuozhan->nChannels+k]+2*(unsigned char)kuozhan->imageData[(i+1)*kuozhan->widthStep+(j+2)*kuozhan->nChannels+k]+(unsigned char)kuozhan->imageData[(i+2)*kuozhan->widthStep+(j+2)*kuozhan->nChannels+k]);
				}
			}
		}

		inputimage = gaosismooth;
	}

	cvSaveImage("GS_MR.jpg",gaosismooth);
	cvNamedWindow("outputimage", 1);
	cvNamedWindow("gaosismooth", 1);

	cvShowImage("outputimage",outputimage);
	cvShowImage("gaosismooth",gaosismooth);
	cvWaitKey(0);

	//cvSaveImage("GS_FF.bmp",gaosismooth);

	cvDestroyWindow("outputimage");
	cvDestroyWindow("gaosismooth");

	cvReleaseImage(&outputimage);
	cvReleaseImage(&gaosismooth);
}
