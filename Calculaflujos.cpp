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

	    this->img_t=img_t;
		this->img_t1=img_t1;
		int c=(this->img_t)->cols;
	    int r=(this->img_t)->rows;

		this->Ixt= Mat::zeros(r,c,CV_32F); //Gradiente X de t
		this->Iyt= Mat::zeros(r,c,CV_32F); // Gradiente Y de t
		this->Ixt1= Mat::zeros(r,c,CV_32F); // Gradiente x de t1
		this->Iyt1= Mat::zeros(r,c,CV_32F); // Gradiente y de t1
		this->Ix= Mat::zeros(r,c,CV_32F);
		this->Iy= Mat::zeros(r,c,CV_32F);
		this->I2t= Mat::zeros(r,c,CV_32F);
		this->I2t1= Mat::zeros(r,c,CV_32F);
		this->It= Mat::zeros(r,c,CV_32F);

	}

void Calculaflujos::Calcula_gradiente(){



		int kernel_size = 11;
		//int scale = 1;
		//int delta = 0;

		Sobel( *(this->img_t), this->Ixt, CV_32F, 1, 0, kernel_size);
		Sobel( *(this->img_t), this->Iyt, CV_32F, 0, 1, kernel_size);

		this->Ixt=abs(this->Ixt);
		this->Iyt=abs(this->Iyt);

		Sobel( *(this->img_t1), this->Ixt1, CV_32F, 1, 0, kernel_size);
		Sobel( *(this->img_t1), this->Iyt1, CV_32F, 0, 1, kernel_size);

		this->Ixt1=abs(this->Ixt1);
		this->Iyt1=abs(this->Iyt1);


		this->Ix=(this->Ixt+Ixt1)/2;
		this->Iy=(this->Iyt+Iyt1)/2;

		GaussianBlur(*(this->img_t),this->I2t,Size(kernel_size,kernel_size),0,0,BORDER_DEFAULT);
		GaussianBlur(*(this->img_t1),this->I2t1,Size(kernel_size,kernel_size),0,0,BORDER_DEFAULT);

		this->It=(this->I2t1)-(this->I2t);


		/*namedWindow( "Ixt", WINDOW_AUTOSIZE );
		imshow( "Ixt", this->Ixt );
		waitKey(0);
		destroyAllWindows();

		namedWindow( "Ixt1", WINDOW_AUTOSIZE );
		imshow( "Ixt1", this->Ixt1 );
		waitKey(0);
		destroyAllWindows();



		namedWindow( "Ix", WINDOW_AUTOSIZE );
		imshow( "Ix", this->Ix );
		waitKey(0);
		destroyAllWindows();



		namedWindow( "Iy", WINDOW_AUTOSIZE );
		imshow( "Iy", this->Iy);
		waitKey(0);
		destroyAllWindows();



		//GaussianBlur(InputArray src, OutputArray dst, Size ksize, double sigmaX, double sigmaY=0, int borderType=BORDER_DEFAULT );



		/*namedWindow( "I2", WINDOW_AUTOSIZE );
		imshow( "I2", this->I2);
		waitKey(0);
		destroyAllWindows();*/


	}


cv::Mat* Calculaflujos::CalculaGradiente(cv::Mat* img_t){

	cv::Point anchor = Point( -1, -1 );
	int delta = 0;
	int ddepth = -1;

	int kernel_size = 3;

	float data[3] = {-1,0,1};
	Mat A = Mat(1, 3, CV_32FC1, &data);

	return &A;



}

Calculaflujos:: ~Calculaflujos() {
	int a=1;
	}
