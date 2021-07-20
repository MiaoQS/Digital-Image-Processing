#include <cv.h>
#include <stdio.h>
#include <cxcore.h>
#include <highgui.h>  
#include <math.h>

unsigned char bj[6][3]={{1,1,100},{1,200,1},{1,1,200},{1,100,100},{1,200,100},{1,100,1}};
int num=0;

void biaoji(IplImage *&inputimage,int i,int j)
{
	if (i<0 || i>=inputimage->height || j<0 || j>=inputimage->width)
	{
		return;
	}
	if ((unsigned char)inputimage->imageData[i*inputimage->widthStep+j*inputimage->nChannels]>1)
	{
		for (int c=0;c<inputimage->nChannels;c++)
		{
			inputimage->imageData[i*inputimage->widthStep+j*inputimage->nChannels+c]=bj[num][c];
		}
	}
	else
	{
		return;
	}
	biaoji(inputimage,i+1,j);
	biaoji(inputimage,i,j+1);
	biaoji(inputimage,i,j-1);
	biaoji(inputimage,i-1,j);

}

void digui(IplImage *&inputimage)
{
	for (int i=0;i<inputimage->height;i++)
	{
		for (int j=0;j<inputimage->width;j++)
		{
			if ((unsigned char)inputimage->imageData[i*inputimage->widthStep+j*inputimage->nChannels]>1)
			{
				biaoji(inputimage,i,j);
				num=++num%6;
			}
		}
	}
}

void main()
{

	IplImage *inputimage=cvLoadImage("test2.bmp",3);

	cvNamedWindow("inputimage",1);
	cvShowImage("inputimage",inputimage);


	digui(inputimage);

	cvNamedWindow("biaoji",1);
	cvShowImage("biaoji",inputimage);

	cvWaitKey(0);
}



