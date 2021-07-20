
// LinyuSmooth.cpp

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
	IplImage *linyusmooth = cvCreateImage(cvSize(inputimage->width, inputimage->height), IPL_DEPTH_8U, inputimage->nChannels);
	
	// ������ʼͼ��
	IplImage *outputimage;
	outputimage = inputimage;

	// ��������ƽ��
	for (int d = 1; d< 10; d++)
	{
		// ��չ��һ�к����һ�У���������һ�к����һ�У�
		for (int j = 1; j < kuozhan->width-1; j++)
		{
			for (int k = 0; k < kuozhan->nChannels; k++)
			{
				kuozhan->imageData[j*kuozhan->nChannels+k] =  inputimage->imageData[(j-1)*inputimage->nChannels+k];
				kuozhan->imageData[(kuozhan->height-1)*kuozhan->widthStep+j*kuozhan->nChannels+k] =  inputimage->imageData[(inputimage->height-1)*inputimage->widthStep+(j-1)*inputimage->nChannels+k];
			}
		}

		// �����м��ͼ��
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

		// ��չ��һ�к����һ��
		for (int i = 0; i < kuozhan->height; i++)
		{
			for (int k = 0; k < kuozhan->nChannels; k++)
			{
				kuozhan->imageData[i*kuozhan->widthStep+k] = kuozhan->imageData[i*kuozhan->widthStep+kuozhan->nChannels+k];
				kuozhan->imageData[(i+1)*kuozhan->widthStep-kuozhan->nChannels+k] = kuozhan->imageData[(i+1)*kuozhan->widthStep-kuozhan->nChannels+k];
			}
		}

		// ����ƽ��
		for (int i = 0; i < linyusmooth->height; i++)
		{
			for (int j = 0; j < linyusmooth->width; j++)
			{
				for (int k = 0; k < linyusmooth->nChannels; k++)
				{
					linyusmooth->imageData[i*linyusmooth->widthStep+j*linyusmooth->nChannels+k] = 1.0/9*((unsigned char)kuozhan->imageData[i*kuozhan->widthStep+j*kuozhan->nChannels+k]+(unsigned char)kuozhan->imageData[(i+1)*kuozhan->widthStep+j*kuozhan->nChannels+k]+(unsigned char)kuozhan->imageData[(i+2)*kuozhan->widthStep+j*kuozhan->nChannels+k]+(unsigned char)kuozhan->imageData[i*kuozhan->widthStep+(j+1)*kuozhan->nChannels+k]+(unsigned char)kuozhan->imageData[(i+1)*kuozhan->widthStep+(j+1)*kuozhan->nChannels+k]+(unsigned char)kuozhan->imageData[(i+2)*kuozhan->widthStep+(j+1)*kuozhan->nChannels+k]+(unsigned char)kuozhan->imageData[i*kuozhan->widthStep+(j+2)*kuozhan->nChannels+k]+(unsigned char)kuozhan->imageData[(i+1)*kuozhan->widthStep+(j+2)*kuozhan->nChannels+k]+(unsigned char)kuozhan->imageData[(i+2)*kuozhan->widthStep+(j+2)*kuozhan->nChannels+k]);
				}
			}
		}

		inputimage = linyusmooth;
	}

	cvNamedWindow("outputimage", 1);
	cvNamedWindow("linyusmooth", 1);

	cvShowImage("outputimage",outputimage);
	cvShowImage("linyusmooth",linyusmooth);
	cvWaitKey(0);

	cvSaveImage("JZ_MR.jpg", linyusmooth);

	cvDestroyWindow("outputimage");
	cvDestroyWindow("linyusmooth");

	cvReleaseImage(&outputimage);
	cvReleaseImage(&linyusmooth);
}
