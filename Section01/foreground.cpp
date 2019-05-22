#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/bgsegm.hpp>

#include "videoprocessor.h"
#include "BGFGSegmentor.h"

int main()
{
	// Open the video file
    cv::VideoCapture capture("bike.avi");
	// check if video successfully opened
	if (!capture.isOpened())
		return 0;

	// current video frame
	cv::Mat frame;
	// foreground binary image
	cv::Mat foreground;
	// background image
	cv::Mat background;

	cv::namedWindow("Extracted Foreground");

	// The Mixture of Gaussian object
	// used with all default parameters
	cv::Ptr<cv::BackgroundSubtractor> ptrMOG = cv::bgsegm::createBackgroundSubtractorMOG();

	bool stop(false);
	// for all frames in video
	while (!stop) {

		// read next frame if any
		if (!capture.read(frame))
			break;

		// update the background
		// and return the foreground
		ptrMOG->apply(frame,foreground,0.01);

		// Complement the image
		cv::threshold(foreground,foreground,128,255,cv::THRESH_BINARY_INV);

		// show foreground and background
		cv::imshow("Extracted Foreground",foreground);

		// introduce a delay
		// or press key to stop
		if (cv::waitKey(10)>=0)
				stop= true;
	}

	cv::waitKey();

	// Create video procesor instance
	VideoProcessor processor;

	// Create background/foreground segmentor
	BGFGSegmentor segmentor;
	segmentor.setThreshold(25);

	// Open video file
	processor.setInput("bike.avi");

	// set frame processor
	processor.setFrameProcessor(&segmentor);

	// Declare a window to display the video
	processor.displayOutput("Extracted Foreground");

	// Play the video at the original frame rate
	processor.setDelay(1000./processor.getFrameRate());

	// Start the process
	processor.run();

	cv::waitKey();
}
