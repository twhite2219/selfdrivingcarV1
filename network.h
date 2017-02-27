#ifndef NETWORK_H
#define NETWORK_H
#include <stdio.h>
#include <pigpio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wiringPi.h>
#include <softPwm.h>
#include "network.h"
#include "move.h"
#include <sstream>
#include <iostream>
#include "distance.h"
#include <linux/videodev2.h>
#include <../include/libv4l2.h>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>  
#include <opencv2/videoio.hpp>
#include "PiCamF.h"
using namespace cv;
using namespace std;

	//need to down scale images to around 50x50 
	
	
char const *path = "/home/pi/selfdrivingcarV1/train_data/";//needs folders of 0 1 2 3  holdig 50 pics each for representing outputs
int const numFilesDirs[]={110,95,63}; //number of photos for each direction (fwd l r)
char const strDirs[]={'0','1','2'}; //optional outputs " 0=go,1 right 2 left
int const numDirs = 3;//number of directions

//cv::Ptr<cv::ml::ANN_MLP> ann = cv::ml::ANN_MLP::create();


void readScanStore(){
cv::Mat trainingData;//mat collectionn of images to train with 


cv::Mat trainingLabels (0,0,(CV_32S));
//same as svm??
for (int i=0;i!=numDirs; i++){//outer for loop to go through all 4 output options
	int numFiles = numFilesDirs[i];//assign inner loop based on size of samples from current outerloop val
		for (int j=0;j!=numFiles;j++){//loop through all files within current output value
			std::cout << "direction" << strDirs[i] << "file: " << j <<".jpg" << "\n";// print current output val and files associated with that direction
			std::stringstream ss;
			ss << path << strDirs[i] << "/" << j << ".jpg";//print current working image
			cv::Mat img = cv::imread(ss.str(),0);
		
		
		if (!img.data)
		cout << "error no file found " << ss << endl;
			//Size size(10,10);
			//Mat ImgCon;
			//resize(img,ImgCon,size);
			//ImgCon.reshape(1,1);
		//assume img is continous
			//reshape image to 1xtotal res 
			
			trainingData.push_back(img); //push back image
			trainingLabels.push_back(i);//assign instruction corresponding to image in label set
		}
	}  
	
trainingData.convertTo(trainingData, CV_32FC1);//convert all images to cv32f (numeric values)

cv::FileStorage fs("VALUES.xml",FileStorage::WRITE);//store numeric values in xml file as training data values for each image.
fs << "TrainingData" <<trainingData;//assign each image 
fs << "classes" << trainingLabels;//assign associated classes

printf("complete");
	}
	
void convertLabels(){
	cv::FileStorage fs;
	fs.open("VALUES.xml",cv::FileStorage::READ);
	cv::Mat trainData;
	Mat classes;
	fs["TrainingData"] >> trainData;
	fs["classes"] >> classes;
	
	//1st element is input nodes (features of sample)
	//;ast is outputlayer size 
	//all inbetween are hidden layer
	
	int buffer[]={trainData.cols,100,numDirs};
	printf("%i",trainData.cols);
	
	cv::Mat const layers(1,3,CV_32SC1, buffer);
	//ann->create(layers, cv::ml::ANN_MLP::SIGMOID_SYM,1,1);
	
	
	//prepare trainingclasses reate a mat with n trained data from n classes
	
	cv::Mat trainClasses(trainData.rows, numDirs,CV_32FC1);
	for (int i=0; i !=trainClasses.rows;i++){
	//	int const labels = *classes.ptr<int>(i);
		//train_ptr = trainClasses.ptr<float>(i);
		for (int k=0;k!= trainClasses.cols;k++){
	//		*train_ptr  k!= labels ?0:1
		//	train_ptr++;
		}
	}
	cv::Mat const weights = cv::Mat::ones(1,trainData.rows, CV_32FC1);
	
	//learn classifier
	//ann->train(trainData,trainClasses,weights);
	
	
	  
	
	
	
}  
	
#endif
