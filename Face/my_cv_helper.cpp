#include "my_cv_helper.h"


void mycv_CheckAndCorrectROIBoundries(cv::Mat& image, cv::Rect& rect)
{
	rect.x = (rect.x > 0 ? rect.x : 0.0);
	rect.width = ((rect.x + rect.width) < image.cols ? rect.width : image.cols - rect.x);
	rect.y = (rect.y < 0 ? 0 : rect.y);
	rect.height = (rect.y + rect.height > image.rows ? image.rows - rect.y : rect.height);
}


std::string mycvLableMaker(std::string root_name, std::string path)
{// Create a label or filename by concatenating strings
	std::string str;
	std::stringstream ss;
	ss << path << root_name;
	return str = ss.str();
}
std::string mycvLableMaker(std::string root_name, int index, std::string path)
{// Create a label or filename by concatenating strings
	std::string str;
	std::stringstream ss;
	ss << path << root_name << index << ".jpg";
	return str = ss.str();
}
std::string mycvLableMaker(std::string root_name, int index)
{// Create a label or filename by concatenating strings
	std::string str; 
	std::stringstream ss;
	ss << root_name << index;
	return str = ss.str();
}

void mycvImgExtract(cv::Mat& img_input, std::vector<cv::Rect> roi, std::vector<cv::Mat> & img_out, std::vector<std::string> filename)
{
	//function to extract a region of interest from an image. If filename provided the ROI is saved. Can accept a vector of regions of interest
	for (int i = 0; i < roi.size(); i++)
	{	
		mycv_CheckAndCorrectROIBoundries(img_input, roi[i]);
		img_out.push_back(cv::Mat(img_input, roi[i]));

		cv::imwrite(filename[i], img_out[i]);
	}
}
void mycvImgExtract(cv::Mat& img_input, cv::Rect roi, cv::Mat& img_out, std::string filename)
{
	//function to extract a region of interest from an image. If filename provided the ROI is saved.
	mycv_CheckAndCorrectROIBoundries(img_input, roi);
	img_out = cv::Mat(img_input, roi);
	
	// Save the frame into a file
	cv::imwrite(filename, img_out);
}
void mycvImgExtract(cv::Mat& img_input, cv::Rect roi, cv::Mat& img_out)
{
	//function to extract a region of interest from an image. If filename provided the ROI is saved.
	
	mycv_CheckAndCorrectROIBoundries(img_input, roi);
	img_out = cv::Mat(img_input, roi);
}

cv::Mat mycvImgResize(cv::Mat& inputImage, int width, int height, cv::Point2f& center)
{ //function scales and crops to preserve aspect ratio

	float w = inputImage.cols;
	float h = inputImage.rows;
	cv::Mat temp, temp2;

	float xscale = float(width) / w;
	float yscale = float(height) / h;

	if (xscale > yscale)
	{
		yscale = xscale;
	}
	if (yscale > xscale)
	{
		xscale = yscale;
	}

	cv::resize(inputImage, temp, cv::Size(), xscale, yscale);
	float xmin = (center.x - width / 2.0 > 0 ? center.x - width / 2.0 : 0);
	float ymin = (center.y - height / 2.0 > 0 ? center.y - height / 2.0 : 0);
	float wd = (center.x + width / 2.0 < w ? width : w - xmin);
	float ht = (center.y + height / 2.0 < h ? height : h - ymin);
	temp2 = cv::Mat(temp, cv::Rect(xmin, ymin, wd, ht));

	return temp2;
}

cv::Mat mycvImgResize(cv::Mat& inputImage, int width, int height)
{ //function scales and crops to preserve aspect ratio

	float w = inputImage.cols;
	float h = inputImage.rows;
	cv::Mat temp, temp2;

	float xscale =float (width) / w;
	float yscale = float(height) / h;

	if (xscale > yscale)
	{
		yscale = xscale;
	}
	if (yscale > xscale)
	{
		xscale = yscale;
	}

	cv::resize(inputImage, temp, cv::Size(), xscale, yscale);

	temp2 = cv::Mat(temp, cv::Rect(0, 0, width, height));

	return temp2;
}

void convert(jaw_t& jaw, std::vector<cv::Point2f>& lndmrks)
{
	for (int i = 0; i < 17; i++)
	{
		cv::Point2f point = cv::Point2f(jaw.data->at(i).x, jaw.data->at(i).y);
		lndmrks.push_back(point);
	}
}

Faces::Faces()
{
}
cv::Rect Faces::getRect()
{
	return cv::Rect(this->x1, this->y1, this->x2 - this->x1, this->y2 - this->y1);
}
cv::Rect Faces::zoom(float factor)
{
	float x_center = (this->x2 + this->x1) / 2;
	float y_center = (this->y2 + this->y1) / 2;
	float x_min = x_center - (x_center - this->x1) * factor;
	float y_min = y_center - (y_center - this->y1) * factor;
	float x_max = x_center - (x_center - this->x2) * factor;
	float y_max = y_center - (y_center - this->y2) * factor;

	return cv::Rect(x_min, y_min, x_max - x_min, y_max - y_min);
}
cv::Point2f Faces::getCenter()
{
	return cv::Point2f((this->x2 + this-> x1)/2.0, (this->y1 + this->y2)/2.0);
}
jaw_t Faces::getJaw()
{
	jaw_t vec;
	for (int i=0; i < 17; i++)
	{
		vec.data->push_back(cv::Point2f(this->lmarks.at(i).x, this->lmarks.at(i).y));
	}
	return vec;
}
eye_t Faces::getRightEye()
{
	eye_t vec;
	return vec;
}
eyebrow_t Faces::getRightEyeBrow()
{	
	eyebrow_t vec;
	return vec;
}
eye_t Faces::getLeftEye()
{
	eye_t vec;
	return vec;
}
eyebrow_t Faces::getLeftEyeBrow()
{
	eyebrow_t vec;
	return vec; 
}
nose_t Faces::getNose()
{
	nose_t vec;
	return vec; 
}
mouth_t Faces::getMouth()
{
	mouth_t vec;
	return vec ;
}
