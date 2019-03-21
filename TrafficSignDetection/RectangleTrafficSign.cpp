
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <iostream>
#include <math.h>
#include <string.h>

using namespace cv;
using namespace std;

extern const char* names[];

static int threshCanny = 100, N = 5;
static const char* wndname = "Square Detection Demo";

// helper function:
// finds a cosine of angle between vectors
// from pt0->pt1 and from pt0->pt2
static double angle(Point pt1, Point pt2, Point pt0)
{
	double dx1 = pt1.x - pt0.x;
	double dy1 = pt1.y - pt0.y;
	double dx2 = pt2.x - pt0.x;
	double dy2 = pt2.y - pt0.y;
	return (dx1*dx2 + dy1 * dy2) / sqrt((dx1*dx1 + dy1 * dy1)*(dx2*dx2 + dy2 * dy2) + 1e-10);
}


// the function draws all the squares in the image
static void drawSquares(Mat& image, const vector<vector<Point> >& squares)
{

	for (size_t i = 0; i < squares.size(); i++)
	{
		const Point* p = &squares[i][0];

		int n = (int)squares[i].size();
		//dont detect the border
		if (p->x > 3 && p->y > 3)
			polylines(image, &p, &n, 1, true, Scalar(0, 0, 255), 3, LINE_AA);
	}

	imshow(wndname, image);
}

/* 
// apply Canny. Take the upper threshold from slider
// and set the lower to 0 (which forces edges merging)

// dilate canny output to remove potential
// holes between edge segments

// find contours and store them all as a list

// arcLength(): Calculates a contour perimeter or a curve length.
// Parameters:
// curve – Input vector of 2D points, stored in std::vector or Mat.
// closed – Flag indicating whether the curve is closed or not.
//
// approxPolyDP():
// Parameters:
// 1. curve – Input vector of a 2D point stored in :
// 2. approxCurve – Result of the approximation. The type should match the type of the input curve. 
// 3. epsilon – Parameter specifying the approximation accuracy. This is the maximum distance between the original curve and its approximation.
// 4. closed – If true, the approximated curve is closed(its first and last vertices are connected).Otherwise, it is not closed.

// Design: approximate contour with accuracy proportional
//         to the contour perimeter using approxPolyDP()
//         arcLength() is used to find perimeter.

// Design: i. square contours should have 4 vertices after approximation.
//        ii. relatively large area (to filter out noisy contours) and 
//       iii. be convex.
// Note: absolute value of an area is used because
// area may be positive or negative - in accordance with the
// contour orientation
// 
// find the maximum cosine of the angle between joint edges using angle()
//  j=2: approx 2, 0, 1
//  j=3: approx 3, 1, 2
//  j=4: approx 0, 2, 3
//
// if cosines of all angles are small
// (all angles are ~90 degree) then write quandrange
// vertices to resultant sequence
*/
static void GrayCannyDilateContours(const Mat& image, vector<vector<Point> >& squares)
{

  Mat	gray0;
  Mat	gray;
  double cosine;
  double maxCosine;

  vector<vector<Point> > contours;

  vector<Point> approx; // approx minimum points to represent the required line or curve

  cv::cvtColor(image, gray0, CV_BGR2GRAY); // aa added

  squares.clear();

  Canny(gray0, gray, 5, threshCanny, 5);      imshow("D2", gray);

  dilate(gray, gray, Mat(), Point(-1, -1));   imshow("D3", gray);

  findContours(gray, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);

  // test each contour
  for (size_t i = 0; i < contours.size(); i++) {
    approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true) * 0.02, true);

    if (approx.size() == 4 
        && fabs(contourArea(Mat(approx))) > 1000 
        && isContourConvex(Mat(approx))) {

      maxCosine = 0;

      for (int j = 2; j < 5; j++) {
        cosine = fabs(angle(approx[j % 4], approx[j - 2], approx[j - 1]));
        maxCosine = MAX(maxCosine, cosine);
      }

      // all angles are ~90 degree i.e. < 0.3 ?
      if (maxCosine < 0.3)
        squares.push_back(approx);
    }
  }
}

// returns sequence of squares detected on the image.
// the sequence is stored in the specified memory storage
static void FindRectangles(const Mat& image, vector<vector<Point> >& squares)
{
	squares.clear();

	//s    Mat pyr, timg, gray0(image.size(), CV_8U), gray;

	// down-scale and upscale the image to filter out the noise
	//pyrDown(image, pyr, Size(image.cols/2, image.rows/2));
	//pyrUp(pyr, timg, image.size());


	// blur will enhance edge detection
	//Mat timg(image);
	//medianBlur(image, timg, 9);
	//Mat gray0(timg.size(), CV_8U);
	Mat	gray0;
	Mat	gray;

	vector<vector<Point> > contours;

	cv::cvtColor(image, gray0, CV_BGR2GRAY); // aa added


											 //Mat structuring_element;
	Mat gray1; //threshold_image;

			   //structuring_element = getStructuringElement(MORPH_RECT, Size(7, 7));

			   //threshold(gray0, gray1, 40, 200, THRESH_BINARY);
	threshold(gray0, gray1, threshCanny, 200, THRESH_BINARY);

	imshow("D4", gray1);
	//

	// hack: use Canny instead of zero threshold level.
	// Canny helps to catch squares with gradient shading
	//if (l == 0)
	{
		//blur(gray0, gray0, Size(3, 3), Point(-1, -1));
		//medianBlur(gray0, gray0, 9); // little better result.! One extra frame

		// apply Canny. Take the upper threshold from slider

		// and set the lower to 0 (which forces edges merging)

		//Canny(gray0, gray, 5, threshCanny, 5);
		Canny(gray1, gray, 5, 100, 5);

		//adaptiveThreshold(gray0, gray, 200, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 3, 0);


		imshow("D2", gray);



		// dilate canny output to remove potential
		// holes between edge segments

		//	structuring_element = getStructuringElement(MORPH_RECT, Size(7, 7));
		//	morphologyEx(threshold_image, top_image, MORPH_TOPHAT, structuring_element);
		//	structuring_element = getStructuringElement(MORPH_RECT, Size(5, 5));
		//	dilate(top_image, out_mat, structuring_element);

		dilate(gray, gray, Mat(), Point(-1, -1));
	}

	// find contours and store them all as a list
	findContours(gray, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);

	imshow("D3", gray);

	vector<Point> approx;

	// test each contour
	for (size_t i = 0; i < contours.size(); i++)
	{
		// arcLength(): Calculates a contour perimeter or a curve length.
		// Parameters:
		// curve – Input vector of 2D points, stored in std::vector or Mat.
		// closed – Flag indicating whether the curve is closed or not.
		//
		// approxPolyDP():
		// Parameters:
		// 1. curve – Input vector of a 2D point stored in :
		// 2. approxCurve – Result of the approximation. The type should match the type of the input curve. 
		// 3. epsilon – Parameter specifying the approximation accuracy. This is the maximum distance between the original curve and its approximation.
		// 4. closed – If true, the approximated curve is closed(its first and last vertices are connected).Otherwise, it is not closed.

		// Design: approximate contour with accuracy proportional
		//         to the contour perimeter
		approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true) * 0.02, true);

		// Design: i. square contours should have 4 vertices after approximation.
		//        ii. relatively large area (to filter out noisy contours) and 
		//       iii. be convex.
		// Note: absolute value of an area is used because
		// area may be positive or negative - in accordance with the
		// contour orientation
		if (approx.size() == 4 &&
			fabs(contourArea(Mat(approx))) > 1000 &&
			isContourConvex(Mat(approx)))
		{
			double maxCosine = 0;

			for (int j = 2; j < 5; j++)
			{
				// find the maximum cosine of the angle between joint edges
				//  j=2: approx 2, 0, 1
				//  j=3: approx 3, 1, 2
				//  j=4: approx 0, 2, 3
				double cosine = fabs(angle(approx[j % 4], approx[j - 2], approx[j - 1]));
				maxCosine = MAX(maxCosine, cosine);
			}

			// if cosines of all angles are small
			// (all angles are ~90 degree) then write quandrange
			// vertices to resultant sequence
			if (maxCosine < 0.3)
				squares.push_back(approx);
		}
	}

}


int FindRectangleMain()
{
	Mat image;
	vector<vector<Point> > squares;

	int iFileNameIndex = 0;
	int iListCount = 17;
	string strFileName;

	int iMethodType = 1;

	namedWindow(wndname, WINDOW_NORMAL); resizeWindow(wndname, 600, 600);
	namedWindow("D1", WINDOW_NORMAL);    resizeWindow("D1", 600, 600);
	namedWindow("D2", WINDOW_NORMAL);    resizeWindow("D2", 600, 600);
	namedWindow("D3", WINDOW_NORMAL);    resizeWindow("D3", 600, 600);

	createTrackbar("T1", "D1", &threshCanny, 255, NULL);

	while (1) {
		Mat image;

		if (iFileNameIndex > iListCount) iFileNameIndex = 0;
		strFileName = names[iFileNameIndex];

		image = imread(strFileName, 1);
		
		if (image.empty()) 	{
			cout << "Couldn't load " << strFileName << endl;
			getchar();
			break;
		}

		switch (iMethodType) {
		case 1:
			GrayCannyDilateContours(image, squares);
			break;
		}

		// draw the detection
		drawSquares(image, squares);

		if (squares.size() > 0) {
			printf("Detected %d in %s\n", squares.size(), strFileName);
		}

		int c = waitKey(10);
		if ((char)c == 0xd) iFileNameIndex++; // Enter
		if ((char)c == 27) break;             // ESC
	}

	return 0;
}
