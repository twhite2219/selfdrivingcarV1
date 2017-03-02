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

void readScanStore(){
cv::Mat trainingData;//mat collectionn of images to train with 
char const *path = "/home/pi/selfdrivingcarV1/train_data/";//needs folders of 0 1 2 3  holdig 50 pics each for representing outputs
int const numFilesDirs[]={217,217,128}; //number of photos for each direction (fwd l r)
char const strDirs[]={'0','1','2'}; //optional outputs " 0=go,1 right 2 left
int const numDirs = 3;//number of directions


cv::Mat trainingLabels (0,0,(CV_32S));
//same as svm??
for (int i=0;i!=numDirs; i++){//outer for loop to go through all 4 output options
	int numFiles = numFilesDirs[i];//assign inner loop based on size of samples from current outerloop val
		for (int j=0;j!=numFiles;j++){//loop through all files within current output value
			std::cout << "direction" << strDirs[i] << "file: " << j <<".jpg" << "\n";// print current output val and files associated with that direction
			std::stringstream ss;
			ss << path << strDirs[i] << "/" <<"i (" << j+1 << ").jpg";//print current working image
			cv::Mat img = cv::imread(ss.str(),0);
		
		
		if (!img.data)
		cout << "error no file found " << ss.str() << endl;
		
		
			Size size(10,10);
			Mat ImgCon;
			resize(img,ImgCon,size);
			ImgCon =ImgCon.reshape(1,1);
			//assume img is continous
			//reshape image to 1xtotal res 
			
			trainingData.push_back(ImgCon); //push back image
			trainingLabels.push_back(i);//assign instruction corresponding to image in label set
		}
	}  
	
trainingData.convertTo(trainingData, CV_32F);//,1/255.0);//convert all images to cv32f (numeric values)

cv::FileStorage fs("VALUES.xml",FileStorage::WRITE);//store numeric values in xml file as training data values for each image.
fs << "TrainingData" <<trainingData;//assign each image 
fs << "classes" << trainingLabels;//assign associated classes

printf("complete");
	}

void train_test(int nclasses, const Mat &train_data, const Mat &train_labels, Mat &confusion) {
   
    int nfeatures = train_data.cols;
    Ptr<ml::ANN_MLP> ann = ml::ANN_MLP::create();
    Mat_<int> layers(4,1);
    layers(0) = nfeatures;     // input
    layers(1) = nclasses * 8;  // hidden
    layers(2) = nclasses * 4;  // hidden
    layers(3) = nclasses;      // output, 1 pin per class.
    ann->setLayerSizes(layers);
    ann->setActivationFunction(ml::ANN_MLP::SIGMOID_SYM,0,0);
    ann->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER+TermCriteria::EPS, 100000, 0.0001));
    ann->setTrainMethod(ml::ANN_MLP::BACKPROP, 0.0001);
printf("sending data to train_test"); // setup the ann:

    // ann requires "one-hot" encoding of class labels:
    Mat train_classes = Mat::zeros(train_data.rows, nclasses, CV_32F);////should this be 32f ONLY???????
    
    
    printf("%i\n",train_classes.rows);
    //////////////////////////////////////////////////////////////////////////
  for(int i=0; i<train_classes.rows; i++)
  {
  train_classes.at<float>(i, train_labels.at<int>(i)) = 1.f;
  }
    cerr << train_data.size() << " " << train_classes.size() << endl;
/////////////////////////////////////////////////////////////////////////////////////  
    ann->train(train_data, ml::ROW_SAMPLE, train_classes);


	cv::FileStorage fs("NNPARAMS.xml",FileStorage::WRITE);//store numeric values in xml file as training data values for each image.
	ann->write(fs);
}	


void trainNetwork(){
    int nclasses = 3;
    
	cv::FileStorage fs;
fs.open("VALUES.xml", cv::FileStorage::READ);
cv::Mat trainData;
cv::Mat classes;
fs["TrainingData"] >> trainData;
fs["classes"] >> classes;
    Mat confusion(nclasses,nclasses,CV_32S, Scalar(0)); // will hold our test results



 train_test(nclasses, trainData, classes, confusion);

   	
}
	

	
#endif
