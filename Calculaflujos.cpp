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

#define D_TYPE CV_32F

using namespace cv;
using namespace std;

Calculaflujos::Calculaflujos(cv::Mat* img_t, cv::Mat* img_t1) {

	    int c=(*img_t).cols;
	    int r=(*img_t).rows;

	    this->img_t=img_t;
	    this->img_t1=img_t1;
		this->Ixt= Mat::zeros(r,c,D_TYPE); //Gradiente X de t
		this->Iyt= Mat::zeros(r,c,D_TYPE); // Gradiente Y de t
		this->Ixt1= Mat::zeros(r,c,D_TYPE); // Gradiente x de t1
		this->Iyt1= Mat::zeros(r,c,D_TYPE); // Gradiente y de t1
		this->Ix= Mat::zeros(r,c,D_TYPE);
		this->Iy= Mat::zeros(r,c,D_TYPE);
		this->I2t= Mat::zeros(r,c,D_TYPE);
		this->I2t1= Mat::zeros(r,c,D_TYPE);
		this->It= Mat::zeros(r,c,D_TYPE);

	}



void Calculaflujos::Calcula_gradiente(){

		int kernel_size = 41;

		GaussianBlur(*(this->img_t),this->I2t,Size(kernel_size,kernel_size),0,0,BORDER_DEFAULT);
		GaussianBlur(*(this->img_t1),this->I2t1,Size(kernel_size,kernel_size),0,0,BORDER_DEFAULT);

		this->It=(this->I2t1)-(this->I2t);

		Sobel(*this->img_t, this->Ixt, D_TYPE, 1, 0, 3);
		Sobel(*this->img_t, this->Iyt, D_TYPE, 0, 1, 3);

		Sobel(this->I2t1, this->Ixt1, D_TYPE, 1, 0, 3);
		Sobel(this->I2t1, this->Iyt1, D_TYPE, 0, 1, 3);

		this->Ix=(this->Ixt+this->Ixt1)/2;
		this->Iy=(this->Iyt+this->Iyt1)/2;

	}


Calculaflujos:: ~Calculaflujos() {

	}
