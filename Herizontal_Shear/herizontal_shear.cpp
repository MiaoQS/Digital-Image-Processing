#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include <stdio.h>
void main()
{
	char *filename = "messi.jpg";
	IplImage *inputimage = cvLoadImage(filename,-1);
	IplImage *outputimage = cvCreateImage(cvSize(inputimage->width,inputimage->width+inputimage->height),IPL_DEPTH_8U,inputimage->nChannels);
	
	for (int i = 0; i<inputimage->height; i++)
	{
		for (int j = 0; j<inputimage->width; j++)
		{
			for (int h = 0; h<inputimage->nChannels; h++)
			{
				outputimage->imageData[(i+j)*outputimage->widthStep+j*outputimage->nChannels+h] = inputimage->imageData[i*inputimage->widthStep+j*inputimage->nChannels+h];
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
