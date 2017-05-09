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
cv::Mat TestData;
char const *path = "/home/pi/selfdrivingcarV1/train_data/";//needs folders of 0 1 2 holding training data fr resenting outputs
int const numFilesDirs[]={875,790,0};///////change!!!!11111!!!!!!!!!!!!!!! //number of photos for each direction ()LEFT =0 straight =1 right =2
char const strDirs[]={'0','1','2'}; //optional outputs " 
int const numDirs = 3;//number of directions

cv::Mat TestLabels (0,0,(CV_32S));
cv::Mat trainingLabels (0,0,(CV_32S));
//same as svm??
for (int i=0;i!=numDirs; i++){//outer for loop to go through all 3 output options
	int numFiles = numFilesDirs[i];//assign inner loop based on size of samples from current outerloop val
		for (int j=0;j!=numFiles;j++){//loop through all files within current output value
			std::cout << "direction" << strDirs[i] << "file: " << j <<".jpg" << "\n";// print current output val and files associated with that direction
			std::stringstream ss;
			ss << path << strDirs[i] << "/"<< j <<".jpg";			//"/" <<"i (" << j+1 << ").jpg";//print current working image
			cv::Mat img = cv::imread(ss.str(),0);
		
		
		if (!img.data)
		cout << "error no file found " << ss.str() << endl;
		
		
			Size size(10,10);
			Mat ImgCon;
			resize(img,ImgCon,size);
			ImgCon =ImgCon.reshape(1,1);
			//assume img is continous
			//reshape image to 1xtotal res 
			if (j%5==0){//push every 5 images to test set
				TestData.push_back(ImgCon);
				TestLabels.push_back(i);
			}
			trainingData.push_back(ImgCon); //push back image
			trainingLabels.push_back(i);//assign instruction corresponding to image in label set
		}
	}  
	TestData.convertTo(TestData, CV_32F);
trainingData.convertTo(trainingData, CV_32F);//,1/255.0);//convert all images to cv32f (numeric values)

cv::FileStorage fs("TRAIN_VALUES.xml",FileStorage::WRITE);//store numeric values in xml file as training data values for each image.
fs << "TrainingData" <<trainingData;//assign each image 
fs << "classes" << trainingLabels;//assign associated classes


cv::FileStorage ffs("TEST_VALUES.xml",FileStorage::WRITE);//store numeric values in xml file as training data values for each image.
ffs << "TestData" <<TestData;//assign each image 
ffs << "classes" << TestLabels;//assign associated classes


printf("complete");
	}

void trainNetwork() {
   
    int nclasses = 3;
    cv::FileStorage fsa;
	fsa.open("TRAIN_VALUES.xml", cv::FileStorage::READ);
	cv::Mat train_data;
	cv::Mat train_labels;
	fsa["TrainingData"] >> train_data;
	fsa["classes"] >> train_labels;
   

   
    int nfeatures = train_data.cols;
    Ptr<ml::ANN_MLP> ann = ml::ANN_MLP::create();
    Mat_<int> layers(5,1);
    layers(0) = nfeatures;     // input 100
    layers(1) = 50;//nclasses * 16;  // hidden 51
    layers(2) = 25;//nclasses * 8;  // hidden  24
    layers(3) = 12;//nclasses * 4;//hidden 12
    layers(4) = nclasses;      // output, 1 pin per class = 3.
    ann->setLayerSizes(layers);
    ann->setActivationFunction(ml::ANN_MLP::SIGMOID_SYM,0,0);
    ann->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER+TermCriteria::EPS, 50000, 0.0001));
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


	
void TestNetwork(){
    cv::FileStorage fsa;
	fsa.open("TEST_VALUES.xml", cv::FileStorage::READ);
	cv::Mat test_data;
	cv::Mat test_labels;
	fsa["TestData"] >> test_data;
	fsa["classes"] >> test_labels;
	
   cout << test_data.row(1).size()<<endl;
	
	FileStorage fs("NNPARAMS.xml",FileStorage::READ);
	Ptr<ml::ANN_MLP> Neural_Net = cv::Algorithm::read<ml::ANN_MLP>(fs.root());
	if (!Neural_Net->isTrained()) printf("network not trained \n");


else {
	int corrcount =0;
			
			for (int i=0; i<test_data.rows; i++){
			Mat Result;
			Neural_Net->predict(test_data.row(i),Result);
			int truth = test_labels.at<int>(i);		
			cv::Point max_loc;
			
			cv::minMaxLoc(Result,0,0,0,&max_loc);
			int pred = max_loc.x;	
			if (pred == truth)
			corrcount++;
			cout << Result <<endl;
			printf("::::  actual : %i , Expected : %i\n",pred,truth);
			
					
					
   }printf("Test Result : %i/%i",corrcount,test_data.rows);  
			
}
}
	
#endif
