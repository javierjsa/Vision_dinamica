/*
 * Calculaflujos.cpp
 *
 *  Created on: 6 mar. 2017
 *      Author: jsa
 */

#ifndef INCLUDES
#define INCLUDES

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>


#endif


#ifndef CALCULAFLUJOS_H_
#include "Calculaflujos.h"
#endif



using namespace cv;
using namespace std;

Calculaflujos::Calculaflujos(cv::Mat* img_t, cv::Mat* img_t1) {

		int c=(this->img_t)->cols;
	    int r=(this->img_t)->rows;
		this->img_t=img_t;
		this->img_t1=img_t1;
		this->Ixt= Mat::zeros(100,100,CV_32F);
	}

void Calculaflujos::Calcula_gradiente(){
		int a=1;
		cout<<"holita\n";

	}

Calculaflujos:: ~Calculaflujos() {
	int a=1;
	}



