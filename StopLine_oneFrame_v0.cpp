#include <opencv2/opencv.hpp> // opencv
#include <iostream> // cout and cin
#include <math.h> // power, tan
#include <stdlib.h>  // absolute value
#include <algorithm> // for sort

using namespace std;

// Function for finding median
double median(vector<double> vec) {

	// get size of vector
	int vecSize = vec.size();

	// if vector is empty throw error
	if (vecSize == 0) {
		throw domain_error("median of empty vector");
	}

	// sort vector
	sort(vec.begin(), vec.end());

	// define middle and median
	int middle;
	double median;

		// if even number of elements in vec, take average of two middle values
	if (vecSize % 2 == 0) {
		// a value representing the middle of the array. If array is of size 4 this is 2
		// if it's 8 then middle is 4
		middle = vecSize/2;

		// take average of middle values, so if vector is [1, 2, 3, 4] we want average of 2 and 3
		// since we index at 0 middle will be the higher one vec[2] in the above vector is 3, and vec[1] is 2
		median = (vec[middle-1] + vec[middle]) / 2;
	}

	// odd number of values in the vector
	else {
		middle = vecSize/2; // take the middle again

		// if vector is 1 2 3 4 5, middle will be 5/2 = 2, and vec[2] = 3, the middle value
		median = vec[middle];
	}

	return median;
}
// Function for finding median
float find_mode(vector<float> vec) {

	// get size of vector
	int vecSize = vec.size();

	// if vector is empty throw error
	if (vecSize == 0) {
		throw domain_error("mode of empty vector");
	}

	// sort vector
	sort(vec.begin(), vec.end());

	// define middle and median
	double number = vec[0];
	double mode = vec[0];
	int count = 1;
	int countMode = 1;

	for (int i=1; i<vec.size(); i++)
	{
	    if (round(vec[i]) == number)
	    {
	        count++;
	    }
	    else
	    {
	        if (count > countMode)
	        {
	            countMode = count;
	            mode = number;
	        }
	        count = 1;
	        number = round(vec[i]);
	    }
	}

	return mode;
}
/*
float find_pair(vector<float> vec) {
	int dist = 100;
	int pair = 0;
	for(int i = 1; i < vec.size(); ++i){
		int dist_t = abs(vec[i] - vec[i-1]);
		if(dist_t < dist){
			dist = dist_t;
			pair = vec[i];
		}
	}
	return pair;
}
*/
int main(int argc, char** argv) {

	//---------------GET IMAGE---------------------
	 // Read the image file
	 // TODO: Change to an argument, enter path to image
	 cv::Mat image = cv::imread( argv[1] ); // read the image into mat type variable, image, if funning from cmd line, path is ../src/1.jpg

	 // Check for failure in reading the image
	 if (image.empty()) // check if it's empty
	 {
	  cout << "Could not open or find the image" << endl;
	  return -1;
	 }

	 // name of window to show original image
	 cv::String originalWindowName = "Original Image";

	 // Show the original image
	 imshow(originalWindowName, image);
	 cv::waitKey(0); // Wait for any keystroke in the window


	 //-----------------resize----------------------- by lin
	 // not yet written
	 cv::resize(image, image, cv::Size(0,0), 1920.0/image.cols, 1920.0/image.cols);

	 //--------------GRAYSCALE IMAGE-----------------
	 // Define grayscale image
	 cv::Mat imageGray;

	 // Convert image to grayscale
	 cv::cvtColor(image, imageGray, cv::COLOR_BGR2GRAY);

	 // window for grayscaled image
	 cv::String grayscaleWindowName = "Grayscaled image";

	 // Show grayscale image
	 cv::imshow(grayscaleWindowName, imageGray);
	 cv::waitKey(0); // wait for a key press


	 //---------------Erosion IMAGE------------------By THLIN
	 // Define erosion image
	 cv::Mat imageEros;

	 // Returns a structuring element of the specified size and shape for morphological operations.
	 // Not understand element Morph shape's influence on erosion
	 int erosion_size = 4;
	 cv::Mat element = getStructuringElement( cv::MORPH_CROSS, cv::Size( 2*erosion_size + 1, 2*erosion_size+1 ), cv::Point( erosion_size, erosion_size ) );

	 erode( imageGray, imageEros, element );

	 cv::imshow("erosionWindowName", imageEros);
	 cv::waitKey(0); // wait for a key press


	 //--------------GAUSSIAN SMOOTHING-----------------
	 // Use low pass filter to remove noise, removes high freq stuff like edges
	 int kernelSize = 9; // bigger kernel = more smoothing

	 // Define smoothed image
	 cv::Mat smoothedIm;
	 cv::GaussianBlur(imageEros, smoothedIm, cv::Size(kernelSize, kernelSize), 0,0);

	 // window for smoothed image
	 cv::String smoothedWindowName = "Smoothed image";

	 // Show smoothed image
	 cv::imshow(smoothedWindowName, smoothedIm);
	 cv::waitKey(0); // wait for a key press

	 //------------------Threshold----------------------- by lin
/*
	 // Sum the elements within the mask range
	 int rowmin = (int)round(image.rows*0.4);
	 int rowmax = (int)round(image.rows*0.85);
	 int colmin = (int)round(image.cols*(0.5 - 1.0/3.0));
	 int colmax = (int)round(image.cols*(0.5 + 1.0/3.0));
 	 cv::Scalar elesum = cv::sum(imageGray(cv::Range(rowmin, rowmax), cv::Range(colmin, colmax)));
*/
	 vector<double> mean, stddev;
	 cv::meanStdDev(imageGray, mean, stddev);
 	 cout << "mean: " << mean[0] << endl << "stddev: " << stddev[0] << endl;
 	 cv::Mat imageThres;
 	 cv::threshold(smoothedIm, imageThres, mean[0]+1.4*stddev[0], 0, cv::THRESH_TOZERO);
 	 cv::String thresholdWindowName = "Threshold image";
 	 cv::imshow(thresholdWindowName, imageThres);
 	 cv::waitKey(0);


	 //---------------EDGE DETECTION---------------------
	 // finds gradient in x,y direction, gradient direction is perpendicular to edges
	 // Define values for edge detection
	 int minVal = 60;
	 int maxVal = 150;

	 // Define edge detection image, do edge detection
	 cv::Mat edgesIm;
	 cv::Canny(imageThres, edgesIm, minVal, maxVal);

	 // window for edge detection image
	 cv::String edgeWindowName = "edge detection image";

	 // Show edge detection image
	 cv::imshow(edgeWindowName, edgesIm);
	 cv::waitKey(0); // wait for a key press

	 //--------------------CREATE MASK---------------------------
	 // Create mask to only keep area defined by four corners
	 // Black out every area outside area

	 // Define masked image
	 // Create all black image with same dimensions as original image
	 // 3rd arg is CV_<bit-depth>{U|S|F}C(<number_of_channels>), so this is 8bit, unsigned int, channels: 1
	 cv::Mat mask(image.size().height, image.size().width, CV_8UC1, cv::Scalar(0)); // CV_8UC3 to make it a 3 channel

	 // Define the points for the mask
	 // Use cv::Point type for x,y points
	 // THLIN has changed the point arguments
	 cv::Point p1 = cv::Point(image.cols * (0.5 - 2.0/7.0), image.rows*0.85);
	 cv::Point p2 = cv::Point(image.cols * (0.5 - 2.0/7.0), image.rows*0.4);
	 cv::Point p3 = cv::Point(image.cols * (0.5 + 2.0/7.0), image.rows*0.4);
	 cv::Point p4 = cv::Point(image.cols * (0.5 + 2.0/7.0), image.rows*0.85);

	 // create vector from array with points we just defined
	 cv::Point vertices1[] = {p1,p2,p3,p4};
	 std::vector<cv::Point> vertices (vertices1, vertices1 + sizeof(vertices1) / sizeof(cv::Point));

	 // Create vector of vectors, add the vertices we defined above
	 // (you could add multiple other similar contours to this vector)
	 std::vector<std::vector<cv::Point> > verticesToFill;
	 verticesToFill.push_back(vertices);

	 // Fill in the vertices on the blank image, showing what the mask is
	 cv::fillPoly(mask, verticesToFill, cv::Scalar(255,255,255));

	 // Show the mask
	 cv::imshow("Mask", mask);
	 cv::waitKey(0);

	 //---------------------APPLY MASK TO IMAGE----------------------
	 // create image only where mask and edge Detection image are the same

	 // Create masked im, which takes input1, input2, and output. Only keeps where two images overlap
	 cv::Mat maskedIm = edgesIm.clone();
	 cv::bitwise_and(edgesIm, mask, maskedIm);

	 // Show masked image
	 cv::imshow("Masked Image", maskedIm);
	 cv::waitKey(0);


	 //------------------------HOUGH LINES----------------------------
	 float rho = 2;
	 float pi = 3.14159265358979323846;
	 float theta = pi/180;
	 float threshold = 45;
	 int minLineLength = 100;
	 int maxLineGap = 100;
	 //bool gotLines = false;

	 // Variables for once we have line averages
	 //float posSlopeMean = 0;
	 //double xInterceptPosMean = 0;
	 //float negSlopeMean = 0;
	 //double xInterceptNegMean = 0;



	 vector<cv::Vec4i> lines; // A Vec4i is a type holding 4 integers
	 cv::HoughLinesP(maskedIm, lines, rho, theta, threshold, minLineLength, maxLineGap);

	 // Check if we got more than one line
	 if (!lines.empty() && lines.size() > 2) {

		 // Initialize lines image
		 cv::Mat allLinesIm(image.size().height, image.size().width, CV_8UC3, cv::Scalar(0,0,0));

		 // Loop through lines
		 // std::size_t can store the maximum size of a theoretically possible object of any type
		 for (size_t i = 0; i != lines.size(); ++i) {

			 // Draw line onto image
			 cv::line(allLinesIm, cv::Point(lines[i][0], lines[i][1]),
			             cv::Point(lines[i][2], lines[i][3]), cv::Scalar(0,0,255), 3, 8 );
		 }

		 // Display images
		 cv::imshow("Hough Lines", allLinesIm);
		 cv::waitKey(0);


		 //---------------Account All Possible Horizontal Line--------------------

		 vector< vector<double> > horizonLines;
		 vector<float> yValues;

		 // keep track of if we added one of each, want at least one of each to proceed
		 bool addedLine = false;

		 // array counter for appending new lines
		 int lineCounter = 0;

		 // Loop through all lines
		 for (size_t i = 0; i != lines.size(); ++i) {

			 // Get points for current line
			 float x1 = lines[i][0];
			 float y1 = lines[i][1];
			 float x2 = lines[i][2];
			 float y2 = lines[i][3];

			 // get line length
			 float lineLength =  pow(pow(x2-x1,2) + pow(y2-y1,2), .5);

			 // if line is long enough
			 if (lineLength > 30) {

				 // dont divide by zero
				 if (x2 != x1) {

					 // get slope
					 float slope = (y2-y1)/(x2-x1);

					 // Find angle of line wrt x axis.
					 float tanTheta = tan ( (y2-y1) / (x2-x1) ); // tan(theta) value
					 float angle = atan (tanTheta) * 180/pi;

						// Only pass good line angles,  dont want verticalish/horizontalish lines
						if (abs(angle) < 5 && abs(angle) >= 0) {

							// Add a row to the matrix
							horizonLines.resize(lineCounter+1);

							// Reshape current row to 5 columns [x1, y1, x2, y2, slope]
							horizonLines[lineCounter].resize(6);

							// Add values to row
							horizonLines[lineCounter][0] = x1;
							horizonLines[lineCounter][1] = y1;
							horizonLines[lineCounter][2] = x2;
							horizonLines[lineCounter][3] = y2;
							horizonLines[lineCounter][4] = -slope;
							horizonLines[lineCounter][5] = -angle;

							// add yValues
							yValues.push_back(y1);
							yValues.push_back(y2);

							// Note that we added a positive slope line
							addedLine = true;

							// iterate the counter
							lineCounter++;

					 	}

				 } // if x2 != x1
			 }// if lineLength > 30
	    // cout << endl;
		 } // looping though all lines

/*
		 // If we didn't get any positive lines, go though again and just add any positive slope lines
		 // Be less strict
		 if (addedLine == false) { // if we didnt add any positive lines

			 // loop through lines
			 for (size_t i = 0; i != lines.size(); ++i) {

				 // Get points for current line
				 float x1 = lines[i][0];
				 float y1 = lines[i][1];
				 float x2 = lines[i][2];
				 float y2 = lines[i][3];

				 // Get slope
				 float slope = (y2-y1)/(x2-x1);

				 // Check if slope is positive
				 if (x2 != x1) {

					 // Find angle of line wrt x axis.
					 float tanTheta = tan ( (abs(y2-y1)) / (abs(x2-x1)) ); // tan(theta) value
					 float angle = atan (tanTheta) * 180/pi;

					 // Only pass good line angles,  dont want verticalish/horizontalish lines
					 if (abs(angle) < 9 && abs(angle) >= 0) {

					 	 // Add a row to the matrix
					 	 horizonLines.resize(lineCounter+1);

					 	 // Reshape current row to 5 columns [x1, y1, x2, y2, slope]
					 	 horizonLines[lineCounter].resize(6);

					 	 // Add values to row
					 	 horizonLines[lineCounter][0] = x1;
					 	 horizonLines[lineCounter][1] = y1;
					 	 horizonLines[lineCounter][2] = x2;
					 	 horizonLines[lineCounter][3] = y2;
					 	 horizonLines[lineCounter][4] = -slope;
						 horizonLines[lineCounter][5] = -angle;

					 	 // add yValues
					 	 yValues.push_back(y1);
					   yValues.push_back(y2);

					 	 // Note that we added a positive slope line
					 	 addedLine = true;

					 	 // iterate the counter
					 	 lineCounter++;
					 }
				 }
			 }
		 } // if addedLine == false
*/

		 // If we still dont have lines then fuck
		 if (addedLine == false) {
			 cout << "Not enough lines found" << endl;
		 }

  //------------------Calculate the most frequent slope------------------------- by lin
		vector<float> horizonAngles;
		for (unsigned int i = 0; i != horizonLines.size(); ++i) {
			//printf("%f\n",horizonLines[i][4]);
			if(horizonLines[i][5] != 0)
				horizonAngles.push_back(horizonLines[i][5]);
		}
		sort(horizonAngles.begin(), horizonAngles.end());
		for (unsigned int i = 0; i != horizonAngles.size(); ++i){
			printf("%f\n",horizonAngles[i]);
		}
		double angleMode = find_mode(horizonAngles);
		printf("%f\n", angleMode);

	//-----------------GET POSITIVE/NEGATIVE SLOPE AVERAGES-----------------------
	// Average the position of lines and extrapolate.
	// Define vector of 'good' slopes, slopes that are drastically different than the others are thrown out
	vector<vector<float>> anglesGoodLines;
	float Sum = 0.0; // sum so we'll be able to get mean

	lineCounter = 0;
	// Loop through positive slopes and add the good ones
	for (size_t i = 0; i != horizonLines.size(); ++i) {

		// check difference between current slope and the median. If the difference is small enough it's good
		if (abs(horizonLines[i][5] - angleMode) < 0.5) {
				anglesGoodLines.resize(lineCounter+1);
				anglesGoodLines[lineCounter].resize(6);
				anglesGoodLines[lineCounter][0] = horizonLines[i][0]; // Add slope to posSlopesGood
				anglesGoodLines[lineCounter][1] = horizonLines[i][1];
				anglesGoodLines[lineCounter][2] = horizonLines[i][2];
				anglesGoodLines[lineCounter][3] = horizonLines[i][3];
				anglesGoodLines[lineCounter][4] = horizonLines[i][4];
				anglesGoodLines[lineCounter][5] = horizonLines[i][5];
				Sum += horizonLines[i][5]; // add to sum
				++lineCounter;
		}
	}

	// Get mean of good positive slopes
	float angleMean = Sum/anglesGoodLines.size();
	printf("%f\n", angleMean);

	////////////////////////////////////////////////////////////////////////
  //------------------Calculate the most frequent y coord------------------------- by lin

	vector<float> yIntercepts;
	for (size_t i = 0; i != anglesGoodLines.size(); ++i) {
		double x1 = anglesGoodLines[i][0]; // x value
		double y1 = image.rows - anglesGoodLines[i][1]; // y value...yaxis is flipped
		double slope = anglesGoodLines[i][4];
		double yIntercept = y1-slope*x1; // yintercept of line
		double xIntercept = -yIntercept/slope; // find x intercept based off y = mx+b
		if (isnan(yIntercept) == 0) { // check for nan
			yIntercepts.push_back(yIntercept); // add value
		}
	}

	sort(yIntercepts.begin(), yIntercepts.end());
	printf("%s\n", "y intercept");
	for (unsigned int i = 0; i != yIntercepts.size(); ++i){
		printf("%f\n", yIntercepts[i]);
	}

	double yIntMode = find_mode(yIntercepts); //find apir is erronous need fix
	printf("%f\n", yIntMode);

	//-----------------------PLOT LANE LINES------------------------
	// Need end points of line to draw in. Have x1,y1 (xIntercept,im.shape[1]) where
	// im.shape[1] is the bottom of the image. take y2 as some num (min/max y in the good lines?)
	// then find corresponding x

	// Create image, lane lines on real image
	cv::Mat laneLineImage = image.clone();
	cv::Mat laneFill = image.clone();

	// Positive Slope Line
	float slope = tan (angleMean * pi/180);
	double x1 = 0;
	int y1 = yIntMode;
	double x2 = image.size().width;
	double y2 = y1 + (x2-x1)*slope;

	// Add positive slope line to image
	x1 = int(x1 + .5);
	x2 = int(x2 + .5);
	y1 = int(y1 + .5);
	y2 = int(y2 + .5);
	cv::line(laneLineImage, cv::Point(x1, image.size().height-y1), cv::Point(x2, image.size().height - y2),
																						cv::Scalar(0,255,0), 3, 8 );

	// Plot positive and negative lane lines
	 cv::imshow("Lane lines on image", laneLineImage);
	 cv::waitKey(0); // wait for a key press

/*
	 // -----------------BLEND IMAGE-----------------------
	 // Use cv::Point type for x,y points
	 cv::Point v1 = cv::Point(x1, image.size().height - y1);
	 cv::Point v2 = cv::Point(x2, image.size().height - y2);
	 cv::Point v3 = cv::Point(x1N, image.size().height-y1N);
	 cv::Point v4 = cv::Point(x2N, image.size().height - y2);

	 // create vector from array of corner points of lane
	 cv::Point verticesBlend[] = {v1,v3,v4,v2};
	 std::vector<cv::Point> verticesVecBlend (verticesBlend, verticesBlend + sizeof(verticesBlend) / sizeof(cv::Point));

	 // Create vector of vectors to be used in fillPoly, add the vertices we defined above
	 std::vector<std::vector<cv::Point> > verticesfp;
	 verticesfp.push_back(verticesVecBlend);

	 // Fill area created from vector points
	 cv::fillPoly(laneFill, verticesfp, cv::Scalar(0,255,255));

	 // Blend image
	 float opacity = .25;
	 cv::Mat blendedIm;
	 cv::addWeighted(laneFill,opacity,image,1-opacity,0,blendedIm);

	 // Plot lane lines
	 cv::line(blendedIm, cv::Point(x1, image.size().height-y1), cv::Point(x2, image.size().height - y2),
	 																						cv::Scalar(0,255,0), 8, 8 );
	 cv::line(blendedIm, cv::Point(x1N, image.size().height-y1N), cv::Point(x2N, image.size().height - y2),
	 																						cv::Scalar(0,255,0), 8, 8 );

	 // Show final frame
	 cv::imshow("Final Output", blendedIm);
	 cv::waitKey(0);

*/

 } // end if we got more than one line


	 // We do none of that if we don't see enough lines
	 else {
		 cout << "Not enough lines found" << endl;
	 }

return 0;
}
