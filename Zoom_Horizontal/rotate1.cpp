#include<cv.h>
#include<cxcore.h>
#include<highgui.h>
#include<stdio.h>
#include<iostream>
using namespace std;

void main()
{
	char *filename = "messi.jpg";
	IplImage *inputimage = cvLoadImage(filename,-1);
	float s;//缩放因子
	cout << "请输入缩放因子（>0）" << endl;
	cin >> s;

	//沿竖直方向扩大为原来的s倍
	IplImage *outputimage = cvCreateImage(cvSize(inputimage->width,inputimage->height*s),IPL_DEPTH_8U,inputimage->nChannels);
	unsigned char* out = (unsigned char*)inputimage->imageData;
	for (int i = 0; i<outputimage->height; i++)
	{
		for (int j = 0; j<outputimage->width; j++)
		{
			for (int k = 0; k<outputimage->nChannels; k++)
			{
				outputimage->imageData[i*outputimage->widthStep+j*outputimage->nChannels+k] = 
					inputimage->imageData[int(i/s)*inputimage->widthStep+j*inputimage->nChannels+k];
			}
		}
	}

	cvNamedWindow("inputimage",1);
	cvNamedWindow("outputimage",1);

	cvShowImage("inputimage",inputimage);
	cvShowImage("outputimage",outputimage);

	cvWaitKey(0);
	cvDestroyWindow("inputimage");
	cvDestroyWindow("outputimage");

	cvReleaseImage(&inputimage);
	cvReleaseImage(&outputimage);

}
