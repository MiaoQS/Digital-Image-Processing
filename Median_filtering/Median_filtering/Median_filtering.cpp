#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include <stdio.h>

void main()
{
	//��ȡͼ��
	char* filename = "PN_MR.jpg";
	IplImage* inputimage = cvLoadImage(filename, -1);

	//����ͼ���Ե
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

	//��������ֵ�˲����д���
	IplImage* outputimage = cvCreateImage(cvSize(inputimage->width, inputimage->height), IPL_DEPTH_8U, inputimage->nChannels);
	unsigned char* Image = (unsigned char*)expandimage->imageData;
	int b[3][3];
	int a[9];
	int i, temp;

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
				int l = 0;
				for (int m = 0; m < 3; m++)
				{
					for (int n = 0; n < 3; n++)
					{
						a[l] = b[m][n];
						l = l + 1;
					}
				}
				//printf("asd\n");
				//ð�������㷨������ n-1 �ֱȽ�
				for (int h = 0; h < 10 - 1; h++)
				{
					//ÿһ�ֱȽ�ǰ n-1-i ����Ҳ����˵���Ѿ�����õ���� i �����ñȽ�
					for (int z = 0; z < 10 - 1 - h; z++)
					{
						if (a[z] > a[z + 1])
						{
							temp = a[z];
							a[z] = a[z + 1];
							a[z + 1] = temp;
						}
					}
				}
				expandimage->imageData[i * expandimage->widthStep + j * expandimage->nChannels + k] = a[4];
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

	cvSaveImage("MF_MR.jpg",outputimage);
	cvNamedWindow("inputimage", 1);
	cvNamedWindow("outputimage", 1);

	cvShowImage("inputimage", inputimage);
	cvShowImage("outputimage", outputimage);
	cvWaitKey(0);

	//cvSaveImage("MF_track.jpg",outputimage);

	cvDestroyWindow("inputimage");
	cvDestroyWindow("outputimage");

	cvReleaseImage(&inputimage);
	cvReleaseImage(&outputimage);


}