
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include <stdio.h>

void main()
{
	char *filename = "lena.bmp";
	IplImage *inputimage = cvLoadImage(filename, -1);

	IplImage *copyimage = cvCreateImage(cvSize(inputimage->width, inputimage->height), IPL_DEPTH_8U, inputimage->nChannels);

	for (int i = 0; i < copyimage->height; i++)
	{
		for (int j = 0; j < copyimage->width; j++)
		{
			for (int k = 0; k < copyimage->nChannels; k++)
				copyimage->imageData[i*copyimage->widthStep+j*copyimage->nChannels+k]
				= inputimage->imageData[i*inputimage->widthStep+j*inputimage->nChannels+k];
		}
	}

	cvNamedWindow("inputimage", 1);
	cvNamedWindow("copyimage", 1);

	cvShowImage("inputimage", inputimage);
	cvShowImage("copyimage", copyimage);
	cvWaitKey(0);

	cvDestroyWindow("inputimage");
	cvDestroyWindow("copyimage");

	cvReleaseImage(&inputimage);
	cvReleaseImage(&copyimage);
}