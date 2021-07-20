#include<stdio.h>
#include<cv.h>
#include<highgui.h>
#include<cxcore.h>
#include<math.h>

void main()
{
	char *filename="messi.jpg";
	
	IplImage *inputimage=cvLoadImage(filename,-1);
	IplImage *newimage=cvCreateImage(cvSize(inputimage->width,inputimage->height),IPL_DEPTH_8U,3);
	int pixel=inputimage->imageData[0];
	int X1,X2,X3,X4;

	for(int i=0;i<inputimage->height;i++)
		{
			for(int j=0;j<inputimage->width;j++)
				{
					for(int k=0;k<inputimage->nChannels;k++)
						{
							newimage->imageData[i*inputimage->widthStep+j*inputimage->nChannels+k]=0;
						}
				}
			}

	for(int i=0;i<inputimage->height-1;i++)
		{
			for(int j=0;j<inputimage->width-1;j++)
				{
					for(int k=0;k<inputimage->nChannels;k++)
						{
							X1=inputimage->imageData[i*inputimage->widthStep+j*inputimage->nChannels+k];
							X2=inputimage->imageData[(i+1)*inputimage->widthStep+j*inputimage->nChannels+k];
							X3=inputimage->imageData[i*inputimage->widthStep+(j+1)*inputimage->nChannels+k];
							X4=inputimage->imageData[(i+1)*inputimage->widthStep+(j+1)*inputimage->nChannels+k];
							if(X2!=pixel&&X3==pixel&&X4==pixel)
								{
									pixel=X2;
									newimage->imageData[(i+1)*inputimage->widthStep+j*inputimage->nChannels+k]=200;
								}
							if(X2==pixel&&X3!=pixel&&X4==pixel)
								{
									pixel=X3;
									newimage->imageData[i*inputimage->widthStep+(j+1)*inputimage->nChannels+k]=200;
								}
							if(X2==pixel&&X3==pixel&&X4!=pixel)
								{
									pixel=X4;
									newimage->imageData[(i+1)*inputimage->widthStep+(j+1)*inputimage->nChannels+k]=200;
								}
					}
				}
	}

	cvNamedWindow("inputimage",1);
	cvNamedWindow("newimage",1);
	cvShowImage("inputimage",inputimage);
	cvShowImage("newimage",newimage);
	cvWaitKey(0);
	cvDestroyWindow("inputimage");
	cvDestroyWindow("newimage");
	cvReleaseImage(&inputimage);
	cvReleaseImage(&newimage);
}