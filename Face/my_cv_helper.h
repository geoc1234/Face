#pragma once
#ifndef  _CV_HELPER_H
#define _CV_HELPER_H

#include <iostream>
#include <stdio.h>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

// Create a label or filename by concatenating strings
std::string mycvLableMaker(std::string root_name, int index, std::string path);
std::string mycvLableMaker(std::string root_name, std::string path);
std::string mycvLableMaker(std::string root_name, int index);

// Extract the region of interest from an image and optionally save it
void mycvImgExtract(cv::Mat& img_input, std::vector<cv::Rect> roi, std::vector<cv::Mat>& img_out, std::vector<std::string> filename);
void mycvImgExtract(cv::Mat& img_input, cv::Rect roi, cv::Mat& img_out, std::string filename);
void mycvImgExtract(cv::Mat& img_input, cv::Rect roi, cv::Mat& img_out);

// Resize an image without creating distortion. Returns a new copy without altering the original
cv::Mat mycvImgResize(cv::Mat& inputImage, int width, int height);
cv::Mat mycvImgResize(cv::Mat& inputImage, int width, int height, cv::Point2f & center);

// Check the bounds of a ROI and ensure they fall within the boundries of the image.  correct the boundary as needed.
void mycv_CheckAndCorrectROIBoundries(cv::Mat& image, cv::Rect& rect);

// Landmark types for 68 point model
struct Landmark_t
{
	std::vector<cv::Point2f> jaw[17];
	std::vector<cv::Point2f> righteyebrow[5];
	std::vector<cv::Point2f> lefteyebrow[5];
	std::vector<cv::Point2f> nose[9];
	std::vector<cv::Point2f> righteye[6];
	std::vector<cv::Point2f> lefteye[6];
	std::vector<cv::Point2f> mouth[20];
};

struct jaw_t { std::vector<cv::Point2f> data[17]; };
struct eyebrow_t { std::vector<cv::Point2f> data[5]; };
struct eye_t { std::vector<cv::Point2f> data[5]; };
struct nose_t { std::vector<cv::Point2f> data[9]; };
struct mouth_t { std::vector<cv::Point2f> data[20]; };

// functions for converting between types....   from --> to 
void convert(jaw_t& jaw, std::vector<cv::Point2f>& lndmrks);

// A class for storing the results of face detection and recognition
class Faces
{
public:
	Faces();
	bool faceDetected;
	bool faceRecognized;
	float confidence;
	int x1, x2, y1, y2;
	std::string label;
	std::vector<cv::Point2f> lmarks;
	cv::Rect rect;
	cv::Rect getRect();
	cv::Rect zoom(float factor);
	cv::Point2f getCenter();
	jaw_t getJaw();
	eye_t getRightEye();
	eyebrow_t getRightEyeBrow();
	eye_t getLeftEye();
	eyebrow_t getLeftEyeBrow();
	nose_t getNose();
	mouth_t getMouth();
};

#endif // ! "_CV_HELPER_H"
