#include <cv.h>
#include <stdio.h>
#include <cxcore.h>
#include <highgui.h>  
#include <math.h>



void main()
{
	IplImage *inputimage=cvLoadImage("lena.bmp",-1);
	IplImage *outputimage=cvCreateImage(cvSize(inputimage->width-2,inputimage->height-2),IPL_DEPTH_8U,1);

	double a[3][3]={{1,2,1},{2,4,2},{1,2,1}};
	double p[4]={0,0,0,0};

	for (int i=0;i<inputimage->height-2;i++)
	{
		for (int j=0;j<inputimage->width-2;j++)
		{
			for (int m=0;m<3;m++)
			{
				for (int n=0;n<3;n++)
				{
					for (int c=0;c<inputimage->nChannels;c++)
					{
						p[c]+=(unsigned char)inputimage->imageData[(i+m)*inputimage->widthStep+(j+n)*inputimage->nChannels+c]*a[m][n];
					}
				}
			}

			for (int c=0;c<inputimage->nChannels;c++)
			{
				p[c]=p[c]/16;
				inputimage->imageData[(i+1)*inputimage->widthStep+(j+1)*inputimage->nChannels+c]=(unsigned char)p[c];
				p[c]=0;
			}
		}
	}

	double b[5][5]={{-2,-4,-4,-4,-2},{-4,0,8,0,-4},{-4,8,24,8,-4},{-4,0,8,0,-4},{-2,-4,-4,-4,-2}};
	double q[4]={0,0,0,0};

	for (int i=0;i<inputimage->height-4;i++)
	{
		for (int j=0;j<inputimage->width-4;j++)
		{
			for (int m=0;m<5;m++)
			{
				for (int n=0;n<5;n++)
				{
					for (int c=0;c<inputimage->nChannels;c++)
					{
						q[c]+=(unsigned char)inputimage->imageData[(i+m)*inputimage->widthStep+(j+n)*inputimage->nChannels+c]*b[m][n];
					}
				}
			}
			outputimage->imageData[i*outputimage->widthStep+j]=(unsigned char)0;
			for (int c=0;c<inputimage->nChannels;c++)
			{
				if (abs(q[c])>500)
				{
					outputimage->imageData[i*outputimage->widthStep+j]=(unsigned char)255;
				}
				q[c]=0;
			}
		}
	}

	cvNamedWindow("inputimage",1);
	cvNamedWindow("outputimage",1);

	cvShowImage("inputimage",inputimage);
	cvShowImage("outputimage",outputimage);

	cvWaitKey(0);
}