
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include <stdio.h>

void main()
{
	char *filename = "lena.bmp";
	IplImage *inputimage = cvLoadImage(filename, -1);

	IplImage *Labimage = cvCreateImage(cvSize(inputimage->width, inputimage->height), IPL_DEPTH_8U, 3);
	cvCvtColor(inputimage, Labimage, CV_BGR2Lab);

	IplImage *grayimage = cvCreateImage(cvSize(inputimage->width, inputimage->height), IPL_DEPTH_8U, 1);
	for (int i = 0; i < grayimage->height; i++)
	{
		for (int j = 0; j < grayimage->width; j++)
		{
			grayimage->imageData[i*grayimage->widthStep+j] = Labimage->imageData[i*Labimage->widthStep+j*3];
		}
	}

	cvNamedWindow("inputimage", 1);
	cvNamedWindow("Labimage", 1);
	cvNamedWindow("grayimage", 1);

	cvShowImage("inputimage", inputimage);
	cvShowImage("Labimage", Labimage);
	cvShowImage("grayimage", grayimage);
	cvWaitKey(0);

	cvDestroyWindow("inputimage");
	cvDestroyWindow("Labimage");
	cvDestroyWindow("grayimage");

	cvReleaseImage(&inputimage);
	cvReleaseImage(&Labimage);
	cvReleaseImage(&grayimage);
}