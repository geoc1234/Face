/* Copyright 2020, G.W. Coulston
Program will extract faces from  video or from files if filename is given on the command line. The program has a built-in path to the folder where datasets
are stored. (C:\users\geo\datasets\).
*/

#include "my_cv_helper.h"
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/face.hpp>
#include <opencv2/imgproc.hpp>
#include "DrawLandmarks.h"

int main(int argc, char** argv)
{
	// booleans controlling looping
	bool loop = false;
	bool newImage = false;

	//the neural net parameters for the trained network to detect faces in an image 
	std::string model = "c:/users/geo/datasets/deploy.prototxt.txt";
	std::string dataset = "c:/users/geo/datasets/res10_300x300_ssd_iter_140000.caffemodel";
	cv::dnn::Net Net_SSD = cv::dnn::readNetFromCaffe(model, dataset);
	std::vector<Faces> vector_of_Faces; //store the results for each face detected in a vector, with each element of the vector a class: Faces
	std::vector<cv::Rect> faces_; //even though saved in vector_of_Faces, easier to recall later

	//the container for the landmarks returned by OpenCV facemark algorithm
	cv::Ptr<cv::face::Facemark> facemark = cv::face::FacemarkLBF::create();
	facemark->loadModel("c:/users/geo/datasets/gsoc2017/data/lbfmodel.yaml");
	std::vector<vector<cv::Point2f>> landmarks;

	//images are stored as a 2d Matrix
	cv::Mat image, image_raw;
	cv::Mat blob;

	//webcam can generate data stream
	cv::VideoCapture webcam(0);

	//interpret the command line
	if (argc == 1)
	{
		std::cout << "Entering Webcam Capture mode with continuous looping\n" << std::endl;
		loop = true;
	}
	else
	{
		std::cout << "Processing files listed on the command line\n" << std::endl;
		newImage = true;
	}

	int counter = 1;
	while (loop || counter < argc)
	{
		if (loop)
		{
			std::cout << "getting webcam frame\n";
			webcam >> image_raw;
		}
		else
		{
			if (counter < argc)
			{
				std:string	file;
				file = mycvLableMaker(std::string(argv[counter]), "c:/users/geo/datasets/");
				image_raw = cv::imread(file);
			}
			else
			{
				break;
			}

		}

		image = mycvImgResize(image_raw, 300, 300);
		int frameHeight = image.rows;
		int frameWidth = image.cols;

		//the caffe model inputs image data in a blob structure.
		blob = cv::dnn::blobFromImage(image, 1.0, cv::Size(300, 300), cv::Scalar(127.5, 127.5, 127.5), false, false);

		//input the blob into the caffe model for face detection
		Net_SSD.setInput(blob);

		//propogate the blob forward through the neural net to get predictions of where faces exist in the image
		cv::Mat	Net_SSD_predict = Net_SSD.forward();
		
		//the results are accessed via a matrix structure.	TODO: add a reference to the documentation on the structure of this matrix
		cv::Mat detectionMat(Net_SSD_predict.size[2], Net_SSD_predict.size[3], CV_32F, Net_SSD_predict.ptr<float>());

		//post process the results by drawing a box around each face in the immage
		for (int i = 0; i < detectionMat.rows; i++)
		{
			if (detectionMat.at<float>(i, 2) >= 0.5)
			{
				Faces res;
				res.faceDetected = true;
				res.confidence = detectionMat.at<float>(i, 2);
			
				res.x1 = static_cast<int>(detectionMat.at<float>(i, 3) * frameWidth);
				res.y1 = static_cast<int>(detectionMat.at<float>(i, 4) * frameHeight);
				res.x2 = static_cast<int>(detectionMat.at<float>(i, 5) * frameWidth);
				res.y2 = static_cast<int>(detectionMat.at<float>(i, 6) * frameHeight);
				res.rect = res.getRect();
				res.label = mycvLableMaker("unknown_", i);

				vector_of_Faces.push_back(res);
				faces_.push_back(res.rect);

				cv::rectangle(image,res.rect, cv::Scalar(255, 0, 0), 1, 8, 0);
			}
		}

		// show the image with all faces enclosed by their associated rectangle
		cv::imshow("detected faces", image);
		cv::waitKey(10);

		// save the extracted faces
		// first create the corresponding vector of filenames
		std::vector<std::string> fnames;
		std::string path = "c:/users/geo/datasets/";
		if (faces_.size() > 0)
		{
			for (int i = 0; i < faces_.size(); i++)
			{
				fnames.push_back(mycvLableMaker(vector_of_Faces[i].label, 0, path));
			}
			// now store the extracted faces
			std::vector<cv::Mat> indiv_faces;
			mycvImgExtract(image, faces_, indiv_faces, fnames);

			// we don't need the extracted faces, need to learn how to free up that memory

			// get the facial landmarks using OpenCV facemarking tool
			bool success = facemark->fit(image, faces_, landmarks);

			if (success)
			{
				// add the landmarks to the face objects and to the image
				for (int i = 0; i < landmarks.size(); i++)
				{
					vector_of_Faces[i].lmarks = landmarks[i];
					drawLandmarks(image, landmarks[i]);
				}
				// extract the faces with the landmarks added; first change the filename index
				fnames.clear();
				for (int i = 0; i < faces_.size(); i++)
				{
					fnames.push_back(mycvLableMaker(vector_of_Faces[i].label, 1, path));
				}
				mycvImgExtract(image, faces_, indiv_faces, fnames);

				imshow("Facial Landmark Detection", image);
				waitKey(10);
			}
		}
		vector_of_Faces.clear();
		faces_.clear();
		landmarks.clear();
		counter++;

	} // end while loop
	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu
