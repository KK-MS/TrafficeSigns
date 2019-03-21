// rectangledetect.cpp : Defines the entry point for the console application.
//


// The "Square Detector" program.
// It loads several images sequentially and tries to find squares in
// each image

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <iostream>
#include <math.h>
#include <string.h>

using namespace cv;
using namespace std;



static void help()
{
	cout <<
		"\nA program using pyramid scaling, Canny, contours, contour simpification and\n"
		"memory storage to find squares in a list of images\n"
		"Returns sequence of squares detected on the image.\n"
		"the sequence is stored in the specified memory storage\n"
		"Call:\n"
		"./executable parameters\n"
		"Using OpenCV version %s\n" << CV_VERSION << "\n" << endl;
}

int FindRectangleMain();

int main(int argc, char** argv)
{

	while (1)
	{
		FindRectangleMain();

		printf("Press ESC to quit. Other key to continue\n");
		int c = waitKey(0);
		if ((char)c == 27)
			break;

	}
}
