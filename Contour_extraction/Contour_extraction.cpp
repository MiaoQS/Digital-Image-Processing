#include <cv.h>
#include <cxcore.h>
#include <highgui.h>


void main()
{
	const char* filename = "test1.bmp";
	IplImage* inputimage = cvLoadImage(filename, -1);
	IplImage* expandimage = cvCreateImage(cvSize(inputimage->width + 2, inputimage->height + 2), IPL_DEPTH_8U, inputimage->nChannels);
	IplImage* extractimage = cvCreateImage(cvSize(inputimage->width, inputimage->height), IPL_DEPTH_8U, inputimage->nChannels);

	for (int j = 1; j < expandimage->width - 1; j++)
	{
		for (int k = 0; k < expandimage->nChannels; k++)
		{
			expandimage->imageData[j * expandimage->nChannels + k] = inputimage->imageData[(j - 1) * inputimage->nChannels + k];
			expandimage->imageData[(expandimage->height - 1) * expandimage->widthStep + j * expandimage->nChannels + k] = inputimage->imageData[(inputimage->height - 1) * inputimage->widthStep + (j - 1) * inputimage->nChannels + k];
		}
	}

	for (int i = 1; i < expandimage->height - 1; i++)
	{
		for (int j = 1; j < expandimage->width - 1; j++)
		{
			for (int k = 0; k < expandimage->nChannels; k++)
			{
				expandimage->imageData[i * expandimage->widthStep + j * expandimage->nChannels + k] = inputimage->imageData[(i - 1) * inputimage->widthStep + (j - 1) * inputimage->nChannels + k];
			}
		}
	}


	for (int i = 0; i < expandimage->height; i++)
	{
		for (int k = 0; k < expandimage->nChannels; k++)
		{
			expandimage->imageData[i * expandimage->widthStep + k] = expandimage->imageData[i * expandimage->widthStep + expandimage->nChannels + k];
			expandimage->imageData[(i + 1) * expandimage->widthStep - expandimage->nChannels + k] = expandimage->imageData[(i + 1) * expandimage->widthStep - 2 * expandimage->nChannels + k];
		}
	}

	for (int i = 0; i < extractimage->height; i++)
	{
		for (int j = 0; j < extractimage->width; j++)
		{
			extractimage->imageData[i * extractimage->widthStep + j * extractimage->nChannels + 0] = 255;
			extractimage->imageData[i * extractimage->widthStep + j * extractimage->nChannels + 1] = 255;
			extractimage->imageData[i * extractimage->widthStep + j * extractimage->nChannels + 2] = 255;
		}
	}

	for (int i = 0; i < extractimage->height; i++)
	{
		for (int j = 0; j < extractimage->width; j++)
		{
			if (expandimage->imageData[i * expandimage->widthStep + j * expandimage->nChannels + 0] == expandimage->imageData[(i + 1) * expandimage->widthStep + (j + 1) * expandimage->nChannels])
				if (expandimage->imageData[(i + 1) * expandimage->widthStep + j * expandimage->nChannels + 0] == expandimage->imageData[(i + 1) * expandimage->widthStep + (j + 1) * expandimage->nChannels])
					if (expandimage->imageData[(i + 2) * expandimage->widthStep + j * expandimage->nChannels + 0] == expandimage->imageData[(i + 1) * expandimage->widthStep + (j + 1) * expandimage->nChannels])
						if (expandimage->imageData[i * expandimage->widthStep + (j + 1) * expandimage->nChannels + 0] == expandimage->imageData[(i + 1) * expandimage->widthStep + (j + 1) * expandimage->nChannels])
							if (expandimage->imageData[(i + 2) * expandimage->widthStep + (j + 1) * expandimage->nChannels + 0] == expandimage->imageData[(i + 1) * expandimage->widthStep + (j + 1) * expandimage->nChannels])
								if (expandimage->imageData[i * expandimage->widthStep + (j + 2) * expandimage->nChannels + 0] == expandimage->imageData[(i + 1) * expandimage->widthStep + (j + 1) * expandimage->nChannels])
									if (expandimage->imageData[(i + 1) * expandimage->widthStep + (j + 2) * expandimage->nChannels + 0] == expandimage->imageData[(i + 1) * expandimage->widthStep + (j + 1) * expandimage->nChannels])
										if (expandimage->imageData[(i + 2) * expandimage->widthStep + (j + 2) * expandimage->nChannels + 0] == expandimage->imageData[(i + 1) * expandimage->widthStep + (j + 1) * expandimage->nChannels])
										{
											extractimage->imageData[i * extractimage->widthStep + j * extractimage->nChannels + 0] = 0;
											extractimage->imageData[i * extractimage->widthStep + j * extractimage->nChannels + 1] = 0;
											extractimage->imageData[i * extractimage->widthStep + j * extractimage->nChannels + 2] = 0;
										}
		}
	}

	cvNamedWindow("inputimage", 1);
	cvNamedWindow("extractimage", 1);

	cvShowImage("inputimage", inputimage);
	cvShowImage("extractimage", extractimage);
	cvWaitKey(0);

	cvDestroyWindow("inputimage");
	cvDestroyWindow("extractimage");

	cvReleaseImage(&inputimage);
	cvReleaseImage(&extractimage);
}
